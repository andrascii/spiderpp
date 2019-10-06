#include "html_resources_parser.h"
#include "page_parser_helpers.h"
#include "ihtml_parser.h"
#include "myhtml_parser.h"

namespace spiderpp
{

HtmlResourcesParser::HtmlResourcesParser(IHtmlParser* htmlParser, bool parseMetaHrefLangLinks)
	: m_htmlParser(htmlParser)
	, m_parseMetaHrefLangLinks(parseMetaHrefLangLinks)
{
}

void HtmlResourcesParser::parse(const ResponseHeaders& headers, ParsedPagePtr& page)
{
	if (page->resourceType != ResourceType::ResourceHtml)
	{
		CompoundParser::parse(headers, page);
		return;
	}

	std::vector<LinkInfo> linksInfo = m_htmlParser->pageUrlList(true);

	DEBUG_ASSERT(page->baseUrl.isValid());

	for (LinkInfo& linkInfo : linksInfo)
	{
		linkInfo.url = PageParserHelpers::resolveUrl(page->baseUrl, linkInfo.url);

		if (!PageParserHelpers::isHttpOrHttpsScheme(linkInfo.url))
		{
			DEBUG_ASSERT(!"This url leads to a not html resources");
			continue;
		}

		if (linkInfo.resourceSource == ResourceSource::SourceTagLinkAlternateHrefLang && !m_parseMetaHrefLangLinks)
		{
			continue;
		}

		if (linkInfo.resourceSource == ResourceSource::SourceTagLinkRelCanonical)
		{
			page->canonicalUrl = linkInfo.url;
		}

		ResourceOnPage resource{ ResourceType::ResourceHtml, std::move(linkInfo) };

		if (page->allResourcesOnPage.find(resource) == page->allResourcesOnPage.end())
		{
			page->allResourcesOnPage.insert(std::move(resource));
		}
	}

	CompoundParser::parse(headers, page);
}

}
