#pragma once

#include "unique_link_store.h"
#include "crawler_options.h"
#include "requester_wrapper.h"

namespace spiderpp
{

class ISpecificLoader;
class IRobotsTxtRules;
class IDownloadHandler;
class SpiderWorker;
class Requester;
class IWorkerPageLoader;

class Spider : public QObject
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

	static Spider& instance();

	Spider(QObject* parent = nullptr);
	virtual ~Spider();

	void setDownloaderType(DownloaderType type);

	void initialize();
	void clearData();
	State state() const noexcept;

	const ISpecificLoader* robotsTxtLoader() const noexcept;

	QString currentCrawledUrl() const noexcept;
	ICrawlerOptions* options() const noexcept;

	size_t scannedPagesCount() const;
	size_t pagesCountOnSite() const;
	void setWorkerCount(unsigned workerCount) noexcept;
	bool crawlingFinished() const noexcept;

signals:
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

private slots:
	void onCrawlingSessionInitialized();
	void onCrawlerOptionsSomethingChanged();

protected:
	virtual IDownloadHandler* createDownloader() const;

	IWorkerPageLoader* createWorkerPageLoader() const;
	const LoadSchedule* uniqueLinkStore() const noexcept;

private:
	bool isPreinitialized() const;
	void tryToLoadCrawlingDependencies();
	void clearDataImpl();
	void setState(State state);
	unsigned workerCount() const noexcept;

private:
	static Spider* s_instance;

	ISpecificLoader* m_robotsTxtLoader;
	LoadSchedule* m_loadSchedule;

	ICrawlerOptions* m_options;
	unsigned int m_theradCount;

	std::vector<SpiderWorker*> m_workers;

	State m_state;
	State m_prevState;

	IDownloadHandler* m_downloader;
	DownloaderType m_downloaderType;

	bool m_crawlingFinished;
};

}
