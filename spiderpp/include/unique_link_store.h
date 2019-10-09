#pragma once

#include "crawler_request.h"

namespace spiderpp
{

//
// ATTENTION: all public methods must be thread-safe
//

class UniqueLinkStore : public QObject
{
	Q_OBJECT

public:
	UniqueLinkStore(QObject* parent);

	bool extractUrl(CrawlerRequest& crawlerRequest) noexcept;

	void addUrl(const Url& url, DownloadRequestType requestType);
	void addUrl(Url&& url, DownloadRequestType requestType);
	void addUrlList(std::vector<Url> urlList, DownloadRequestType requestType);

	bool addCrawledUrl(const Url& url, DownloadRequestType requestType);
	bool hasCrawledRequest(const CrawlerRequest& request);

	void activeRequestReceived(const CrawlerRequest& request);
	void setLimitCrawledLinksCount(int value) noexcept;

	size_t crawledCount() const noexcept;
	size_t pendingCount() const noexcept;
	size_t activeUrlCount() const noexcept;
	std::vector<CrawlerRequest> pendingAndActiveUrls() const;

	std::vector<CrawlerRequest> pendingUrls() const;
	void setPendingUrls(const std::vector<CrawlerRequest>& urls);
	void clearPending();

	std::vector<CrawlerRequest> crawledUrls() const;
	void setCrawledUrls(const std::vector<CrawlerRequest>& urls);

	void clear();

signals:
	void urlAdded();

private:
	void addUrlInternal(CrawlerRequest&& request);

private:
	struct UrlListItemHasher
	{
		size_t operator()(const CrawlerRequest& item) const noexcept
		{
			return hasher(item.url.urlStr().toStdString()) + static_cast<size_t>(item.requestType);
		}

		boost::hash<std::string> hasher;
	};

	using UrlList = std::unordered_set<CrawlerRequest, UrlListItemHasher>;

	UrlList m_pendingUrlList;
	UrlList m_crawledUrlList;
	UrlList m_activeUrlList;

	mutable std::recursive_mutex m_mutex;
	int m_limitCrawledLinksCount;
};

}
