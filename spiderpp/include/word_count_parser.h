#pragma once

#include "ipage_parser.h"

namespace spiderpp
{

class IHtmlParser;

class WordCountParser : public IPageParser
{
public:
	WordCountParser(IHtmlParser* htmlParser);

	virtual void parse(const ResponseHeaders& headers, ParsedPagePtr& pageRaw) noexcept override;

private:
	IHtmlParser* m_htmlParser;
};

}
