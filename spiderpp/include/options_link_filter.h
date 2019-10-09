#pragma once

#include "crawler_options.h"
#include "parsed_page.h"

namespace spiderpp
{

struct LinkInfo;

class OptionsLinkFilter
{
public:
	OptionsLinkFilter(const CrawlerOptionsData& crawlerOptionsData, const cpprobotparser::RobotsTxtRules& robotsTxtRules);

	const Url& startCrawlingPage() const noexcept;
	bool checkRestriction(Restriction restriction, const LinkInfo& linkInfo) const;
	bool isUrlAllowedToCrawl(const Url& url, LinkParameter linkParameter) const;

private:
	bool isLinkBlockedByRobotsTxt(const LinkInfo& linkInfo) const;
	bool isLinkBlockedByRobotsTxt(const Url& url) const;

private:
	CrawlerOptionsData m_crawlerOptionsData;
	cpprobotparser::RobotsTxtRules m_robotsTxtRules;
	QVector<QRegExp> m_excludeUrlRegExps;
};

}
