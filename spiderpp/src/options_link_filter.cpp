#include "options_link_filter.h"
#include "parsed_page.h"
#include  "page_parser_helpers.h"

namespace spiderpp
{

OptionsLinkFilter::OptionsLinkFilter(const CrawlerOptionsData& crawlerOptionsData, const cpprobotparser::RobotsTxtRules& robotsTxtRules)
	: m_crawlerOptionsData(crawlerOptionsData)
	, m_robotsTxtRules(robotsTxtRules)
{
}

const Url& OptionsLinkFilter::startCrawlingPage() const noexcept
{
	return m_crawlerOptionsData.startCrawlingPage;
}

bool OptionsLinkFilter::checkRestriction(Restriction restriction, const LinkInfo& linkInfo, const MetaRobotsFlagsSet& metaRobotsFlags) const
{
	if(!PageParserHelpers::isHttpOrHttpsScheme(linkInfo.url))
	{
		return false;
	}

	const bool isUrlExternal = PageParserHelpers::isUrlExternal(m_crawlerOptionsData.startCrawlingPage, linkInfo.url, m_crawlerOptionsData.checkSubdomains);
	const bool isNofollowLink = linkInfo.linkParameter == LinkParameter::NofollowParameter;

	const bool isExternalNofollowNotAllowed = isNofollowLink && isUrlExternal && !m_crawlerOptionsData.followExternalNofollow;
	const bool isInternalNofollowNotAllowed = isNofollowLink && !isUrlExternal && !m_crawlerOptionsData.followInternalNofollow;

	if (restriction == Restriction::RestrictionNofollowNotAllowed)
	{
		return isInternalNofollowNotAllowed || isExternalNofollowNotAllowed;
	}

	const bool isSubdomain = PageParserHelpers::isSubdomain(m_crawlerOptionsData.startCrawlingPage, linkInfo.url);

	if (restriction == Restriction::RestrictionSubdomainNotAllowed)
	{
		return isSubdomain && !m_crawlerOptionsData.checkSubdomains;
	}

	if (restriction == Restriction::RestrictionBlockedByFolder)
	{
		return !isUrlExternal && !isSubdomain && !m_crawlerOptionsData.crawlOutsideOfStartFolder &&
			!PageParserHelpers::isUrlInsideBaseUrlFolder(m_crawlerOptionsData.startCrawlingPage, linkInfo.url);
	}

	if (restriction == Restriction::RestrictionExternalLinksNotAllowed)
	{
		return isUrlExternal && !m_crawlerOptionsData.checkExternalLinks;
	}

	if (restriction == Restriction::RestrictionBlockedByRobotsTxtRules)
	{
		return !isUrlExternal && m_crawlerOptionsData.followRobotsTxtRules && 
			isLinkBlockedByRobotsTxt(linkInfo);
	}
	
	if (restriction == Restriction::RestrictionBlockedByMetaRobotsRules)
	{
		return !isUrlExternal && m_crawlerOptionsData.followRobotsTxtRules && 
			isLinkBlockedByMetaRobots(metaRobotsFlags);
	}

	return false;
}

bool OptionsLinkFilter::isLinkBlockedByRobotsTxt(const LinkInfo& linkInfo) const
{
	const QByteArray checkingUrl = linkInfo.url.toDisplayString().toUtf8();
	return !m_robotsTxtRules.isUrlAllowed(checkingUrl.constData(), m_crawlerOptionsData.userAgentToFollow);
}

}
