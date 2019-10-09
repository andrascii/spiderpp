#pragma once

#include "page_parser_helpers.h"
#include "url.h"

namespace spiderpp
{

enum class HttpLoadType
{
	RequestTypeGet,
	RequestTypeHead
};

struct DataToLoad
{
	Url url;
	HttpLoadType requestType;

	bool operator==(const DataToLoad& other) const
	{
		return requestType == other.requestType && url.compare(other.url);
	}

	friend bool operator<(const DataToLoad& lhs, const DataToLoad& rhs)
	{
		return lhs.url < rhs.url;
	}
};

struct CrawlerRequestHasher
{
	size_t operator()(const DataToLoad& item) const noexcept
	{
		return hasher(item.url.toString().toStdString()) + static_cast<size_t>(item.requestType);
	}

	boost::hash<std::string> hasher;
};

bool operator<(const DataToLoad& lhs, const DataToLoad& rhs);

}

Q_DECLARE_METATYPE(spiderpp::HttpLoadType)
