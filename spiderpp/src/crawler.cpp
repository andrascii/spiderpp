#include "crawler.h"
#include "crawler_worker.h"
#include "robots_txt_rules.h"
#include "robots_txt_loader.h"
#include "thread_manager.h"
#include "qt_based_download_handler.h"
#include "host_info_provider.h"
#include "service_locator.h"
#include "inotification_service.h"
#include "notification_service.h"
#include "xml_sitemap_loader.h"
#include "get_host_info_request.h"
#include "get_host_info_response.h"
#include "helpers.h"
#include "common_constants.h"
#include "proper_404_checker.h"

#ifdef ENABLE_SCREENSHOTS
#include "screenshot_maker.h"
#endif

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
	, m_xmlSitemapLoader(new XmlSitemapLoader(static_cast<RobotsTxtLoader*>(m_robotsTxtLoader), this))
	, m_uniqueLinkStore(nullptr)
	, m_options(new CrawlerOptions(this))
	, m_theradCount(0)
	, m_crawlingStateTimer(new QTimer(this))
	, m_state(StatePending)
	, m_downloader(nullptr)
	, m_webHostInfo(nullptr)
	, m_downloaderType(DownloaderTypeQNetworkAccessManager)
	, m_crawlingFinished(false)
{
	ServiceLocator* serviceLocator = ServiceLocator::instance();
	serviceLocator->addService<INotificationService>(new NotificationService);

	ASSERT(s_instance == nullptr && "Allowed only one instance of Crawler");
	ASSERT(qRegisterMetaType<WorkerResult>());
	ASSERT(qRegisterMetaType<ParsedPagePtr>());
	ASSERT(qRegisterMetaType<std::vector<ParsedPagePtr>>());
	ASSERT(qRegisterMetaType<CrawlingProgress>());
	ASSERT(qRegisterMetaType<CrawlerOptionsData>() > -1);
	ASSERT(qRegisterMetaType<RobotsTxtRules>());

	VERIFY(connect(m_crawlingStateTimer, &QTimer::timeout, this, &Crawler::onAboutCrawlingState));

	Common::Helpers::connectSignalsToMetaMethod(
		options()->qobject(),
		Common::Helpers::allUserSignals(options()->qobject()),
		this,
		Common::Helpers::metaMethodOfSlot(this, "onCrawlerOptionsSomethingChanged()")
	);

	VERIFY(connect(m_robotsTxtLoader->qobject(), SIGNAL(ready()), this, SLOT(onCrawlingSessionInitialized()), Qt::QueuedConnection));
	VERIFY(connect(m_xmlSitemapLoader->qobject(), SIGNAL(ready()), this, SLOT(onCrawlingSessionInitialized()), Qt::QueuedConnection));

	m_crawlingStateTimer->setInterval(100);

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
	m_webHostInfo = new WebHostInfo(this, m_xmlSitemapLoader, m_robotsTxtLoader);

#ifdef ENABLE_SCREENSHOTS
	VERIFY(connect(m_webHostInfo, &WebHostInfo::webScreenshotLoaded, this, &Crawler::onSessionChanged));
#endif

	ThreadManager& threadManager = ThreadManager::instance();

	threadManager.moveObjectToThread(m_downloader->qobject(), "DownloaderThread");
	threadManager.moveObjectToThread(createHostInfoProvider()->qobject(), "BackgroundThread");
	threadManager.moveObjectToThread(new Proper404Checker, "BackgroundThread");

#ifdef ENABLE_SCREENSHOTS
	threadManager.moveObjectToThread(createScreenshotMaker()->qobject(), "BackgroundThread");
#endif

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

	m_hostInfo.reset();

	emit onAboutClearData();
}

void Crawler::clearDataImpl()
{
	m_crawlingFinished = false;
	CrawlerSharedState::instance()->clear();
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

	initializeCrawlingSession();
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

	m_crawlingStateTimer->stop();

	emit crawlerStopped();

	ServiceLocator* serviceLocator = ServiceLocator::instance();
	serviceLocator->service<INotificationService>()->info(tr("Crawler state"), tr("Crawler stopped"));
}

void Crawler::onAboutCrawlingState()
{
	CrawlingProgress progress;

	const CrawlerSharedState* state = CrawlerSharedState::instance();

	const int sequencedDataCollectionCount = state->sequencedDataCollectionLinksCount();
	const int modelControllerCrawledLinksCount = state->modelControllerCrawledLinksCount();
	const int modelControllerAcceptedLinksCount = state->modelControllerAcceptedLinksCount();
	const int uniqueLinkStoreCrawledCount = state->downloaderCrawledLinksCount();
	const int uniqueLinkStorePendingCount = state->downloaderPendingLinksCount();

	const size_t controllerPending = qMax(uniqueLinkStoreCrawledCount, modelControllerCrawledLinksCount) - modelControllerCrawledLinksCount;
	const size_t seqCollPending = qMax(modelControllerAcceptedLinksCount, sequencedDataCollectionCount) - sequencedDataCollectionCount;
	const size_t additionalPendingCount = controllerPending + seqCollPending;

	progress.crawledLinkCount = sequencedDataCollectionCount;
	progress.pendingLinkCount = uniqueLinkStorePendingCount + additionalPendingCount;

	emit crawlingProgress(progress);

	const bool isCrawlingEnded = uniqueLinkStorePendingCount == 0 &&
		sequencedDataCollectionCount > 0 &&
		uniqueLinkStoreCrawledCount == state->workersProcessedLinksCount() &&
		state->modelControllerCrawledLinksCount() == state->workersProcessedLinksCount() &&
		modelControllerAcceptedLinksCount == sequencedDataCollectionCount;

	if (isCrawlingEnded)
	{
		stopCrawling();
		setState(StatePending);

		ServiceLocator* serviceLocator = ServiceLocator::instance();
		serviceLocator->service<INotificationService>()->info(tr("Crawler state"), tr("Program has ended crawling"));
		m_crawlingFinished = true;

		emit crawlerFinished();
	}
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
			Q_ARG(const CrawlerOptionsData&, m_options->data()), Q_ARG(RobotsTxtRules, RobotsTxtRules(m_robotsTxtLoader->content()))));
	}

	m_crawlingStateTimer->start();

	emit crawlerStarted();

	ServiceLocator::instance()->service<INotificationService>()->info(tr("Crawler state"), tr("Crawler started"));
}

void Crawler::onCrawlerOptionsSomethingChanged()
{
}

bool Crawler::isPreinitialized() const
{
	return m_robotsTxtLoader->isReady() && m_xmlSitemapLoader->isReady();
}

void Crawler::initializeCrawlingSession()
{
	if (m_hostInfo)
	{
		tryToLoadCrawlingDependencies();
		return;
	}

	GetHostInfoRequest request(m_options->startCrawlingPage());

	m_hostInfoRequester.reset(request, this, &Crawler::onHostInfoResponse);

	m_hostInfoRequester->start();
}

void Crawler::onHostInfoResponse(Requester*, const GetHostInfoResponse& response)
{
	m_hostInfoRequester->stop();

	if (response.hostInfo.error() != QHostInfo::NoError)
	{
		ServiceLocator* serviceLocator = ServiceLocator::instance();

		serviceLocator->service<INotificationService>()->error(
			tr("DNS Lookup Failed!"),
			tr("I'm sorry but I cannot find this website\n"
				"Please, be sure that you entered a valid address")
		);

		setState(StatePending);
		emit crawlerFailed();

		return;
	}

	m_hostInfo.reset(new QHostInfo(response.hostInfo));
	m_options->setStartCrawlingPage(response.url);

	tryToLoadCrawlingDependencies();
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
	m_xmlSitemapLoader->setHost(m_options->startCrawlingPage());
	m_webHostInfo->reset(m_options->startCrawlingPage());
	m_robotsTxtLoader->load();
	m_xmlSitemapLoader->load();
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

IHostInfoProvider* Crawler::createHostInfoProvider() const
{
	return new HostInfoProvider;
}

#ifdef ENABLE_SCREENSHOTS
IScreenshotMaker* Crawler::createScreenshotMaker()
{
	return new ScreenshotMaker;
}
#endif

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

const ISpecificLoader* Crawler::xmlSitemapLoader() const noexcept
{
	return m_xmlSitemapLoader;
}

const WebHostInfo * Crawler::webHostInfo() const
{
	return m_webHostInfo;
}

std::optional<QByteArray> Crawler::currentCrawledSiteIPv4() const
{
	if (m_hostInfo && !m_hostInfo->addresses().isEmpty())
	{
		return m_hostInfo->addresses().first().toString().toUtf8();
	}

	return std::make_optional<QByteArray>();
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

bool Crawler::readyForRefreshPage() const noexcept
{
	return state() == StatePause || state() == StatePending;
}

}
