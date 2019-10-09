#include "spider_worker.h"
#include "unique_link_store.h"
#include "page_parser_helpers.h"
#include "download_request.h"
#include "download_response.h"
#include "service_locator.h"
#include "common_constants.h"
#include "spider.h"
#include "finally.h"
#include "iworker_page_loader.h"
#include "url_parser.h"

namespace
{

QVector<QRegExp> getRegExps(const QString& str)
{
	QVector<QRegExp> result;
	QStringList parts = str.split("\n", QString::SkipEmptyParts);
	for (auto it = parts.cbegin(); it != parts.cend(); ++it)
	{
		QRegExp regExp = QRegExp((*it).trimmed());
		if (regExp.isValid() && !regExp.isEmpty())
		{
			result.push_back(regExp);
		}
	}
	return result;
}

}

namespace spiderpp
{

SpiderWorker::SpiderWorker(UniqueLinkStore* uniqueLinkStore, IWorkerPageLoader* pageLoader)
	: QObject(nullptr)
	, m_uniqueLinkStore(uniqueLinkStore)
	, m_isRunning(false)
	, m_defferedProcessingTimer(new QTimer(this))
	, m_pageLoader(pageLoader)
{
	m_pageLoader->qobject()->setParent(this);

	ASSERT(qRegisterMetaType<RedirectChain>("RedirectChain"));
	ASSERT(qRegisterMetaType<HttpLoadType>("DownloadRequestType"));
	ASSERT(qRegisterMetaType<LoadResult>());

	VERIFY(connect(m_pageLoader->qobject(), SIGNAL(pageLoaded(RedirectChain&, HttpLoadType)),
		this, SLOT(onLoadingDone(RedirectChain&, HttpLoadType)), Qt::QueuedConnection));

	VERIFY(connect(m_uniqueLinkStore, &UniqueLinkStore::urlAdded, this,
		&SpiderWorker::extractUrlAndDownload, Qt::QueuedConnection));

	VERIFY(connect(&Spider::instance(), &Spider::onAboutClearData,
		this, &SpiderWorker::onAllLoadedDataToBeCleared, Qt::QueuedConnection));

	VERIFY(connect(m_defferedProcessingTimer, &QTimer::timeout,
		this, &SpiderWorker::extractUrlAndDownload));

	m_defferedProcessingTimer->setInterval(1000);
	m_defferedProcessingTimer->setSingleShot(true);
}

void SpiderWorker::start(const CrawlerOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules)
{
	DEBUG_ASSERT(thread() == QThread::currentThread());

	m_isRunning = true;
	m_optionsData = optionsData;
	m_excludeUrlRegExps = getRegExps(m_optionsData.excludeUrlRegExps);

	m_pageLoader->setReceiveState(IWorkerPageLoader::CanReceivePages);

	reinitOptions(optionsData, robotsTxtRules);
	extractUrlAndDownload();
}

void SpiderWorker::stop()
{
	DEBUG_ASSERT(thread() == QThread::currentThread());

	m_isRunning = false;

	m_pageLoader->setReceiveState(IWorkerPageLoader::CantReceivePages);
}

void SpiderWorker::reinitOptions(const CrawlerOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules)
{
	DEBUG_ASSERT(thread() == QThread::currentThread());

	m_optionsLinkFilter.reset(new OptionsLinkFilter(optionsData, robotsTxtRules));
	m_excludeUrlRegExps = getRegExps(optionsData.excludeUrlRegExps);
}

void SpiderWorker::extractUrlAndDownload()
{
	if (!m_isRunning)
	{
		return;
	}

	DataToLoad crawlerRequest;

	if (!m_pageLoader->canPullLoading() || !m_isRunning)
	{
		m_defferedProcessingTimer->start();
		return;
	}

	const bool isUrlExtracted = m_uniqueLinkStore->extractUrl(crawlerRequest);

	if (isUrlExtracted)
	{
		m_pageLoader->performLoading(crawlerRequest);
	}
}

void SpiderWorker::onAllLoadedDataToBeCleared()
{
	m_pageLoader->clear();
}

bool SpiderWorker::isExcludedByRegexp(const Url& url) const
{
	for (auto it = m_excludeUrlRegExps.cbegin(); it != m_excludeUrlRegExps.cend(); ++it)
	{
		const auto urlString = url.toDisplayString();

		if (it->indexIn(urlString) != -1)
		{
			return true;
		}
	}

	return false;
}

void SpiderWorker::onLoadingDone(RedirectChain& redirectChain, HttpLoadType requestType)
{
	DataToLoad readyRequest = { redirectChain.firstLoadResult().url(), requestType };
	m_uniqueLinkStore->activeRequestReceived(readyRequest);

	extractUrlAndDownload();

	DEBUG_ASSERT(requestType != HttpLoadType::RequestTypeHead || redirectChain.firstLoadResult().body().isEmpty());

	handleResponse(redirectChain, requestType);
}

/*
void CrawlerWorker::fixDDOSGuardRedirectsIfNeeded(std::vector<ParsedPagePtr>& pages) const
{
	const int pagesCount = static_cast<int>(pages.size());

	for (int i = pagesCount - 1; i >= 0; --i)
	{
		// fix ddos guard redirects like page.html -> ddos-site -> page.html
		for (int j = i - 1; j >= 0; --j)
		{
			if (pages[i]->url.urlStr() == pages[j]->url.urlStr())
			{
				const Url redirectUrl = pages[j]->redirectedUrl;

				std::swap(*pages[j], *pages[i]);

				pages[j]->redirectedUrl = redirectUrl;
			}
		}
	}
}*/

void SpiderWorker::handleResponse(RedirectChain& redirectChain, HttpLoadType requestType)
{
	bool checkUrl = false;

	//fixDDOSGuardRedirectsIfNeeded(pages);

	for (LoadResult& loadResult : redirectChain)
	{
		if (checkUrl && !loadResult.url().fragment().isEmpty())
		{
			loadResult.url().setFragment(QString());
		}

		const bool isUrlAdded = !checkUrl || m_uniqueLinkStore->addCrawledUrl(loadResult.url(), requestType);
		checkUrl = true;

		handlePage(loadResult);
	}
}

void SpiderWorker::handlePage(const LoadResult& loadResult)
{
	UrlParser::UrlList urlList = m_urlParser.urlList(loadResult);

	urlList.dofollowAhrefs.erase(std::remove_if(urlList.dofollowAhrefs.begin(), urlList.dofollowAhrefs.end(), [this](const Url& url)
	{
		return m_optionsLinkFilter->isUrlAllowedToCrawl(url, AhrefRelType::DofollowParameter) ||
			isExcludedByRegexp(url);
	}));

	urlList.nofollowAhrefs.erase(std::remove_if(urlList.nofollowAhrefs.begin(), urlList.nofollowAhrefs.end(), [this](const Url& url)
	{
		return m_optionsLinkFilter->isUrlAllowedToCrawl(url, AhrefRelType::NofollowParameter) ||
			isExcludedByRegexp(url);
	}));

	m_uniqueLinkStore->addUrlList(std::move(urlList.dofollowAhrefs), HttpLoadType::RequestTypeGet);
	m_uniqueLinkStore->addUrlList(std::move(urlList.nofollowAhrefs), HttpLoadType::RequestTypeGet);

	if (!m_optionsData.crawlMetaHrefLangLinks)
	{
		return;
	}

	urlList.hreflangs.erase(std::remove_if(urlList.hreflangs.begin(), urlList.hreflangs.end(), [this](const Url& url)
	{
		return m_optionsLinkFilter->isUrlAllowedToCrawl(url, AhrefRelType::DofollowParameter) ||
			isExcludedByRegexp(url);
	}));

	m_uniqueLinkStore->addUrlList(std::move(urlList.hreflangs), HttpLoadType::RequestTypeGet);
}

}
