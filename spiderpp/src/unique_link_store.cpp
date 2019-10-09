#include "unique_link_store.h"
#include "service_locator.h"
#include "common_constants.h"

namespace spiderpp
{

LoadSchedule::LoadSchedule(QObject* parent)
	: QObject(parent)
	, m_limitCrawledLinksCount(-1)
{
}

void LoadSchedule::addUrl(const Url& url, HttpLoadType requestType)
{
	std::lock_guard locker(m_mutex);
	addUrlInternal(DataToLoad{ url, requestType });
}

void LoadSchedule::addUrl(Url&& url, HttpLoadType requestType)
{
	std::lock_guard locker(m_mutex);
	addUrlInternal(DataToLoad{ std::move(url), requestType });
}

bool LoadSchedule::extractUrl(DataToLoad& crawlerRequest) noexcept
{
	std::lock_guard locker(m_mutex);

	if (m_pendingUrlList.empty())
	{
		return false;
	}

	{
		const auto iter = m_pendingUrlList.begin();
		crawlerRequest = *iter;

		if (m_crawledUrlList.find(crawlerRequest) != m_crawledUrlList.end())
		{
			INFOLOG << "Url from pending found in crawled url list:"
				<< crawlerRequest.url.urlStr()
				<< static_cast<int>(crawlerRequest.requestType);
		}

		DEBUG_ASSERT(m_crawledUrlList.find(crawlerRequest) == m_crawledUrlList.end());

		DataToLoad request = std::move(*iter);
		m_crawledUrlList.insert(request);
		m_activeUrlList.insert(request);
		m_pendingUrlList.erase(iter);
	}

	return true;
}

void LoadSchedule::addUrlList(std::vector<Url> urlList, HttpLoadType requestType)
{
	if (urlList.empty())
	{
		return;
	}

	std::lock_guard locker(m_mutex);

	{
		QSignalBlocker blocker(this);

		std::for_each(std::make_move_iterator(urlList.begin()), std::make_move_iterator(urlList.end()), [&](Url&& url)
		{
			addUrl(std::move(url), requestType);
		});
	}

	emit urlAdded();
}

bool LoadSchedule::addCrawledUrl(const Url& url, HttpLoadType requestType)
{
	DEBUG_ASSERT(url.isValid());
	DEBUG_ASSERT(url.fragment().isEmpty());
	std::lock_guard locker(m_mutex);

	const size_t countLinks = m_pendingUrlList.size() + m_crawledUrlList.size();

	if (m_limitCrawledLinksCount > 0 &&
		m_crawledUrlList.size() + m_pendingUrlList.size() >= static_cast<size_t>(m_limitCrawledLinksCount))
	{
		// don't add this url if we exceed the "Limit Crawled Links Count" option
		return false;
	}

	DataToLoad request{ url, requestType };

	const auto& [iter, insertionResult] = m_crawledUrlList.insert(request);
	m_pendingUrlList.erase(std::move(request));

	return insertionResult;
}

bool LoadSchedule::hasCrawledRequest(const DataToLoad& request)
{
	std::lock_guard locker(m_mutex);

	return m_crawledUrlList.find(request) != m_crawledUrlList.end();
}

void LoadSchedule::activeRequestReceived(const DataToLoad& request)
{
	std::lock_guard locker(m_mutex);
	m_activeUrlList.erase(request);
}

size_t LoadSchedule::crawledCount() const noexcept
{
	std::lock_guard locker(m_mutex);
	return m_crawledUrlList.size();
}

size_t LoadSchedule::pendingCount() const noexcept
{
	std::lock_guard locker(m_mutex);
	return m_pendingUrlList.size();
}

size_t LoadSchedule::activeUrlCount() const noexcept
{
	std::lock_guard locker(m_mutex);
	return m_activeUrlList.size();
}

std::vector<DataToLoad> LoadSchedule::crawledUrls() const
{
	std::lock_guard locker(m_mutex);
	std::vector<DataToLoad> result;
	result.reserve(m_crawledUrlList.size());

	for (auto it = m_crawledUrlList.begin(); it != m_crawledUrlList.end(); ++it)
	{
		result.emplace_back(*it);
	}

	return result;
}

std::vector<DataToLoad> LoadSchedule::pendingUrls() const
{
	std::lock_guard locker(m_mutex);
	std::vector<DataToLoad> result;
	result.reserve(m_pendingUrlList.size());

	for (auto it = m_pendingUrlList.begin(); it != m_pendingUrlList.end(); ++it)
	{
		result.emplace_back(*it);
	}

	return result;
}

std::vector<DataToLoad> LoadSchedule::pendingAndActiveUrls() const
{
	std::lock_guard locker(m_mutex);
	std::vector<DataToLoad> result;
	result.reserve(m_pendingUrlList.size());

	for (auto it = m_pendingUrlList.begin(); it != m_pendingUrlList.end(); ++it)
	{
		result.emplace_back(*it);
	}

	for (auto it = m_activeUrlList.begin(); it != m_activeUrlList.end(); ++it)
	{
		result.emplace_back(*it);
	}

	return result;
}

void LoadSchedule::setCrawledUrls(const std::vector<DataToLoad>& urls)
{
	std::lock_guard locker(m_mutex);
	m_crawledUrlList.clear();
	m_crawledUrlList.insert(urls.begin(), urls.end());
}

void LoadSchedule::setPendingUrls(const std::vector<DataToLoad>& urls)
{
	std::lock_guard locker(m_mutex);
	m_pendingUrlList.clear();
	m_pendingUrlList.insert(urls.begin(), urls.end());
}

void LoadSchedule::clear()
{
	std::lock_guard locker(m_mutex);

	m_pendingUrlList.clear();
	m_crawledUrlList.clear();
	m_activeUrlList.clear();
}

void LoadSchedule::clearPending()
{
	std::lock_guard locker(m_mutex);
	m_pendingUrlList.clear();
}

void LoadSchedule::setLimitCrawledLinksCount(int value) noexcept
{
	m_limitCrawledLinksCount = value;
}

void LoadSchedule::addUrlInternal(DataToLoad&& request)
{
	DEBUG_ASSERT(request.url.isValid());
	DEBUG_ASSERT(request.url.fragment().isEmpty());
	const size_t countLinks = m_pendingUrlList.size() + m_crawledUrlList.size();

	if (m_limitCrawledLinksCount > 0 && countLinks >= static_cast<size_t>(m_limitCrawledLinksCount))
	{
		// don't add this url if we exceed the "Limit Crawled Links Count" option
		return;
	}

	// this function must be obfuscated and additionally tied to a serial number
	if (m_crawledUrlList.find(request) != m_crawledUrlList.end())
	{
		return;
	}

	m_pendingUrlList.insert(std::move(request));

	emit urlAdded();
}

}