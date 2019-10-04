#pragma once

namespace spiderpp
{

enum class ExtractorRuleType
{
    TypeXPath,
    TypeCssPath,
    TypeRegex
};

}

Q_DECLARE_METATYPE(spiderpp::ExtractorRuleType);