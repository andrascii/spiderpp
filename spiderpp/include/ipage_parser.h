#pragma once

#include "gumbo.h"
#include "parsed_page.h"
#include "response_headers.h"

namespace spiderpp
{

class IPageParser
{
public:
	virtual ~IPageParser() = default;
	virtual void parse(const ResponseHeaders& headers, ParsedPagePtr& page) = 0;
};

}
