#include "options_link_filter.h"
#include  "page_parser_helpers.h"

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

OptionsLinkFilter::OptionsLinkFilter(const CrawlerOptionsData& crawlerOptionsData, const cpprobotparser::RobotsTxtRules& robotsTxtRules)
	: m_crawlerOptionsData(crawlerOptionsData)
	, m_robotsTxtRules(robotsTxtRules)
{
	m_excludeUrlRegExps = getRegExps(crawlerOptionsData.excludeUrlRegExps);
}

const Url& OptionsLinkFilter::startCrawlingPage() const noexcept
{
	return m_crawlerOptionsData.startCrawlingPage;
}

bool OptionsLinkFilter::isUrlAllowedToCrawl(const Url& url, AhrefRelType ahrefRelType) const
{
	if (!PageParserHelpers::isHttpOrHttpsScheme(url))
	{
		return false;
	}

	const bool isUrlExternal = PageParserHelpers::isUrlExternal(m_crawlerOptionsData.startCrawlingPage, url, m_crawlerOptionsData.checkSubdomains);
	const bool isNofollowLink = ahrefRelType == AhrefRelType::NofollowParameter;

	const bool isExternalNofollowNotAllowed = isNofollowLink && isUrlExternal && !m_crawlerOptionsData.followExternalNofollow;
	const bool isInternalNofollowNotAllowed = isNofollowLink && !isUrlExternal && !m_crawlerOptionsData.followInternalNofollow;

	if (isExternalNofollowNotAllowed || isInternalNofollowNotAllowed)
	{
		return false;
	}

	const bool isSubdomain = PageParserHelpers::isSubdomain(m_crawlerOptionsData.startCrawlingPage, url);
	const bool isSubdomainNotAllowed = isSubdomain && !m_crawlerOptionsData.checkSubdomains;

	if (isSubdomainNotAllowed)
	{
		return false;;
	}

	const bool isBlockedByFolder =
		!isUrlExternal &&
		!isSubdomain &&
		!m_crawlerOptionsData.crawlOutsideOfStartFolder &&
		!PageParserHelpers::isUrlInsideBaseUrlFolder(m_crawlerOptionsData.startCrawlingPage, url);

	if (isBlockedByFolder)
	{
		return false;
	}

	// all external urls are not allowed
	const bool isAllExternalUrlsAreNotAllowed = isUrlExternal && !m_crawlerOptionsData.checkExternalLinks;

	if (isAllExternalUrlsAreNotAllowed)
	{
		return false;
	}

	const bool isBlockedByRobotsTxt =
		!isUrlExternal &&
		m_crawlerOptionsData.followRobotsTxtRules &&
		isLinkBlockedByRobotsTxt(url);

	if (isBlockedByRobotsTxt)
	{
		return false;
	}

	return true;
}

bool OptionsLinkFilter::isLinkBlockedByRobotsTxt(const Url& url) const
{
	const QByteArray checkingUrl = url.toDisplayString().toUtf8();
	return !m_robotsTxtRules.isUrlAllowed(checkingUrl.constData(), m_crawlerOptionsData.userAgentToFollow);
}

}
