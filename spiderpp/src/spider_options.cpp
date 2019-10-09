#include "spider_options.h"

namespace spiderpp
{

SpiderOptions::SpiderOptions(QObject* parent)
	: QObject(parent)
{
}

QObject* SpiderOptions::qobject() const noexcept
{
	return const_cast<SpiderOptions* const>(this);
}

Url SpiderOptions::startCrawlingPage() const noexcept
{
	return m_data.startCrawlingPage;
}

void SpiderOptions::setStartCrawlingPage(const Url& url)
{
	m_data.startCrawlingPage = url;

	emit startCrawlingPageChanged(m_data.startCrawlingPage);
}

int SpiderOptions::limitSearchTotal() const noexcept
{
	return m_data.limitSearchTotal;
}

void SpiderOptions::setLimitSearchTotal(int value) noexcept
{
	m_data.limitSearchTotal = value;

	emit limitSearchTotalChanged(m_data.limitSearchTotal);
}

int SpiderOptions::limitTimeout() const noexcept
{
	return m_data.limitTimeout;
}

void SpiderOptions::setLimitTimeout(int value) noexcept
{
	m_data.limitTimeout = value;

	emit limitTimeoutChanged(m_data.limitTimeout);
}

int SpiderOptions::maxRedirectsToFollow() const noexcept
{
	return m_data.maxRedirectsToFollow;
}

void SpiderOptions::setMaxRedirectsToFollow(int value) noexcept
{
	m_data.maxRedirectsToFollow = value;

	emit maxRedirectsToFollowChanged(m_data.maxRedirectsToFollow);
}

int SpiderOptions::maxParallelConnections() const noexcept
{
	return m_data.maxParallelConnections;
}

void SpiderOptions::setMaxParallelConnections(int value) noexcept
{
	m_data.maxParallelConnections = value;
	emit maxParallelConnectionsChanged(m_data.maxParallelConnections);
}

int SpiderOptions::maxLinksCountOnPage() const noexcept
{
	return m_data.maxLinksCountOnPage;
}

void SpiderOptions::setMaxLinksCountOnPage(int value) noexcept
{
	m_data.maxLinksCountOnPage = value;

	emit maxLinksCountOnPageChanged(m_data.maxLinksCountOnPage);
}

int SpiderOptions::maxTitleLength() const noexcept
{
	return m_data.maxTitleLength;
}

void SpiderOptions::setMaxTitleLength(int value) noexcept
{
	m_data.maxTitleLength = value;

	emit maxTitleLengthChanged(m_data.maxTitleLength);
}

int SpiderOptions::minTitleLength() const noexcept
{
	return m_data.minTitleLength;
}

void SpiderOptions::setMinTitleLength(int value) noexcept
{
	m_data.minTitleLength = value;

	emit minTitleLengthChanged(m_data.minTitleLength);
}

int SpiderOptions::maxDescriptionLength() const noexcept
{
	return m_data.maxDescriptionLength;
}

void SpiderOptions::setMaxDescriptionLength(int value) noexcept
{
	m_data.maxDescriptionLength = value;

	emit maxDescriptionLengthChanged(m_data.maxDescriptionLength);
}

int SpiderOptions::minDescriptionLength() const noexcept
{
	return m_data.minDescriptionLength;
}

void SpiderOptions::setMinDescriptionLength(int value) noexcept
{
	m_data.minDescriptionLength = value;

	emit minDescriptionLengthChanged(m_data.minDescriptionLength);
}

int SpiderOptions::maxH1LengthChars() const noexcept
{
	return m_data.maxH1LengthChars;
}

void SpiderOptions::setMaxH1LengthChars(int value) noexcept
{
	m_data.maxH1LengthChars = value;

	emit maxH1LengthCharsChanged(m_data.maxH1LengthChars);
}

int SpiderOptions::maxH2LengthChars() const noexcept
{
	return m_data.maxH2LengthChars;
}

void SpiderOptions::setMaxH2LengthChars(int value) noexcept
{
	m_data.maxH2LengthChars = value;

	emit maxH2LengthCharsChanged(m_data.maxH2LengthChars);
}

int SpiderOptions::maxImageAltTextChars() const noexcept
{
	return m_data.maxImageAltTextChars;
}

void SpiderOptions::setMaxImageAltTextChars(int value) noexcept
{
	m_data.maxImageAltTextChars = value;

	emit maxImageAltTextCharsChanged(m_data.maxImageAltTextChars);
}

int SpiderOptions::maxImageSizeKb() const noexcept
{
	return m_data.maxImageSizeKb;
}

void SpiderOptions::setMaxImageSizeKb(int value) noexcept
{
	m_data.maxImageSizeKb = value;

	emit maxImageSizeKbChanged(m_data.maxImageSizeKb);
}

int SpiderOptions::maxPageSizeKb() const noexcept
{
	return m_data.maxPageSizeKb;
}

void SpiderOptions::setMaxPageSizeKb(int value) noexcept
{
	m_data.maxPageSizeKb = value;

	emit maxPageSizeKbChanged(m_data.maxPageSizeKb);
}

bool SpiderOptions::useProxy() const noexcept
{
	return m_data.useProxy;
}

void SpiderOptions::setUseProxy(bool value) noexcept
{
	m_data.useProxy = value;

	emit useProxyChanged(m_data.useProxy);
}

QString SpiderOptions::proxyHostName() const noexcept
{
	return m_data.proxyHostName;
}

void SpiderOptions::setProxyHostName(const QString& value)
{
	m_data.proxyHostName = value;

	emit proxyHostNameChanged(m_data.proxyHostName);
}

int SpiderOptions::proxyPort() const noexcept
{
	return m_data.proxyPort;
}

void SpiderOptions::setProxyPort(int value) noexcept
{
	m_data.proxyPort = value;

	emit proxyPortChanged(m_data.proxyPort);
}

QString SpiderOptions::proxyUser() const noexcept
{
	return m_data.proxyUser;
}

void SpiderOptions::setProxyUser(const QString& value)
{
	m_data.proxyUser = value;

	emit proxyUserChanged(m_data.proxyUser);
}

QString SpiderOptions::proxyPassword() const noexcept
{
	return m_data.proxyPassword;
}

void SpiderOptions::setProxyPassword(const QString& value)
{
	m_data.proxyPassword = value;

	emit proxyPasswordChanged(m_data.proxyPassword);
}

bool SpiderOptions::checkExternalLinks() const noexcept
{
	return m_data.checkExternalLinks;
}

void SpiderOptions::setCheckExternalLinks(bool value) noexcept
{
	m_data.checkExternalLinks = value;

	emit checkExternalLinksChanged(m_data.checkExternalLinks);
}

bool SpiderOptions::followInternalNofollow() const noexcept
{
	return m_data.followInternalNofollow;
}

void SpiderOptions::setFollowInternalNofollow(bool value) noexcept
{
	m_data.followInternalNofollow = value;

	emit followInternalNofollowChanged(m_data.followInternalNofollow);
}

bool SpiderOptions::followExternalNofollow() const noexcept
{
	return m_data.followExternalNofollow;
}

void SpiderOptions::setFollowExternalNofollow(bool value) noexcept
{
	m_data.followExternalNofollow = value;

	emit followExternalNofollowChanged(m_data.followExternalNofollow);
}

bool SpiderOptions::checkCanonicals() const noexcept
{
	return m_data.checkCanonicals;
}

void SpiderOptions::setCheckCanonicals(bool value) noexcept
{
	m_data.checkCanonicals = value;

	emit checkCanonicalsChanged(m_data.checkCanonicals);
}

bool SpiderOptions::checkSubdomains() const noexcept
{
	return m_data.checkSubdomains;
}

void SpiderOptions::setCheckSubdomains(bool value) noexcept
{
	m_data.checkSubdomains = value;

	emit checkSubdomainsChanged(m_data.checkSubdomains);
}

bool SpiderOptions::crawlOutsideOfStartFolder() const noexcept
{
	return m_data.crawlOutsideOfStartFolder;
}

void SpiderOptions::setCrawlOutsideOfStartFolder(bool value) noexcept
{
	m_data.crawlOutsideOfStartFolder = value;

	emit crawlOutsideOfStartFolderChanged(m_data.crawlOutsideOfStartFolder);
}

bool SpiderOptions::crawlMetaHrefLangLinks() const noexcept
{
	return m_data.crawlMetaHrefLangLinks;
}

void SpiderOptions::setCrawlMetaHrefLangLinks(bool value) noexcept
{
	m_data.crawlMetaHrefLangLinks = value;
	emit crawlMetaHrefLangLinksChanged(m_data.crawlMetaHrefLangLinks);
}

bool SpiderOptions::followRobotsTxtRules() const noexcept
{
	return m_data.followRobotsTxtRules;
}

void SpiderOptions::setFollowRobotsTxtRules(bool value) noexcept
{
	m_data.followRobotsTxtRules = value;

	emit followRobotsTxtRulesChanged(m_data.followRobotsTxtRules);
}

cpprobotparser::WellKnownUserAgent SpiderOptions::userAgentToFollow() const noexcept
{
	return m_data.userAgentToFollow;
}

void SpiderOptions::setUserAgentToFollow(cpprobotparser::WellKnownUserAgent value) noexcept
{
	m_data.userAgentToFollow = value;

	emit userAgentToFollowChanged(m_data.userAgentToFollow);
}

ParserTypeFlags SpiderOptions::parserTypeFlags() const noexcept
{
	return m_data.parserTypeFlags;
}

void SpiderOptions::setParserTypeFlags(ParserTypeFlags value) noexcept
{
	m_data.parserTypeFlags = value;

	emit parserTypeFlagsChanged(m_data.parserTypeFlags);
}

int SpiderOptions::pauseRangeFrom() const noexcept
{
	return m_data.pauseRangeFrom;
}

void SpiderOptions::setPauseRangeFrom(int value) noexcept
{
	m_data.pauseRangeFrom = value;

	emit pauseRangeFromChanged(m_data.pauseRangeFrom);
}

int SpiderOptions::pauseRangeTo() const noexcept
{
	return m_data.pauseRangeTo;
}

void SpiderOptions::setPauseRangeTo(int value) noexcept
{
	m_data.pauseRangeTo = value;

	emit pauseRangeToChanged(m_data.pauseRangeTo);
}

bool SpiderOptions::pauseRangeEnabled() const noexcept
{
	return m_data.pauseRangeEnabled;
}

void SpiderOptions::setPauseRangeEnabled(bool value) noexcept
{
	m_data.pauseRangeEnabled = value;

	emit pauseRangeEnabledChanged(m_data.pauseRangeEnabled);
}

QByteArray SpiderOptions::userAgent() const noexcept
{
	return m_data.userAgent;
}

void SpiderOptions::setUserAgent(const QByteArray& value)
{
	m_data.userAgent = value;

	emit userAgentChanged(m_data.userAgent);
}

QString SpiderOptions::excludeUrlRegExps() const noexcept
{
	return m_data.excludeUrlRegExps;
}

void SpiderOptions::setExcludeUrlRegExps(const QString& value)
{
	m_data.excludeUrlRegExps = value;

	emit excludeUrlRegExpsChanged(m_data.excludeUrlRegExps);
}

const SpiderOptionsData& SpiderOptions::data() const noexcept
{
	return m_data;
}

void SpiderOptions::setData(const SpiderOptionsData& data) noexcept
{
	m_data = data;
	emit dataChanged(m_data);
}

void SpiderOptions::setData(SpiderOptionsData&& data) noexcept
{
	m_data = std::move(data);
	emit dataChanged(m_data);
}

}
