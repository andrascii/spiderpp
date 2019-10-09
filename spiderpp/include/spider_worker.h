#pragma once

#include "robots_txt_rules.h"
#include "data_to_load.h"
#include "spider_options.h"
#include "options_link_filter.h"
#include "url_parser.h"
#include "load_result.h"

namespace spiderpp
{

class IWorkerPageLoader;
class LoadSchedule;
class RedirectChain;
struct DownloadResponse;

class SpiderWorker : public QObject
{
	Q_OBJECT

public:
	SpiderWorker(LoadSchedule* loadSchedule, IWorkerPageLoader* pageLoader);

signals:
	void onAboutLoadResult(LoadResult loadResult) const;

public slots:
	void reinitOptions(const SpiderOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules);
	void start(const SpiderOptionsData& optionsData, cpprobotparser::RobotsTxtRules robotsTxtRules);
	void stop();

private slots:
	void extractUrlAndDownload();
	void onAllLoadedDataToBeCleared();
	void onLoadingDone(RedirectChain& redirectChain, HttpLoadType requestType);

private:
	bool isExcludedByRegexp(const Url& url) const;
	//void fixDDOSGuardRedirectsIfNeeded(std::vector<ParsedPagePtr>& pages) const;
	void handleResponse(RedirectChain& redirectChain, HttpLoadType requestType);
	void handlePage(const LoadResult& loadResult);

private:
	LoadSchedule* m_loadSchedule;
	std::unique_ptr<OptionsLinkFilter> m_optionsLinkFilter;

	bool m_isRunning;

	QTimer* m_defferedProcessingTimer;
	SpiderOptionsData m_optionsData;
	IWorkerPageLoader* m_pageLoader;

	QVector<QRegExp> m_excludeUrlRegExps;
	UrlParser m_urlParser;
};

}
