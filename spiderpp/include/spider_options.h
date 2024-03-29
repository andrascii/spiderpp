#pragma once

#include "ispider_options.h"
#include "user_agent_type.h"

namespace spiderpp
{

constexpr size_t c_extractorCount = 10;

struct SpiderOptionsData final
{
	Url startCrawlingPage;
	int limitSearchTotal = int();
	int limitTimeout = int();
	int maxRedirectsToFollow = int();
	int maxParallelConnections = int();
	int maxLinksCountOnPage = int();
	int maxTitleLength = int();
	int minTitleLength = int();
	int maxDescriptionLength = int();
	int minDescriptionLength = int();
	int maxH1LengthChars = int();
	int maxH2LengthChars = int();
	int maxImageAltTextChars = int();
	int maxImageSizeKb = int();
	int maxPageSizeKb = int();
	bool useProxy = bool();
	QString proxyHostName;
	int proxyPort = int();
	QString proxyUser;
	QString proxyPassword;
	bool checkExternalLinks = bool();
	bool followInternalNofollow = bool();
	bool followExternalNofollow = bool();
	bool checkCanonicals = bool();
	bool checkSubdomains = bool();
	bool crawlOutsideOfStartFolder = bool();
	bool crawlMetaHrefLangLinks = bool();
	bool followRobotsTxtRules = bool();
	cpprobotparser::WellKnownUserAgent userAgentToFollow = cpprobotparser::WellKnownUserAgent::AllRobots;
	ParserTypeFlags parserTypeFlags;
	int pauseRangeFrom = -1;
	int pauseRangeTo = -1;
	bool pauseRangeEnabled = false;
	QByteArray userAgent;
	QString excludeUrlRegExps;
};

class SpiderOptions : public QObject, public ISpiderOptions
{
	Q_OBJECT

	Q_PROPERTY(Url startCrawlingPage READ startCrawlingPage WRITE setStartCrawlingPage NOTIFY startCrawlingPageChanged);
	Q_PROPERTY(int limitSearchTotal READ limitSearchTotal WRITE setLimitSearchTotal NOTIFY limitSearchTotalChanged);
	Q_PROPERTY(int limitTimeout READ limitTimeout WRITE setLimitTimeout NOTIFY limitTimeoutChanged);
	Q_PROPERTY(int maxRedirectsToFollow READ maxRedirectsToFollow WRITE setMaxRedirectsToFollow NOTIFY maxRedirectsToFollowChanged);
	Q_PROPERTY(int maxParallelConnections READ maxParallelConnections WRITE setMaxParallelConnections NOTIFY maxParallelConnectionsChanged);
	Q_PROPERTY(int maxLinksCountOnPage READ maxLinksCountOnPage WRITE setMaxLinksCountOnPage NOTIFY maxLinksCountOnPageChanged);
	Q_PROPERTY(int maxTitleLength READ maxTitleLength WRITE setMaxTitleLength NOTIFY maxTitleLengthChanged);
	Q_PROPERTY(int minTitleLength READ minTitleLength WRITE setMinTitleLength NOTIFY minTitleLengthChanged);
	Q_PROPERTY(int maxDescriptionLength READ maxDescriptionLength WRITE setMaxDescriptionLength NOTIFY maxDescriptionLengthChanged);
	Q_PROPERTY(int minDescriptionLength READ minDescriptionLength WRITE setMinDescriptionLength NOTIFY minDescriptionLengthChanged);
	Q_PROPERTY(int maxH1LengthChars READ maxH1LengthChars WRITE setMaxH1LengthChars NOTIFY maxH1LengthCharsChanged);
	Q_PROPERTY(int maxH2LengthChars READ maxH2LengthChars WRITE setMaxH2LengthChars NOTIFY maxH2LengthCharsChanged);
	Q_PROPERTY(int maxImageAltTextChars READ maxImageAltTextChars WRITE setMaxImageAltTextChars NOTIFY maxImageAltTextCharsChanged);
	Q_PROPERTY(int maxImageSizeKb READ maxImageSizeKb WRITE setMaxImageSizeKb NOTIFY maxImageSizeKbChanged);
	Q_PROPERTY(int maxPageSizeKb READ maxPageSizeKb WRITE setMaxPageSizeKb NOTIFY maxPageSizeKbChanged);
	Q_PROPERTY(bool useProxy READ useProxy WRITE setUseProxy NOTIFY useProxyChanged);
	Q_PROPERTY(QString proxyHostName READ proxyHostName WRITE setProxyHostName NOTIFY proxyHostNameChanged);
	Q_PROPERTY(int proxyPort READ proxyPort WRITE setProxyPort NOTIFY proxyPortChanged);
	Q_PROPERTY(QString proxyUser READ proxyUser WRITE setProxyUser NOTIFY proxyUserChanged);
	Q_PROPERTY(QString proxyPassword READ proxyPassword WRITE setProxyPassword NOTIFY proxyPasswordChanged);
	Q_PROPERTY(bool checkExternalLinks READ checkExternalLinks WRITE setCheckExternalLinks NOTIFY checkExternalLinksChanged);
	Q_PROPERTY(bool followInternalNofollow READ followInternalNofollow WRITE setFollowInternalNofollow NOTIFY followInternalNofollowChanged);
	Q_PROPERTY(bool followExternalNofollow READ followExternalNofollow WRITE setFollowExternalNofollow NOTIFY followExternalNofollowChanged);
	Q_PROPERTY(bool checkCanonicals READ checkCanonicals WRITE setCheckCanonicals NOTIFY checkCanonicalsChanged);
	Q_PROPERTY(bool checkSubdomains READ checkSubdomains WRITE setCheckSubdomains NOTIFY checkSubdomainsChanged);
	Q_PROPERTY(bool crawlOutsideOfStartFolder READ crawlOutsideOfStartFolder WRITE setCrawlOutsideOfStartFolder NOTIFY crawlOutsideOfStartFolderChanged);
	Q_PROPERTY(bool crawlMetaHrefLangLinks READ crawlMetaHrefLangLinks WRITE setCrawlMetaHrefLangLinks NOTIFY crawlMetaHrefLangLinksChanged);
	Q_PROPERTY(bool followRobotsTxtRules READ followRobotsTxtRules WRITE setFollowRobotsTxtRules NOTIFY followRobotsTxtRulesChanged);
	Q_PROPERTY(cpprobotparser::WellKnownUserAgent userAgentToFollow READ userAgentToFollow WRITE setUserAgentToFollow NOTIFY userAgentToFollowChanged);
	Q_PROPERTY(ParserTypeFlags parserTypeFlags READ parserTypeFlags WRITE setParserTypeFlags NOTIFY parserTypeFlagsChanged);
	Q_PROPERTY(int pauseRangeFrom READ pauseRangeFrom WRITE setPauseRangeFrom NOTIFY pauseRangeFromChanged);
	Q_PROPERTY(int pauseRangeTo READ pauseRangeTo WRITE setPauseRangeTo NOTIFY pauseRangeToChanged);
	Q_PROPERTY(bool pauseRangeEnabled READ pauseRangeEnabled WRITE setPauseRangeEnabled NOTIFY pauseRangeEnabledChanged);
	Q_PROPERTY(QByteArray userAgent READ userAgent WRITE setUserAgent NOTIFY userAgentChanged);
	Q_PROPERTY(QString excludeUrlRegExps READ excludeUrlRegExps WRITE setExcludeUrlRegExps NOTIFY excludeUrlRegExpsChanged);

public:
	SpiderOptions(QObject* parent = nullptr);

	virtual QObject* qobject() const noexcept override;

	virtual const SpiderOptionsData& data() const noexcept override;
	virtual void setData(const SpiderOptionsData& data) noexcept override;
	virtual void setData(SpiderOptionsData&& data) noexcept override;
	Q_SIGNAL virtual void dataChanged(const SpiderOptionsData& data) const override;

	virtual Url startCrawlingPage() const noexcept override;
	Q_SLOT virtual void setStartCrawlingPage(const Url& url) override;
	Q_SIGNAL virtual void startCrawlingPageChanged(const Url& url) const override;

	virtual int limitSearchTotal() const noexcept override;
	Q_SLOT virtual void setLimitSearchTotal(int value) noexcept override;
	Q_SIGNAL virtual void limitSearchTotalChanged(int value) const override;

	virtual int limitTimeout() const noexcept override;
	Q_SLOT virtual void setLimitTimeout(int value) noexcept override;
	Q_SIGNAL virtual int limitTimeoutChanged(int value) const override;

	virtual int maxRedirectsToFollow() const noexcept override;
	Q_SLOT virtual void setMaxRedirectsToFollow(int value) noexcept override;
	Q_SIGNAL virtual void maxRedirectsToFollowChanged(int value) const override;

	virtual int maxParallelConnections() const noexcept override;
	Q_SLOT virtual void setMaxParallelConnections(int value) noexcept override;
	Q_SIGNAL virtual void maxParallelConnectionsChanged(int value) const override;

	virtual int maxLinksCountOnPage() const noexcept override;
	Q_SLOT virtual void setMaxLinksCountOnPage(int value) noexcept override;
	Q_SIGNAL virtual void maxLinksCountOnPageChanged(int value) const override;

	virtual int maxTitleLength() const noexcept override;
	Q_SLOT virtual void setMaxTitleLength(int value) noexcept override;
	Q_SIGNAL virtual void maxTitleLengthChanged(int value) const override;

	virtual int minTitleLength() const noexcept override;
	Q_SLOT virtual void setMinTitleLength(int value) noexcept override;
	Q_SIGNAL virtual void minTitleLengthChanged(int value) const override;

	virtual int maxDescriptionLength() const noexcept override;
	Q_SLOT virtual void setMaxDescriptionLength(int value) noexcept override;
	Q_SIGNAL virtual void maxDescriptionLengthChanged(int value) const override;

	virtual int minDescriptionLength() const noexcept override;
	Q_SLOT virtual void setMinDescriptionLength(int value) noexcept override;
	Q_SIGNAL virtual void minDescriptionLengthChanged(int value) const override;

	virtual int maxH1LengthChars() const noexcept override;
	Q_SLOT virtual void setMaxH1LengthChars(int value) noexcept override;
	Q_SIGNAL virtual void maxH1LengthCharsChanged(int value) const override;

	virtual int maxH2LengthChars() const noexcept override;
	Q_SLOT virtual void setMaxH2LengthChars(int value) noexcept override;
	Q_SIGNAL virtual void maxH2LengthCharsChanged(int value) const override;

	virtual int maxImageAltTextChars() const noexcept override;
	Q_SLOT virtual void setMaxImageAltTextChars(int value) noexcept override;
	Q_SIGNAL virtual void maxImageAltTextCharsChanged(int value) const override;

	virtual int maxImageSizeKb() const noexcept override;
	Q_SLOT virtual void setMaxImageSizeKb(int value) noexcept override;
	Q_SIGNAL virtual void maxImageSizeKbChanged(int value) const override;

	virtual int maxPageSizeKb() const noexcept override;
	Q_SLOT virtual void setMaxPageSizeKb(int value) noexcept override;
	Q_SIGNAL virtual void maxPageSizeKbChanged(int value) const override;

	virtual bool useProxy() const noexcept override;
	Q_SLOT virtual void setUseProxy(bool value) noexcept override;
	Q_SIGNAL virtual void useProxyChanged(int value) const override;

	virtual QString proxyHostName() const noexcept override;
	Q_SLOT virtual void setProxyHostName(const QString& value) override;
	Q_SIGNAL virtual void proxyHostNameChanged(const QString& value) const override;

	virtual int proxyPort() const noexcept override;
	Q_SLOT virtual void setProxyPort(int value) noexcept override;
	Q_SIGNAL virtual void proxyPortChanged(int value) const override;

	virtual QString proxyUser() const noexcept override;
	Q_SLOT virtual void setProxyUser(const QString& value) override;
	Q_SIGNAL virtual void proxyUserChanged(const QString& value) const override;

	virtual QString proxyPassword() const noexcept override;
	Q_SLOT virtual void setProxyPassword(const QString& value) override;
	Q_SIGNAL virtual void proxyPasswordChanged(const QString& value) const override;

	virtual bool checkExternalLinks() const noexcept override;
	Q_SLOT virtual void setCheckExternalLinks(bool value) noexcept override;
	Q_SIGNAL virtual void checkExternalLinksChanged(bool value) const override;

	virtual bool followInternalNofollow() const noexcept override;
	Q_SLOT virtual void setFollowInternalNofollow(bool value) noexcept override;
	Q_SIGNAL virtual void followInternalNofollowChanged(bool value) const override;

	virtual bool followExternalNofollow() const noexcept override;
	Q_SLOT virtual void setFollowExternalNofollow(bool value) noexcept override;
	Q_SIGNAL virtual void followExternalNofollowChanged(bool value) const override;

	virtual bool checkCanonicals() const noexcept override;
	Q_SLOT virtual void setCheckCanonicals(bool value) noexcept override;
	Q_SIGNAL virtual void checkCanonicalsChanged(bool value) const override;

	virtual bool checkSubdomains() const noexcept override;
	Q_SLOT virtual void setCheckSubdomains(bool value) noexcept override;
	Q_SIGNAL virtual void checkSubdomainsChanged(bool value) const override;

	virtual bool crawlOutsideOfStartFolder() const noexcept override;
	Q_SLOT virtual void setCrawlOutsideOfStartFolder(bool value) noexcept override;
	Q_SIGNAL virtual void crawlOutsideOfStartFolderChanged(bool value) const override;

	virtual bool crawlMetaHrefLangLinks() const noexcept override;
	Q_SLOT virtual void setCrawlMetaHrefLangLinks(bool value) noexcept override;
	Q_SIGNAL virtual void crawlMetaHrefLangLinksChanged(bool value) const override;

	virtual bool followRobotsTxtRules() const noexcept override;
	Q_SLOT virtual void setFollowRobotsTxtRules(bool value) noexcept override;
	Q_SIGNAL virtual void followRobotsTxtRulesChanged(bool value) const override;

	virtual cpprobotparser::WellKnownUserAgent userAgentToFollow() const noexcept override;
	Q_SLOT virtual void setUserAgentToFollow(cpprobotparser::WellKnownUserAgent value) noexcept override;
	Q_SIGNAL virtual void userAgentToFollowChanged(cpprobotparser::WellKnownUserAgent value) const override;

	virtual ParserTypeFlags parserTypeFlags() const noexcept override;
	Q_SLOT virtual void setParserTypeFlags(ParserTypeFlags value) noexcept override;
	Q_SIGNAL virtual void parserTypeFlagsChanged(ParserTypeFlags value) const override;

	virtual int pauseRangeFrom() const noexcept override;
	Q_SLOT virtual void setPauseRangeFrom(int value) noexcept override;
	Q_SIGNAL virtual void pauseRangeFromChanged(int value) const override;

	virtual int pauseRangeTo() const noexcept override;
	Q_SLOT virtual void setPauseRangeTo(int value) noexcept override;
	Q_SIGNAL virtual void pauseRangeToChanged(int value) const override;

	virtual bool pauseRangeEnabled() const noexcept override;
	Q_SLOT virtual void setPauseRangeEnabled(bool value) noexcept override;
	Q_SIGNAL virtual void pauseRangeEnabledChanged(bool value) const override;

	virtual QByteArray userAgent() const noexcept override;
	Q_SLOT virtual void setUserAgent(const QByteArray& value) override;
	Q_SIGNAL virtual void userAgentChanged(const QByteArray& value) const override;

	virtual QString excludeUrlRegExps() const noexcept override;
	Q_SLOT virtual void setExcludeUrlRegExps(const QString& value) override;
	Q_SIGNAL virtual void excludeUrlRegExpsChanged(const QString& value) const override;

private:
	SpiderOptionsData m_data;
};

}

Q_DECLARE_METATYPE(spiderpp::SpiderOptionsData)
