#pragma once

namespace spiderpp
{

enum class ExtractionType
{
    TypeExtractInnerHtml,
    TypeExtractHtmlElement,
    TypeExtractText,
    TypeExtractFunctionValue
};

}

Q_DECLARE_METATYPE(spiderpp::ExtractionType);