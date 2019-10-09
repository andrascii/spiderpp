#include "crawler.h"
#include "crawler_worker.h"
#include "thread_manager.h"
#include "qt_based_download_handler.h"
#include "service_locator.h"
#include "inotification_service.h"
#include "notification_service.h"
#include "helpers.h"
#include "common_constants.h"
#include "robots_txt_loader.h"
#include "multi_socket_download_handler.h"
#include "multi_request_page_loader.h"
#include "qt_page_loader.h"

namespace spiderpp
{

Crawler* Crawler::s_instance = nullptr;

Crawler& Crawler::instance()
{
	return *s_instance;
}

Crawler::Crawler(QObject* parent)
	: QObject(parent)
	, m_robotsTxtLoader(new RobotsTxtLoader(this))
	, m_uniqueLinkStore(nullptr)
	, m_options(new CrawlerOptions(this))
	, m_theradCount(0)
	, m_state(StatePending)
	, m_downloader(nullptr)
	, m_downloaderType(DownloaderTypeQNetworkAccessManager)
	, m_crawlingFinished(false)
{
	ServiceLocator* serviceLocator = ServiceLocator::instance();
	serviceLocator->addService<INotificationService>(new NotificationService);

	ASSERT(s_instance == nullptr && "Allowed only one instance of Crawler");
	ASSERT(qRegisterMetaType<CrawlerOptionsData>() > -1);
	ASSERT(qRegisterMetaType<cpprobotparser::RobotsTxtRules>());

	Common::Helpers::connectSignalsToMetaMethod(
		options()->qobject(),
		Common::Helpers::allUserSignals(options()->qobject()),
		this,
		Common::Helpers::metaMethodOfSlot(this, "onCrawlerOptionsSomethingChanged()")
	);

	VERIFY(connect(m_robotsTxtLoader->qobject(), SIGNAL(ready()), this, SLOT(onCrawlingSessionInitialized()), Qt::QueuedConnection));

	s_instance = this;
}

Crawler::~Crawler()
{
	for (CrawlerWorker* worker : m_workers)
	{
		VERIFY(QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection));
	}

	ThreadManager::destroy();

	s_instance = nullptr;

	ServiceLocator::instance()->destroyService<INotificationService>();
}

void Crawler::setDownloaderType(DownloaderType type)
{
	ASSERT(type == DownloaderTypeLibCurlMultiSocket || type == DownloaderTypeQNetworkAccessManager);
	m_downloaderType = type;
}

void Crawler::initialize()
{
	m_downloader = createDownloader();

	ThreadManager& threadManager = ThreadManager::instance();
	threadManager.moveObjectToThread(m_downloader->qobject(), "DownloaderThread");

	m_uniqueLinkStore = new UniqueLinkStore(this);

	for (unsigned i = 0; i < workerCount(); ++i)
	{
		m_workers.push_back(new CrawlerWorker(m_uniqueLinkStore, createWorkerPageLoader()));
		threadManager.moveObjectToThread(m_workers.back(), QString("CrawlerWorkerThread#%1").arg(i).toLatin1());
	}
}

void Crawler::clearData()
{
	ASSERT(state() == StatePending || state() == StatePause);

	clearDataImpl();

	setState(StatePending);

	emit onAboutClearData();
}

void Crawler::clearDataImpl()
{
	m_crawlingFinished = false;
	m_uniqueLinkStore->clear();
}

void Crawler::setState(State state)
{
	m_prevState = m_state;
	m_state = state;
	emit stateChanged(state);
}

unsigned Crawler::workerCount() const noexcept
{
	const unsigned hardwareConcurrency = std::thread::hardware_concurrency();

	if (m_theradCount == 0 || m_theradCount > hardwareConcurrency * 2)
	{
		return hardwareConcurrency;
	}

	return m_theradCount;
}

Crawler::State Crawler::state() const noexcept
{
	return m_state;
}

void Crawler::startCrawling()
{
	m_crawlingFinished = false;
	setState(StatePreChecking);

	if (m_options->pauseRangeFrom() == -1 && m_options->pauseRangeTo() == -1 ||
		m_options->pauseRangeFrom() == 0 && m_options->pauseRangeTo() == 0 || !m_options->pauseRangeEnabled())
	{
		VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "resetPauseRange", Qt::BlockingQueuedConnection));
	}
	else
	{
		const int rangeFrom = qMax(0, m_options->pauseRangeFrom());
		const int rangeTo = qMax(rangeFrom, m_options->pauseRangeTo());
		VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setPauseRange",
			Qt::BlockingQueuedConnection, Q_ARG(int, rangeFrom), Q_ARG(int, rangeTo)));
	}

	VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setTimeout",
		Qt::BlockingQueuedConnection, Q_ARG(int, m_options->limitTimeout())));

	VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setMaxRedirects",
		Qt::BlockingQueuedConnection, Q_ARG(int, m_options->maxRedirectsToFollow())));

	VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setMaxParallelConnections",
		Qt::BlockingQueuedConnection, Q_ARG(int, m_options->maxParallelConnections())));

	m_uniqueLinkStore->setLimitCrawledLinksCount(m_options->limitSearchTotal());

	tryToLoadCrawlingDependencies();
}

void Crawler::stopCrawling()
{
	if (state() == StatePause || state() == StatePending)
	{
		return;
	}

	setState(StatePause);

	for (CrawlerWorker* worker : m_workers)
	{
		VERIFY(QMetaObject::invokeMethod(worker, "stop", Qt::BlockingQueuedConnection));
	}

	emit crawlerStopped();

	ServiceLocator* serviceLocator = ServiceLocator::instance();
	serviceLocator->service<INotificationService>()->info(tr("Crawler state"), tr("Crawler stopped"));
}

void Crawler::onCrawlingSessionInitialized()
{
	if (!isPreinitialized())
	{
		return;
	}

	setState(StateWorking);
	setUserAgent(m_options->userAgent());

	if (m_options->useProxy())
	{
		VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setProxy",
			Qt::BlockingQueuedConnection,
			Q_ARG(const QString&, m_options->proxyHostName()),
			Q_ARG(int, m_options->proxyPort()),
			Q_ARG(const QString&, m_options->proxyUser()),
			Q_ARG(const QString&, m_options->proxyPassword())));
	}
	else
	{
		VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "resetProxy", Qt::BlockingQueuedConnection));
	}

	m_uniqueLinkStore->addUrl(m_options->startCrawlingPage(), DownloadRequestType::RequestTypeGet);

	for (CrawlerWorker* worker : m_workers)
	{
		VERIFY(QMetaObject::invokeMethod(worker, "start", Qt::QueuedConnection,
			Q_ARG(const CrawlerOptionsData&, m_options->data()), Q_ARG(cpprobotparser::RobotsTxtRules, cpprobotparser::RobotsTxtRules(m_robotsTxtLoader->content().constData()))));
	}

	emit crawlerStarted();

	ServiceLocator::instance()->service<INotificationService>()->info(tr("Crawler state"), tr("Crawler started"));
}

void Crawler::onCrawlerOptionsSomethingChanged()
{
}

bool Crawler::isPreinitialized() const
{
	return m_robotsTxtLoader->isReady();
}

void Crawler::tryToLoadCrawlingDependencies()
{
	DEBUG_ASSERT(m_options->startCrawlingPage().isValid());

	if (isPreinitialized())
	{
		onCrawlingSessionInitialized();
		return;
	}

	m_robotsTxtLoader->setHost(m_options->startCrawlingPage());
	m_robotsTxtLoader->load();
}

IWorkerPageLoader* Crawler::createWorkerPageLoader() const
{
	switch (m_downloaderType)
	{
		case DownloaderTypeQNetworkAccessManager:
		{
			INFOLOG << "Creating QtPageLoader";
			return new QtPageLoader(m_uniqueLinkStore);
		}
		case DownloaderTypeLibCurlMultiSocket:
		{
			INFOLOG << "Creating MultiRequestPageLoader";
			return new MultiRequestPageLoader(m_uniqueLinkStore);
		}
	}

	ASSERT(!"Invalid downloader type");

	return nullptr;
}

IDownloadHandler* Crawler::createDownloader() const
{
	switch (m_downloaderType)
	{
		case DownloaderTypeQNetworkAccessManager:
		{
			INFOLOG << "Creating QtBasedDownloadHandler";
			return new QtBasedDownloadHandler;
		}
		case DownloaderTypeLibCurlMultiSocket:
		{
			INFOLOG << "Creating MultiSocketDownloadHandler";
			return new MultiSocketDownloadHandler;
		}
	}

	ASSERT(!"Invalid downloader type");

	return nullptr;
}

const ISpecificLoader* Crawler::robotsTxtLoader() const noexcept
{
	return m_robotsTxtLoader;
}

QString Crawler::currentCrawledUrl() const noexcept
{
	return m_options->startCrawlingPage().urlStr();
}

void Crawler::setUserAgent(const QByteArray& userAgent)
{
	VERIFY(QMetaObject::invokeMethod(m_downloader->qobject(), "setUserAgent",
		Qt::BlockingQueuedConnection, Q_ARG(const QByteArray&, userAgent)));
}

const UniqueLinkStore* Crawler::uniqueLinkStore() const noexcept
{
	return m_uniqueLinkStore;
}

ICrawlerOptions* Crawler::options() const noexcept
{
	return m_options;
}

size_t Crawler::scannedPagesCount() const
{
	return m_uniqueLinkStore->crawledCount();
}

size_t Crawler::pagesCountOnSite() const
{
	return m_uniqueLinkStore->pendingCount();
}

void Crawler::setWorkerCount(unsigned workerCount) noexcept
{
	m_theradCount = workerCount;
}

bool Crawler::crawlingFinished() const noexcept
{
	return m_crawlingFinished;
}

}
