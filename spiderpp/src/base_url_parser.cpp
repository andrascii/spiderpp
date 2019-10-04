#include "base_url_parser.h"
#include "ihtml_parser.h"

namespace CrawlerEngine
{

BaseUrlParser::BaseUrlParser(IHtmlParser* htmlParser)
	: m_htmlParser(htmlParser)
	, m_regExp("[\\n\\t]+")
{
}

void BaseUrlParser::parse(const ResponseHeaders& headers, ParsedPagePtr& page)
{
	Q_UNUSED(headers);

	std::vector<IHtmlNodeCountedPtr> baseTagNodes = m_htmlParser->matchNodesInDepth(IHtmlNode::TagIdBase);
	IHtmlNodeCountedPtr baseTagNode = baseTagNodes.empty() ? nullptr : baseTagNodes[0];

	if (baseTagNode && baseTagNode->hasAttribute("href"))
	{
		const Url url = baseTagNode->attribute("href").remove(m_regExp);

		if (url.isValid())
		{
			page->baseUrl = url;
		}
		else
		{
			WARNLOG << "On page:" << page->url.toDisplayString() << "found base tag but that tag has an invalid url address";

			page->baseUrl = page->url;
		}
	}
	else
	{
		page->baseUrl = page->url;
	}
}

}