#pragma once

#include "parsed_page.h"
#include "unique_link_store.h"
#include "crawler_options.h"
#include "requester_wrapper.h"
#include "storage_type.h"
#include "web_host_info.h"

namespace spiderpp
{

class ISpecificLoader;
class IRobotsTxtRules;
class IDownloadHandler;
class IWebScreenShot;
class IHostInfoProvider;
class CrawlerWorker;
class ModelController;
class Requester;
class IWorkerPageLoader;

#ifdef ENABLE_SCREENSHOTS
class IScreenshotMaker;
#endif

struct SiteMapSettings;
struct GetHostInfoResponse;

struct CrawlingProgress
{
	size_t crawledLinkCount;
	size_t pendingLinkCount;
};

class Crawler : public QObject
{
	Q_OBJECT

public:
	enum State
	{
		StateWorking,
		StatePause,

		// This state indicates that:
		// 1. crawler has ended crawling or
		// 2. crawling still didn't started
		StatePending,

		// This state indicates that crawler:
		// 1. checks sitemap
		// 2. checks robots.txt
		// 3. checks IP address using DNS
		// 4. making main page screenshot
		StatePreChecking
	};

	enum DownloaderType
	{
		DownloaderTypeLibCurlMultiSocket,
		DownloaderTypeQNetworkAccessManager
	};

	static Crawler& instance();

	Crawler(QObject* parent = nullptr);
	virtual ~Crawler();

	void setDownloaderType(DownloaderType type);

	void initialize();
	void clearData();
	State state() const noexcept;

	const ISpecificLoader* robotsTxtLoader() const noexcept;
	const ISpecificLoader* xmlSitemapLoader() const noexcept;
	const WebHostInfo* webHostInfo() const;
	std::optional<QByteArray> currentCrawledSiteIPv4() const;

	QString currentCrawledUrl() const noexcept;
	bool readyForRefreshPage() const noexcept;
	ICrawlerOptions* options() const noexcept;

	size_t scannedPagesCount() const;
	size_t pagesCountOnSite() const;
	void setWorkerCount(unsigned workerCount) noexcept;
	bool crawlingFinished() const noexcept;

signals:
	void crawlingProgress(CrawlingProgress progress);
	void crawlerStarted();
	void crawlerStopped();
	void crawlerFinished();
	void crawlerFailed();
	void stateChanged(int state);
	void onAboutClearData();
	void crawlerOptionsLoaded();

public slots:
	void startCrawling();
	void stopCrawling();
	void setUserAgent(const QByteArray& userAgent);

protected slots:
	void onRefreshPageDone();

private slots:
	void onCrawlingSessionInitialized();
	void onCrawlerOptionsSomethingChanged();

protected:
	virtual IHostInfoProvider* createHostInfoProvider() const;

#ifdef ENABLE_SCREENSHOTS
	virtual IScreenshotMaker* createScreenshotMaker();
#endif

	virtual IDownloadHandler* createDownloader() const;

	IWorkerPageLoader* createWorkerPageLoader() const;
	const UniqueLinkStore* uniqueLinkStore() const noexcept;

private:
	bool isPreinitialized() const;
	void initializeCrawlingSession();
	void onHostInfoResponse(Requester* requester, const GetHostInfoResponse& response);
	void tryToLoadCrawlingDependencies();
	void clearDataImpl();
	void setState(State state);
	unsigned workerCount() const noexcept;

private:
	static Crawler* s_instance;

	ISpecificLoader* m_robotsTxtLoader;
	ISpecificLoader* m_xmlSitemapLoader;
	UniqueLinkStore* m_uniqueLinkStore;

	ICrawlerOptions* m_options;
	unsigned int m_theradCount;

	std::vector<CrawlerWorker*> m_workers;

	State m_state;
	State m_prevState;

	IDownloadHandler* m_downloader;

	RequesterWrapper m_hostInfoRequester;
	WebHostInfo* m_webHostInfo;
	std::unique_ptr<QHostInfo> m_hostInfo;

	DownloaderType m_downloaderType;

	bool m_crawlingFinished;
};

}

Q_DECLARE_METATYPE(spiderpp::CrawlingProgress)
