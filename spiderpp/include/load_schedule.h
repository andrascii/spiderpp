#pragma once

#include "data_to_load.h"

namespace spiderpp
{

//
// ATTENTION: all public methods must be thread-safe
//

class LoadSchedule : public QObject
{
	Q_OBJECT

public:
	LoadSchedule(QObject* parent);

	bool extractUrl(DataToLoad& crawlerRequest) noexcept;

	void addUrl(const Url& url, HttpLoadType requestType);
	void addUrl(Url&& url, HttpLoadType requestType);
	void addUrlList(std::vector<Url> urlList, HttpLoadType requestType);

	bool addCrawledUrl(const Url& url, HttpLoadType requestType);
	bool hasCrawledRequest(const DataToLoad& request);

	void activeRequestReceived(const DataToLoad& request);
	void setLimitCrawledLinksCount(int value) noexcept;

	size_t crawledCount() const noexcept;
	size_t pendingCount() const noexcept;
	size_t activeUrlCount() const noexcept;
	std::vector<DataToLoad> pendingAndActiveUrls() const;

	std::vector<DataToLoad> pendingUrls() const;
	void setPendingUrls(const std::vector<DataToLoad>& urls);
	void clearPending();

	std::vector<DataToLoad> crawledUrls() const;
	void setCrawledUrls(const std::vector<DataToLoad>& urls);

	void clear();

signals:
	void urlAdded();

private:
	void addUrlInternal(DataToLoad&& request);

private:
	struct UrlListItemHasher
	{
		size_t operator()(const DataToLoad& item) const noexcept
		{
			return hasher(item.url.urlStr().toStdString()) + static_cast<size_t>(item.requestType);
		}

		boost::hash<std::string> hasher;
	};

	using UrlList = std::unordered_set<DataToLoad, UrlListItemHasher>;

	UrlList m_pendingUrlList;
	UrlList m_crawledUrlList;
	UrlList m_activeUrlList;

	mutable std::recursive_mutex m_mutex;
	int m_limitCrawledLinksCount;
};

}
