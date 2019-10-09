#include "url_parser.h"
#include "url.h"
#include "load_result.h"
#include "myhtml_parser.h"
#include "parsed_page.h"
#include "page_parser_helpers.h"

namespace spiderpp
{

UrlParser::UrlParser()
	: m_htmlParser(Common::make_counted<MyHtmlParser>())
	, m_regExp("[\\n\\t]+")
{
}

UrlParser::UrlList UrlParser::urlList(const LoadResult& hop)
{
	UrlList result;

	m_htmlParser->parseHtmlPage(hop.body(), hop.responseHeaders());

	const std::vector<QString> contentTypeValues = hop.responseHeaders().valueOf("content-type");
	const QString& contentType = contentTypeValues.empty() ? QString() : contentTypeValues.front();

	if (resourceType(contentType) != ResourceType::ResourceHtml)
	{
		return result;
	}

	const Url baseUrl = parseBaseTagValue();

	if (!baseUrl.isValid())
	{
		INFOLOG << hop.url().toDisplayString() << "not found base tag or it has an invalid value";
	}

	result.hreflangs = m_htmlParser->hreflangs();
	result.dofollowAhrefs = m_htmlParser->dofollowAhrefs();
	result.nofollowAhrefs = m_htmlParser->nofollowAhrefs();

	const auto resolveUrl = [&baseUrl](const Url& url)
	{
		return PageParserHelpers::resolveUrl(baseUrl, url);
	};

	std::for_each(result.dofollowAhrefs.begin(), result.dofollowAhrefs.end(), resolveUrl);
	std::for_each(result.nofollowAhrefs.begin(), result.nofollowAhrefs.end(), resolveUrl);
	std::for_each(result.hreflangs.begin(), result.hreflangs.end(), resolveUrl);

	return result;
}

Url UrlParser::parseBaseTagValue() const
{
	std::vector<IHtmlNodeCountedPtr> baseTagNodes = m_htmlParser->matchNodesInDepth(IHtmlNode::TagIdBase);
	IHtmlNodeCountedPtr baseTagNode = baseTagNodes.empty() ? nullptr : baseTagNodes[0];

	if (!baseTagNode || !baseTagNode->hasAttribute("href"))
	{
		return Url();
	}

	const Url url = baseTagNode->attribute("href").remove(m_regExp);

	if (url.isValid())
	{
		return url;
	}

	return Url();
}

ResourceType UrlParser::resourceType(const QString& contentType) const
{
	if (contentType.contains("javascript"))
	{
		return ResourceType::ResourceJavaScript;
	}

	if (contentType.startsWith("image/"))
	{
		return ResourceType::ResourceImage;
	}

	if (contentType == "text/css")
	{
		return ResourceType::ResourceStyleSheet;
	}

	if (PageParserHelpers::isHtmlOrPlainContentType(contentType))
	{
		return ResourceType::ResourceHtml;
	}

	WARNLOG << "Unknown resource type:" << contentType;

	return ResourceType::ResourceOther;
}

}
