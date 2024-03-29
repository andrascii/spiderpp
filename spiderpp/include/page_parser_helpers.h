#pragma once

namespace spiderpp
{

class Url;

class PageParserHelpers
{
public:
	static Url resolveRelativeUrl(const Url& relativeUrl, const Url& baseUrl);
	static void resolveUrlList(const Url& baseUrl, std::vector<Url>& urlList) noexcept;
	static Url resolveUrl(const Url& baseUrl, const Url& url) noexcept;
	static bool checkUrlIdentity(const Url& lhs, const Url& rhs);

	static bool isUrlExternal(const Url& baseUrl, const Url& url, bool allowSubDomains) noexcept;
	static bool isUrlInsideBaseUrlFolder(const Url& baseUrl, const Url& url) noexcept;
	static bool isHtmlOrPlainContentType(const QString& contentType) noexcept;
	static bool isHttpOrHttpsScheme(const Url& url) noexcept;
	static bool isSubdomain(const Url& baseUrl, const Url& url);
};

}