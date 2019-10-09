#pragma once

#include "parsed_page.h"
#include "robots_txt_rules.h"
#include "crawler_request.h"
#include "worker_result.h"
#include "crawler_options.h"
#include "options_link_filter.h"
#include "url_parser.h"

namespace spiderpp
{

class IWorkerPageLoader;
class UniqueLinkStore;
class PageDataCollector;
class HopsChain;
class Hop;
struct DownloadResponse;

class CrawlerWorker : public QObject
{
	Q_OBJECT

public:
	CrawlerWorker(UniqueLinkStore* uniqueLinkStore, IWorkerPageLoader* pageLoader);

signals:
	void workerResult(WorkerResult workerResult) const;

public slots:
	void reinitOptions(const CrawlerOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules);
	void start(const CrawlerOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules);
	void stop();

private slots:
	void extractUrlAndDownload();
	void onAllLoadedDataToBeCleared();
	void onLoadingDone(HopsChain& hopsChain, DownloadRequestType requestType);

private:
	struct SchedulePagesResult
	{
		std::vector<ResourceOnPage> blockedByRobotsTxtLinks;
		std::vector<ResourceOnPage> tooLongLinks;
	};


	bool isExcludedByRegexp(const Url& url) const;
	void fixDDOSGuardRedirectsIfNeeded(std::vector<ParsedPagePtr>& pages) const;
	void handleResponse(HopsChain& hopsChain, DownloadRequestType requestType);
	void handlePage(const Hop& loadResult);

private:
	UniqueLinkStore* m_uniqueLinkStore;
	std::unique_ptr<OptionsLinkFilter> m_optionsLinkFilter;

	bool m_isRunning;

	QTimer* m_defferedProcessingTimer;
	CrawlerOptionsData m_optionsData;
	IWorkerPageLoader* m_pageLoader;

	QVector<QRegExp> m_excludeUrlRegExps;
	UrlParser m_urlParser;
};

}
