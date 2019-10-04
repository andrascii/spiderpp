#include "crawler_options.h"

namespace CrawlerEngine
{

CrawlerOptions::CrawlerOptions(QObject* parent)
	: QObject(parent)
{
    std::fill(m_data.extractorRuleTypes.begin(), m_data.extractorRuleTypes.end(), -1);
    std::fill(m_data.extractionTypes.begin(), m_data.extractionTypes.end(), -1);
}

QObject* CrawlerOptions::qobject() const noexcept
{
	return const_cast<CrawlerOptions* const>(this);
}

Url CrawlerOptions::startCrawlingPage() const noexcept
{
	return m_data.startCrawlingPage;
}

void CrawlerOptions::setStartCrawlingPage(const Url& url)
{
	m_data.startCrawlingPage = url;

	emit startCrawlingPageChanged(m_data.startCrawlingPage);
}

int CrawlerOptions::limitMaxUrlLength() const noexcept
{
	return m_data.limitMaxUrlLength;
}

void CrawlerOptions::setLimitMaxUrlLength(int value) noexcept
{
	m_data.limitMaxUrlLength = value;

	emit limitMaxUrlLengthChanged(m_data.limitMaxUrlLength);
}

int CrawlerOptions::limitSearchTotal() const noexcept
{
	return m_data.limitSearchTotal;
}

void CrawlerOptions::setLimitSearchTotal(int value) noexcept
{
	m_data.limitSearchTotal = value;

	emit limitSearchTotalChanged(m_data.limitSearchTotal);
}

int CrawlerOptions::limitTimeout() const noexcept
{
	return m_data.limitTimeout;
}

void CrawlerOptions::setLimitTimeout(int value) noexcept
{
	m_data.limitTimeout = value;

	emit limitTimeoutChanged(m_data.limitTimeout);
}

int CrawlerOptions::maxRedirectsToFollow() const noexcept
{
	return m_data.maxRedirectsToFollow;
}

void CrawlerOptions::setMaxRedirectsToFollow(int value) noexcept
{
	m_data.maxRedirectsToFollow = value;

	emit maxRedirectsToFollowChanged(m_data.maxRedirectsToFollow);
}

int CrawlerOptions::maxParallelConnections() const noexcept
{
	return m_data.maxParallelConnections;
}

void CrawlerOptions::setMaxParallelConnections(int value) noexcept
{
	m_data.maxParallelConnections = value;
	emit maxParallelConnectionsChanged(m_data.maxParallelConnections);
}

int CrawlerOptions::maxLinksCountOnPage() const noexcept
{
	return m_data.maxLinksCountOnPage;
}

void CrawlerOptions::setMaxLinksCountOnPage(int value) noexcept
{
	m_data.maxLinksCountOnPage = value;

	emit maxLinksCountOnPageChanged(m_data.maxLinksCountOnPage);
}

int CrawlerOptions::maxTitleLength() const noexcept
{
	return m_data.maxTitleLength;
}

void CrawlerOptions::setMaxTitleLength(int value) noexcept
{
	m_data.maxTitleLength = value;

	emit maxTitleLengthChanged(m_data.maxTitleLength);
}

int CrawlerOptions::minTitleLength() const noexcept
{
	return m_data.minTitleLength;
}

void CrawlerOptions::setMinTitleLength(int value) noexcept
{
	m_data.minTitleLength = value;

	emit minTitleLengthChanged(m_data.minTitleLength);
}

int CrawlerOptions::maxDescriptionLength() const noexcept
{
	return m_data.maxDescriptionLength;
}

void CrawlerOptions::setMaxDescriptionLength(int value) noexcept
{
	m_data.maxDescriptionLength = value;

	emit maxDescriptionLengthChanged(m_data.maxDescriptionLength);
}

int CrawlerOptions::minDescriptionLength() const noexcept
{
	return m_data.minDescriptionLength;
}

void CrawlerOptions::setMinDescriptionLength(int value) noexcept
{
	m_data.minDescriptionLength = value;

	emit minDescriptionLengthChanged(m_data.minDescriptionLength);
}

int CrawlerOptions::maxH1LengthChars() const noexcept
{
	return m_data.maxH1LengthChars;
}

void CrawlerOptions::setMaxH1LengthChars(int value) noexcept
{
	m_data.maxH1LengthChars = value;

	emit maxH1LengthCharsChanged(m_data.maxH1LengthChars);
}

int CrawlerOptions::maxH2LengthChars() const noexcept
{
	return m_data.maxH2LengthChars;
}

void CrawlerOptions::setMaxH2LengthChars(int value) noexcept
{
	m_data.maxH2LengthChars = value;

	emit maxH2LengthCharsChanged(m_data.maxH2LengthChars);
}

int CrawlerOptions::maxImageAltTextChars() const noexcept
{
	return m_data.maxImageAltTextChars;
}

void CrawlerOptions::setMaxImageAltTextChars(int value) noexcept
{
	m_data.maxImageAltTextChars = value;

	emit maxImageAltTextCharsChanged(m_data.maxImageAltTextChars);
}

int CrawlerOptions::maxImageSizeKb() const noexcept
{
	return m_data.maxImageSizeKb;
}

void CrawlerOptions::setMaxImageSizeKb(int value) noexcept
{
	m_data.maxImageSizeKb = value;

	emit maxImageSizeKbChanged(m_data.maxImageSizeKb);
}

int CrawlerOptions::maxPageSizeKb() const noexcept
{
	return m_data.maxPageSizeKb;
}

void CrawlerOptions::setMaxPageSizeKb(int value) noexcept
{
	m_data.maxPageSizeKb = value;

	emit maxPageSizeKbChanged(m_data.maxPageSizeKb);
}

bool CrawlerOptions::useProxy() const noexcept
{
	return m_data.useProxy;
}

void CrawlerOptions::setUseProxy(bool value) noexcept
{
	m_data.useProxy = value;

	emit useProxyChanged(m_data.useProxy);
}

QString CrawlerOptions::proxyHostName() const noexcept
{
	return m_data.proxyHostName;
}

void CrawlerOptions::setProxyHostName(const QString& value)
{
	m_data.proxyHostName = value;

	emit proxyHostNameChanged(m_data.proxyHostName);
}

int CrawlerOptions::proxyPort() const noexcept
{
	return m_data.proxyPort;
}

void CrawlerOptions::setProxyPort(int value) noexcept
{
	m_data.proxyPort = value;

	emit proxyPortChanged(m_data.proxyPort);
}

QString CrawlerOptions::proxyUser() const noexcept
{
	return m_data.proxyUser;
}

void CrawlerOptions::setProxyUser(const QString& value)
{
	m_data.proxyUser = value;

	emit proxyUserChanged(m_data.proxyUser);
}

QString CrawlerOptions::proxyPassword() const noexcept
{
	return m_data.proxyPassword;
}

void CrawlerOptions::setProxyPassword(const QString& value)
{
	m_data.proxyPassword = value;

	emit proxyPasswordChanged(m_data.proxyPassword);
}

bool CrawlerOptions::checkExternalLinks() const noexcept
{
	return m_data.checkExternalLinks;
}

void CrawlerOptions::setCheckExternalLinks(bool value) noexcept
{
	m_data.checkExternalLinks = value;

	emit checkExternalLinksChanged(m_data.checkExternalLinks);
}

bool CrawlerOptions::followInternalNofollow() const noexcept
{
	return m_data.followInternalNofollow;
}

void CrawlerOptions::setFollowInternalNofollow(bool value) noexcept
{
	m_data.followInternalNofollow = value;

	emit followInternalNofollowChanged(m_data.followInternalNofollow);
}

bool CrawlerOptions::followExternalNofollow() const noexcept
{
	return m_data.followExternalNofollow;
}

void CrawlerOptions::setFollowExternalNofollow(bool value) noexcept
{
	m_data.followExternalNofollow = value;

	emit followExternalNofollowChanged(m_data.followExternalNofollow);
}

bool CrawlerOptions::checkCanonicals() const noexcept
{
	return m_data.checkCanonicals;
}

void CrawlerOptions::setCheckCanonicals(bool value) noexcept
{
	m_data.checkCanonicals = value;

	emit checkCanonicalsChanged(m_data.checkCanonicals);
}

bool CrawlerOptions::checkSubdomains() const noexcept
{
	return m_data.checkSubdomains;
}

void CrawlerOptions::setCheckSubdomains(bool value) noexcept
{
	m_data.checkSubdomains = value;

	emit checkSubdomainsChanged(m_data.checkSubdomains);
}

bool CrawlerOptions::crawlOutsideOfStartFolder() const noexcept
{
	return m_data.crawlOutsideOfStartFolder;
}

void CrawlerOptions::setCrawlOutsideOfStartFolder(bool value) noexcept
{
	m_data.crawlOutsideOfStartFolder = value;

	emit crawlOutsideOfStartFolderChanged(m_data.crawlOutsideOfStartFolder);
}

bool CrawlerOptions::crawlMetaHrefLangLinks() const noexcept
{
	return m_data.crawlMetaHrefLangLinks;
}

void CrawlerOptions::setCrawlMetaHrefLangLinks(bool value) noexcept
{
	m_data.crawlMetaHrefLangLinks = value;
	emit crawlMetaHrefLangLinksChanged(m_data.crawlMetaHrefLangLinks);
}

bool CrawlerOptions::followRobotsTxtRules() const noexcept
{
	return m_data.followRobotsTxtRules;
}

void CrawlerOptions::setFollowRobotsTxtRules(bool value) noexcept
{
	m_data.followRobotsTxtRules = value;

	emit followRobotsTxtRulesChanged(m_data.followRobotsTxtRules);
}

UserAgentType CrawlerOptions::userAgentToFollow() const noexcept
{
	return m_data.userAgentToFollow;
}

void CrawlerOptions::setUserAgentToFollow(UserAgentType value) noexcept
{
	m_data.userAgentToFollow = value;

	emit userAgentToFollowChanged(m_data.userAgentToFollow);
}

ParserTypeFlags CrawlerOptions::parserTypeFlags() const noexcept
{
	return m_data.parserTypeFlags;
}

void CrawlerOptions::setParserTypeFlags(ParserTypeFlags value) noexcept
{
	m_data.parserTypeFlags = value;

	emit parserTypeFlagsChanged(m_data.parserTypeFlags);
}

int CrawlerOptions::pauseRangeFrom() const noexcept
{
	return m_data.pauseRangeFrom;
}

void CrawlerOptions::setPauseRangeFrom(int value) noexcept
{
	m_data.pauseRangeFrom = value;

	emit pauseRangeFromChanged(m_data.pauseRangeFrom);
}

int CrawlerOptions::pauseRangeTo() const noexcept
{
	return m_data.pauseRangeTo;
}

void CrawlerOptions::setPauseRangeTo(int value) noexcept
{
	m_data.pauseRangeTo = value;

	emit pauseRangeToChanged(m_data.pauseRangeTo);
}

bool CrawlerOptions::pauseRangeEnabled() const noexcept
{
	return m_data.pauseRangeEnabled;
}

void CrawlerOptions::setPauseRangeEnabled(bool value) noexcept
{
	m_data.pauseRangeEnabled = value;

	emit pauseRangeEnabledChanged(m_data.pauseRangeEnabled);
}

QByteArray CrawlerOptions::userAgent() const noexcept
{
	return m_data.userAgent;
}

void CrawlerOptions::setUserAgent(const QByteArray& value)
{
	m_data.userAgent = value;

	emit userAgentChanged(m_data.userAgent);
}

QString CrawlerOptions::excludeUrlRegExps() const noexcept
{
	return m_data.excludeUrlRegExps;
}

void CrawlerOptions::setExcludeUrlRegExps(const QString& value)
{
	m_data.excludeUrlRegExps = value;

	emit excludeUrlRegExpsChanged(m_data.excludeUrlRegExps);
}


bool CrawlerOptions::searchYandexMetricaCounters() const noexcept
{
	return m_data.searchYandexMetricaCounters;
}


void CrawlerOptions::setSearchYandexMetricaCounters(bool value)
{
	m_data.searchYandexMetricaCounters = value;

	emit searchYandexMetricaCountersChanged(m_data.searchYandexMetricaCounters);
}


bool CrawlerOptions::searchYandexMetricaCounter1() const noexcept
{
	return m_data.searchYandexMetricaCounter1;
}


void CrawlerOptions::setSearchYandexMetricaCounter1(bool value)
{
	m_data.searchYandexMetricaCounter1 = value;

	emit searchYandexMetricaCounter1Changed(m_data.searchYandexMetricaCounter1);
}

int CrawlerOptions::yandexMetricaCounter1Id() const noexcept
{
	return m_data.yandexMetricaCounter1Id;
}

void CrawlerOptions::setYandexMetricaCounter1Id(int value)
{
	m_data.yandexMetricaCounter1Id = value;

	emit yandexMetricaCounter1IdChanged(m_data.yandexMetricaCounter1Id);
}

bool CrawlerOptions::searchYandexMetricaCounter2() const noexcept
{
	return m_data.searchYandexMetricaCounter2;
}

void CrawlerOptions::setSearchYandexMetricaCounter2(bool value)
{
	m_data.searchYandexMetricaCounter2 = value;

	emit searchYandexMetricaCounter2Changed(m_data.searchYandexMetricaCounter2);
}

int CrawlerOptions::yandexMetricaCounter2Id() const noexcept
{
	return m_data.yandexMetricaCounter2Id;
}


void CrawlerOptions::setYandexMetricaCounter2Id(int value)
{
	m_data.yandexMetricaCounter2Id = value;

	emit yandexMetricaCounter2IdChanged(m_data.yandexMetricaCounter2Id);
}

bool CrawlerOptions::searchYandexMetricaCounter3() const noexcept
{
	return m_data.searchYandexMetricaCounter3;
}

void CrawlerOptions::setSearchYandexMetricaCounter3(bool value)
{
	m_data.searchYandexMetricaCounter3 = value;

	emit searchYandexMetricaCounter3Changed(m_data.searchYandexMetricaCounter3);
}

int CrawlerOptions::yandexMetricaCounter3Id() const noexcept
{
	return m_data.yandexMetricaCounter3Id;
}

void CrawlerOptions::setYandexMetricaCounter3Id(int value)
{
	m_data.yandexMetricaCounter3Id = value;

	emit yandexMetricaCounter3IdChanged(m_data.yandexMetricaCounter3Id);
}

bool CrawlerOptions::searchYandexMetricaCounter4() const noexcept
{
	return m_data.searchYandexMetricaCounter4;
}

void CrawlerOptions::setSearchYandexMetricaCounter4(bool value)
{
	m_data.searchYandexMetricaCounter4 = value;

	emit searchYandexMetricaCounter4Changed(m_data.searchYandexMetricaCounter4);
}

int CrawlerOptions::yandexMetricaCounter4Id() const noexcept
{
	return m_data.yandexMetricaCounter4Id;
}

void CrawlerOptions::setYandexMetricaCounter4Id(int value)
{
	m_data.yandexMetricaCounter4Id = value;

	emit yandexMetricaCounter4IdChanged(m_data.yandexMetricaCounter4Id);
}

bool CrawlerOptions::searchYandexMetricaCounter5() const noexcept
{
	return m_data.searchYandexMetricaCounter5;
}

void CrawlerOptions::setSearchYandexMetricaCounter5(bool value)
{
	m_data.searchYandexMetricaCounter5 = value;

	emit searchYandexMetricaCounter5Changed(m_data.searchYandexMetricaCounter5);
}

int CrawlerOptions::yandexMetricaCounter5Id() const noexcept
{
	return m_data.yandexMetricaCounter5Id;
}


void CrawlerOptions::setYandexMetricaCounter5Id(int value)
{
	m_data.yandexMetricaCounter5Id = value;

	emit yandexMetricaCounter5IdChanged(m_data.yandexMetricaCounter5Id);
}

bool CrawlerOptions::searchGoogleAnalyticsCounters() const noexcept
{
	return m_data.searchGoogleAnalyticsCounters;
}


void CrawlerOptions::setSearchGoogleAnalyticsCounters(bool value)
{
	m_data.searchGoogleAnalyticsCounters = value;

	emit searchGoogleAnalyticsCountersChanged(m_data.searchGoogleAnalyticsCounters);
}


bool CrawlerOptions::searchGoogleAnalyticsCounter1() const noexcept
{
	return m_data.searchGoogleAnalyticsCounter1;
}


void CrawlerOptions::setSearchGoogleAnalyticsCounter1(bool value)
{
	m_data.searchGoogleAnalyticsCounter1 = value;

	emit searchGoogleAnalyticsCounter1Changed(m_data.searchGoogleAnalyticsCounter1);
}

const QString& CrawlerOptions::googleAnalyticsCounter1Id() const noexcept
{
	return m_data.googleAnalyticsCounter1Id;
}

void CrawlerOptions::setGoogleAnalyticsCounter1Id(const QString& value)
{
	m_data.googleAnalyticsCounter1Id = value;

	emit googleAnalyticsCounter1IdChanged(m_data.googleAnalyticsCounter1Id);
}

bool CrawlerOptions::searchGoogleAnalyticsCounter2() const noexcept
{
	return m_data.searchGoogleAnalyticsCounter2;
}

void CrawlerOptions::setSearchGoogleAnalyticsCounter2(bool value)
{
	m_data.searchGoogleAnalyticsCounter2 = value;

	emit searchGoogleAnalyticsCounter2Changed(m_data.searchGoogleAnalyticsCounter2);
}

const QString& CrawlerOptions::googleAnalyticsCounter2Id() const noexcept
{
	return m_data.googleAnalyticsCounter2Id;
}


void CrawlerOptions::setGoogleAnalyticsCounter2Id(const QString& value)
{
	m_data.googleAnalyticsCounter2Id = value;

	emit googleAnalyticsCounter2IdChanged(m_data.googleAnalyticsCounter2Id);
}

bool CrawlerOptions::searchGoogleAnalyticsCounter3() const noexcept
{
	return m_data.searchGoogleAnalyticsCounter3;
}

void CrawlerOptions::setSearchGoogleAnalyticsCounter3(bool value)
{
	m_data.searchGoogleAnalyticsCounter3 = value;

	emit searchGoogleAnalyticsCounter3Changed(m_data.searchGoogleAnalyticsCounter3);
}

const QString& CrawlerOptions::googleAnalyticsCounter3Id() const noexcept
{
	return m_data.googleAnalyticsCounter3Id;
}

void CrawlerOptions::setGoogleAnalyticsCounter3Id(const QString& value)
{
	m_data.googleAnalyticsCounter3Id = value;

	emit googleAnalyticsCounter3IdChanged(m_data.googleAnalyticsCounter3Id);
}

bool CrawlerOptions::searchGoogleAnalyticsCounter4() const noexcept
{
	return m_data.searchGoogleAnalyticsCounter4;
}

void CrawlerOptions::setSearchGoogleAnalyticsCounter4(bool value)
{
	m_data.searchGoogleAnalyticsCounter4 = value;

	emit searchGoogleAnalyticsCounter4Changed(m_data.searchGoogleAnalyticsCounter4);
}

const QString& CrawlerOptions::googleAnalyticsCounter4Id() const noexcept
{
	return m_data.googleAnalyticsCounter4Id;
}

void CrawlerOptions::setGoogleAnalyticsCounter4Id(const QString& value)
{
	m_data.googleAnalyticsCounter4Id = value;

	emit googleAnalyticsCounter4IdChanged(m_data.googleAnalyticsCounter4Id);
}

bool CrawlerOptions::searchGoogleAnalyticsCounter5() const noexcept
{
	return m_data.searchGoogleAnalyticsCounter5;
}

void CrawlerOptions::setSearchGoogleAnalyticsCounter5(bool value)
{
	m_data.searchGoogleAnalyticsCounter5 = value;

	emit searchGoogleAnalyticsCounter5Changed(m_data.searchGoogleAnalyticsCounter5);
}

const QString& CrawlerOptions::googleAnalyticsCounter5Id() const noexcept
{
	return m_data.googleAnalyticsCounter5Id;
}


void CrawlerOptions::setGoogleAnalyticsCounter5Id(const QString& value)
{
	m_data.googleAnalyticsCounter5Id = value;

	emit googleAnalyticsCounter5IdChanged(m_data.googleAnalyticsCounter5Id);
}

const QString& CrawlerOptions::extractorName1() const noexcept
{
    return m_data.extractorNames[0];
}

const QString& CrawlerOptions::extractorName2() const noexcept
{
    return m_data.extractorNames[1];
}

const QString& CrawlerOptions::extractorName3() const noexcept
{
    return m_data.extractorNames[2];
}

const QString& CrawlerOptions::extractorName4() const noexcept
{
    return m_data.extractorNames[3];
}

const QString& CrawlerOptions::extractorName5() const noexcept
{
    return m_data.extractorNames[4];
}

const QString& CrawlerOptions::extractorName6() const noexcept
{
    return m_data.extractorNames[5];
}

const QString& CrawlerOptions::extractorName7() const noexcept
{
    return m_data.extractorNames[6];
}

const QString& CrawlerOptions::extractorName8() const noexcept
{
    return m_data.extractorNames[7];
}

const QString& CrawlerOptions::extractorName9() const noexcept
{
    return m_data.extractorNames[8];
}

const QString& CrawlerOptions::extractorName10() const noexcept
{
    return m_data.extractorNames[9];
}

void CrawlerOptions::setExtractorName1(const QString& value)
{
    m_data.extractorNames[0] = value;
    emit extractorName1Changed(value);
}

void CrawlerOptions::setExtractorName2(const QString& value)
{
    m_data.extractorNames[1] = value;
    emit extractorName2Changed(value);
}

void CrawlerOptions::setExtractorName3(const QString& value)
{
    m_data.extractorNames[2] = value;
    emit extractorName3Changed(value);
}

void CrawlerOptions::setExtractorName4(const QString& value)
{
    m_data.extractorNames[3] = value;
    emit extractorName4Changed(value);
}

void CrawlerOptions::setExtractorName5(const QString& value)
{
    m_data.extractorNames[4] = value;
    emit extractorName5Changed(value);
}

void CrawlerOptions::setExtractorName6(const QString& value)
{
    m_data.extractorNames[5] = value;
    emit extractorName6Changed(value);
}

void CrawlerOptions::setExtractorName7(const QString& value)
{
    m_data.extractorNames[6] = value;
    emit extractorName7Changed(value);
}

void CrawlerOptions::setExtractorName8(const QString& value)
{
    m_data.extractorNames[7] = value;
    emit extractorName8Changed(value);
}

void CrawlerOptions::setExtractorName9(const QString& value)
{
    m_data.extractorNames[8] = value;
    emit extractorName9Changed(value);
}

void CrawlerOptions::setExtractorName10(const QString& value)
{
    m_data.extractorNames[9] = value;
    emit extractorName10Changed(value);
}

int CrawlerOptions::extractorRuleType1() const noexcept
{
    return m_data.extractorRuleTypes[0];
}

int CrawlerOptions::extractorRuleType2() const noexcept
{
    return m_data.extractorRuleTypes[1];
}

int CrawlerOptions::extractorRuleType3() const noexcept
{
    return m_data.extractorRuleTypes[2];
}

int CrawlerOptions::extractorRuleType4() const noexcept
{
    return m_data.extractorRuleTypes[3];
}

int CrawlerOptions::extractorRuleType5() const noexcept
{
    return m_data.extractorRuleTypes[4];
}

int CrawlerOptions::extractorRuleType6() const noexcept
{
    return m_data.extractorRuleTypes[5];
}

int CrawlerOptions::extractorRuleType7() const noexcept
{
    return m_data.extractorRuleTypes[6];
}

int CrawlerOptions::extractorRuleType8() const noexcept
{
    return m_data.extractorRuleTypes[7];
}

int CrawlerOptions::extractorRuleType9() const noexcept
{
    return m_data.extractorRuleTypes[8];
}

int CrawlerOptions::extractorRuleType10() const noexcept
{
    return m_data.extractorRuleTypes[9];
}

void CrawlerOptions::setExtractorRuleType1(int value) noexcept
{
    m_data.extractorRuleTypes[0] = value;
    emit extractorRuleType1Changed(value);
}

void CrawlerOptions::setExtractorRuleType2(int value) noexcept
{
    m_data.extractorRuleTypes[1] = value;
    emit extractorRuleType2Changed(value);
}

void CrawlerOptions::setExtractorRuleType3(int value) noexcept
{
    m_data.extractorRuleTypes[2] = value;
    emit extractorRuleType3Changed(value);
}

void CrawlerOptions::setExtractorRuleType4(int value) noexcept
{
    m_data.extractorRuleTypes[3] = value;
    emit extractorRuleType4Changed(value);
}

void CrawlerOptions::setExtractorRuleType5(int value) noexcept
{
    m_data.extractorRuleTypes[4] = value;
    emit extractorRuleType5Changed(value);
}

void CrawlerOptions::setExtractorRuleType6(int value) noexcept
{
    m_data.extractorRuleTypes[5] = value;
    emit extractorRuleType6Changed(value);
}

void CrawlerOptions::setExtractorRuleType7(int value) noexcept
{
    m_data.extractorRuleTypes[6] = value;
    emit extractorRuleType7Changed(value);
}

void CrawlerOptions::setExtractorRuleType8(int value) noexcept
{
    m_data.extractorRuleTypes[7] = value;
    emit extractorRuleType8Changed(value);
}

void CrawlerOptions::setExtractorRuleType9(int value) noexcept
{
    m_data.extractorRuleTypes[8] = value;
    emit extractorRuleType9Changed(value);
}

void CrawlerOptions::setExtractorRuleType10(int value) noexcept
{
    m_data.extractorRuleTypes[9] = value;
    emit extractorRuleType10Changed(value);
}

const QString& CrawlerOptions::extractorRule1() const noexcept
{
    return m_data.extractorRules[0];
}

const QString& CrawlerOptions::extractorRule2() const noexcept
{
    return m_data.extractorRules[1];
}

const QString& CrawlerOptions::extractorRule3() const noexcept
{
    return m_data.extractorRules[2];
}

const QString& CrawlerOptions::extractorRule4() const noexcept
{
    return m_data.extractorRules[3];
}

const QString& CrawlerOptions::extractorRule5() const noexcept
{
    return m_data.extractorRules[4];
}

const QString& CrawlerOptions::extractorRule6() const noexcept
{
    return m_data.extractorRules[5];
}

const QString& CrawlerOptions::extractorRule7() const noexcept
{
    return m_data.extractorRules[6];
}

const QString& CrawlerOptions::extractorRule8() const noexcept
{
    return m_data.extractorRules[7];
}

const QString& CrawlerOptions::extractorRule9() const noexcept
{
    return m_data.extractorRules[8];
}

const QString& CrawlerOptions::extractorRule10() const noexcept
{
    return m_data.extractorRules[9];
}

void CrawlerOptions::setExtractorRule1(const QString& value)
{
    m_data.extractorRules[0] = value;
    emit extractorRule1Changed(value);
}

void CrawlerOptions::setExtractorRule2(const QString& value)
{
    m_data.extractorRules[1] = value;
    emit extractorRule2Changed(value);
}

void CrawlerOptions::setExtractorRule3(const QString& value)
{
    m_data.extractorRules[2] = value;
    emit extractorRule3Changed(value);
}

void CrawlerOptions::setExtractorRule4(const QString& value)
{
    m_data.extractorRules[4] = value;
    emit extractorRule4Changed(value);
}

void CrawlerOptions::setExtractorRule5(const QString& value)
{
    m_data.extractorRules[4] = value;
    emit extractorRule5Changed(value);
}

void CrawlerOptions::setExtractorRule6(const QString& value)
{
    m_data.extractorRules[5] = value;
    emit extractorRule6Changed(value);
}

void CrawlerOptions::setExtractorRule7(const QString& value)
{
    m_data.extractorRules[6] = value;
    emit extractorRule7Changed(value);
}

void CrawlerOptions::setExtractorRule8(const QString& value)
{
    m_data.extractorRules[7] = value;
    emit extractorRule8Changed(value);
}

void CrawlerOptions::setExtractorRule9(const QString& value)
{
    m_data.extractorRules[8] = value;
    emit extractorRule9Changed(value);
}

void CrawlerOptions::setExtractorRule10(const QString& value)
{
    m_data.extractorRules[9] = value;
    emit extractorRule10Changed(value);
}

int CrawlerOptions::extractionType1() const noexcept
{
    return m_data.extractionTypes[0];
}

int CrawlerOptions::extractionType2() const noexcept
{
    return m_data.extractionTypes[1];
}

int CrawlerOptions::extractionType3() const noexcept
{
    return m_data.extractionTypes[2];
}

int CrawlerOptions::extractionType4() const noexcept
{
    return m_data.extractionTypes[3];
}

int CrawlerOptions::extractionType5() const noexcept
{
    return m_data.extractionTypes[4];
}

int CrawlerOptions::extractionType6() const noexcept
{
    return m_data.extractionTypes[5];
}

int CrawlerOptions::extractionType7() const noexcept
{
    return m_data.extractionTypes[6];
}

int CrawlerOptions::extractionType8() const noexcept
{
    return m_data.extractionTypes[7];
}

int CrawlerOptions::extractionType9() const noexcept
{
    return m_data.extractionTypes[8];
}

int CrawlerOptions::extractionType10() const noexcept
{
    return m_data.extractionTypes[9];
}

void CrawlerOptions::setExtractionType1(int value) noexcept
{
    m_data.extractionTypes[0] = value;
    emit extractionType1Changed(value);
}

void CrawlerOptions::setExtractionType2(int value) noexcept
{
    m_data.extractionTypes[1] = value;
    emit extractionType2Changed(value);
}

void CrawlerOptions::setExtractionType3(int value) noexcept
{
    m_data.extractionTypes[2] = value;
    emit extractionType3Changed(value);
}

void CrawlerOptions::setExtractionType4(int value) noexcept
{
    m_data.extractionTypes[3] = value;
    emit extractionType4Changed(value);
}

void CrawlerOptions::setExtractionType5(int value) noexcept
{
    m_data.extractionTypes[4] = value;
    emit extractionType5Changed(value);
}

void CrawlerOptions::setExtractionType6(int value) noexcept
{
    m_data.extractionTypes[5] = value;
    emit extractionType6Changed(value);
}

void CrawlerOptions::setExtractionType7(int value) noexcept
{
    m_data.extractionTypes[6] = value;
    emit extractionType7Changed(value);
}

void CrawlerOptions::setExtractionType8(int value) noexcept
{
    m_data.extractionTypes[7] = value;
    emit extractionType8Changed(value);
}

void CrawlerOptions::setExtractionType9(int value) noexcept
{
    m_data.extractionTypes[8] = value;
    emit extractionType9Changed(value);
}

void CrawlerOptions::setExtractionType10(int value) noexcept
{
    m_data.extractionTypes[9] = value;
    emit extractionType10Changed(value);
}

const CrawlerOptionsData& CrawlerOptions::data() const noexcept
{
	return m_data;
}

void CrawlerOptions::setData(const CrawlerOptionsData& data) noexcept
{
	m_data = data;
	emit dataChanged(m_data);
}

void CrawlerOptions::setData(CrawlerOptionsData&& data) noexcept
{
	m_data = std::move(data);
	emit dataChanged(m_data);
}

}