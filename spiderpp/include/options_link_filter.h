#pragma once

#include "spider_options.h"
#include "ahref_rel_type.h"

namespace spiderpp
{

struct LinkInfo;

class OptionsLinkFilter
{
public:
	OptionsLinkFilter(const SpiderOptionsData& crawlerOptionsData, const cpprobotparser::RobotsTxtRules& robotsTxtRules);

	const Url& startCrawlingPage() const noexcept;
	bool isUrlAllowedToCrawl(const Url& url, AhrefRelType ahrefRelType) const;

private:
	bool isLinkBlockedByRobotsTxt(const Url& url) const;

private:
	SpiderOptionsData m_crawlerOptionsData;
	cpprobotparser::RobotsTxtRules m_robotsTxtRules;
	QVector<QRegExp> m_excludeUrlRegExps;
};

}
