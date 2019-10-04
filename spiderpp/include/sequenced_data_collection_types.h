#pragma once
#include "parsed_page.h"

namespace spiderpp
{
	
struct LinksToThisResourceChanges
{
	struct Change
	{
		ParsedPageWeakPtr page;
		size_t fromIndex;
	};
	std::vector<Change> changes;
};

}

Q_DECLARE_METATYPE(spiderpp::LinksToThisResourceChanges)
