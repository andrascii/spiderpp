#include "gumbo_html_parser.h"
#include "gumbo_html_node.h"
#include "response_headers.h"
#include "page_parser_helpers.h"
#include "url.h"
#include "ahref_rel_type.h"

namespace
{

template <class UnaryPredicate>
static std::vector<const GumboNode*> findNodesRecursive(const GumboNode* node, UnaryPredicate predicate) noexcept
{
	std::vector<const GumboNode*> result;

	if (predicate(node))
	{
		result.push_back(node);
	}

	if (node->type == GUMBO_NODE_ELEMENT)
	{
		const GumboVector* children = &node->v.element.children;

		for (unsigned int i = 0; i < children->length; ++i)
		{
			const GumboNode* child = static_cast<const GumboNode*>(children->data[i]);
			std::vector<const GumboNode*> childResult = findNodesRecursive(child, predicate);

			result.insert(std::end(result), std::begin(childResult), std::end(childResult));
		}
	}

	return result;
}

template <class UnaryPredicate, class MapFunc>
static auto findNodesAndGetResult(const GumboNode* parentNode, UnaryPredicate predicate, MapFunc mapFunc)
{
	std::vector<const GumboNode*> nodes = findNodesRecursive(parentNode, predicate);
	std::vector<decltype(mapFunc(parentNode))> result;

	for (const GumboNode* node : nodes)
	{
		result.push_back(mapFunc(node));
	}

	return result;
}

bool checkAttribute(const GumboNode* node, const char* attribute, const char* expectedValue) noexcept
{
	const GumboAttribute* attr = gumbo_get_attribute(&node->v.element.attributes, attribute);
	return attr && (strlen(expectedValue) == 0 || QString(attr->value).toLower().trimmed() == expectedValue);
}

}

namespace spiderpp
{

GumboHtmlParser::GumboHtmlParser()
	: m_gumboOptions(&kGumboDefaultOptions)
	, m_gumboOutput(nullptr)
	, m_rootNode(nullptr)
	, m_regExp("[\\n\\t]+")
{
}

GumboHtmlParser::GumboHtmlParser(const GumboOptions* options, const QByteArray& htmlPage)
	: m_gumboOptions(options)
	, m_gumboOutput(gumbo_parse_with_options(options, htmlPage.data(), htmlPage.size()))
	, m_htmlPage(htmlPage)
	, m_rootNode(m_gumboOutput->root)
	, m_regExp("[\\n\\t]+")
{
}

GumboHtmlParser::~GumboHtmlParser()
{
	gumbo_destroy_output(m_gumboOptions, m_gumboOutput);
}

void GumboHtmlParser::parseHtmlPage(const QByteArray& htmlPage, const ResponseHeaders& headers)
{
	if (m_gumboOutput)
	{
		gumbo_destroy_output(m_gumboOptions, m_gumboOutput);
	}

	m_gumboOutput = gumbo_parse_with_options(&kGumboDefaultOptions, htmlPage.data(), htmlPage.size());
	m_htmlPage = htmlPage;
	m_rootNode = GumboHtmlNode(m_gumboOutput->root);

	// we need double parsing because we need firstly decode html page
	// this need in order to we'll be able to extract right data
	m_htmlPage = decodeHtmlPage(headers);

	gumbo_destroy_output(m_gumboOptions, m_gumboOutput);
	m_gumboOutput = gumbo_parse_with_options(&kGumboDefaultOptions, m_htmlPage.data(), m_htmlPage.size());
	m_rootNode = GumboHtmlNode(m_gumboOutput->root);
}

QByteArray GumboHtmlParser::encodingFromPage() const
{
	IHtmlNodeCountedPtr headNode = m_rootNode.firstMatchSubNode(IHtmlNode::TagIdHead);

	if (!headNode)
	{
		INFOLOG << "tag <head> not found";
		return QByteArray();
	}

	const auto metaTags = headNode->matchSubNodes(IHtmlNode::TagIdMeta);

	for (const auto& metaTag : metaTags)
	{
		const bool hasContentAttribute = metaTag->hasAttribute("content");
		const bool hasCharsetAttribute = metaTag->hasAttribute("charset");

		if (!hasContentAttribute && !hasCharsetAttribute)
		{
			continue;
		}

		if (hasCharsetAttribute)
		{
			return metaTag->attribute("charset").toLatin1();
		}

		const QString contentAttribute = metaTag->attribute("content");

		if (metaTag->hasAttribute("http-equiv"))
		{
			const QString attributeValue = metaTag->attribute("http-equiv").toLower();

			if (attributeValue == "content-type")
			{
				const auto charsetFromHtmlPage = contentAttribute.right(
					contentAttribute.size() - contentAttribute.lastIndexOf("=") - 1
				);

				return charsetFromHtmlPage.toLatin1();
			}
		}
	}

	return QByteArray();
}

QByteArray GumboHtmlParser::decodeHtmlPage(const ResponseHeaders& headers)
{
	const std::vector<QString> contentTypeValues = headers.valueOf("content-type");

	if (!contentTypeValues.empty())
	{
		for (const QString& contentTypeValue : contentTypeValues)
		{
			const QByteArray charsetFromHttpResponse = contentTypeValue.right(contentTypeValue.size() - contentTypeValue.lastIndexOf("=") - 1).toLatin1();
			const QTextCodec* codecForCharset = QTextCodec::codecForName(charsetFromHttpResponse);

			if (!codecForCharset)
			{
				continue;
			}

			m_currentPageEncoding = charsetFromHttpResponse;
			m_htmlPage = codecForCharset->toUnicode(m_htmlPage).toStdString().data();

			return m_htmlPage;
		}
	}

	const QByteArray charsetFromHtmlPage = encodingFromPage();

	if (!charsetFromHtmlPage.isEmpty())
	{
		QTextCodec* codecForCharset = QTextCodec::codecForName(charsetFromHtmlPage);

		if (codecForCharset)
		{
			m_currentPageEncoding = charsetFromHtmlPage;
			m_htmlPage = codecForCharset->toUnicode(m_htmlPage.data()).toStdString().data();
		}
		else
		{
			ERRLOG << "Cannot find QTextCodec for page encoding";
			ERRLOG << "charset:" << charsetFromHtmlPage;
		}
	}
	else
	{
		QTextCodec* codecForHtml = QTextCodec::codecForHtml(m_htmlPage);

		if (codecForHtml)
		{
			m_currentPageEncoding = codecForHtml->name();
			m_htmlPage = codecForHtml->toUnicode(m_htmlPage).toStdString().data();
		}
		else
		{
			ERRLOG << "Cannot identify page encoding";
		}
	}

	return m_htmlPage;
}

std::vector<Url> GumboHtmlParser::ahrefs(AhrefsType type) const
{
	const auto predicate = [this, type](const GumboNode* node)
	{
		bool result = node &&
			node->type == GUMBO_NODE_ELEMENT &&
			node->v.element.tag == GUMBO_TAG_A &&
			gumbo_get_attribute(&node->v.element.attributes, "href");

		if (!result)
		{
			return result;
		}

		GumboAttribute* hrefAttribute = gumbo_get_attribute(&node->v.element.attributes, "href");

		QString hrefVal;

		if (hrefAttribute)
		{
			hrefVal = hrefAttribute->value;
		}
		else
		{
			return false;
		}

		const GumboAttribute* relAttribute = gumbo_get_attribute(&node->v.element.attributes, "rel");

		const bool isDofollowAhref = !relAttribute ||
			relAttribute && QString(relAttribute->value).trimmed().remove(m_regExp) != "nofollow";

		if (type == NofollowAhrefs && isDofollowAhref ||
			type == DofollowAhrefs && !isDofollowAhref)
		{
			return false;
		}

		return PageParserHelpers::isHttpOrHttpsScheme(Url(hrefVal));
	};

	const auto resultGetter = [this](const GumboNode* node)
	{
		const GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
		const Url url = QString(href->value).trimmed().remove(m_regExp);

		return url;
	};

	return findNodesAndGetResult(m_gumboOutput->root, predicate, resultGetter);
}

std::vector<Url> GumboHtmlParser::dofollowAhrefs() const
{
	return ahrefs(DofollowAhrefs);
}

std::vector<Url> GumboHtmlParser::nofollowAhrefs() const
{
	return ahrefs(NofollowAhrefs);
}

std::vector<Url> GumboHtmlParser::hreflangs() const
{
	return getLinkRelUrl(m_gumboOutput->root, "alternate", "hreflang", false);
}

std::vector<Url> GumboHtmlParser::getLinkRelUrl(const GumboNode* node, const char* relValue, const char* requiredAttribute, bool getFirstValueOnly) const
{
	const auto cond = [relValue, requiredAttribute](const GumboNode* node)
	{
		bool result = node &&
			node->type == GUMBO_NODE_ELEMENT &&
			node->v.element.tag == GUMBO_TAG_LINK &&
			checkAttribute(node, "rel", relValue) &&
			gumbo_get_attribute(&node->v.element.attributes, "href") &&
			(requiredAttribute == nullptr || gumbo_get_attribute(&node->v.element.attributes, requiredAttribute));
		return result;
	};

	const auto res = [](const GumboNode* node)
	{
		const GumboAttribute* href = gumbo_get_attribute(&node->v.element.attributes, "href");
		return Url(href->value);
	};

	std::vector<Url> result = findNodesAndGetResult(node, cond, res);

	if (getFirstValueOnly && !result.empty())
	{
		return { result[0] };
	}

	return result;
}

IHtmlNodeCountedPtr GumboHtmlParser::firstMatchNode(IHtmlNode::TagId tagId) const
{
	return m_rootNode.firstMatchSubNode(tagId);
}

std::vector<IHtmlNodeCountedPtr> GumboHtmlParser::matchNodes(IHtmlNode::TagId tagId) const
{
	return m_rootNode.matchSubNodes(tagId);
}

std::vector<IHtmlNodeCountedPtr> GumboHtmlParser::matchNodesInDepth(IHtmlNode::TagId tagId) const
{
	return m_rootNode.matchSubNodesInDepth(tagId);
}

std::vector<IHtmlNodeCountedPtr> GumboHtmlParser::matchNodesInDepth(const std::function<bool(const IHtmlNode&)>& predicate) const
{
	return m_rootNode.matchSubNodesInDepth(predicate);
}

IHtmlNodeCountedPtr GumboHtmlParser::findNodeWithAttributeValue(IHtmlNode::TagId tagId, std::pair<const char*, const char*> expectedAttributes) const
{
	return m_rootNode.childNodeByAttributeValue(tagId, expectedAttributes);
}

IHtmlNodeCountedPtr GumboHtmlParser::findNodeWithAttributesValues(IHtmlNode::TagId tagId, const std::map<const char*, const char*>& expectedAttributes) const
{
	return m_rootNode.childNodeByAttributesValues(tagId, expectedAttributes);
}

IHtmlNodeCountedPtr GumboHtmlParser::fromData(void* data) const
{
	Q_UNUSED(data);
	ASSERT(!"Not implemented!");
	return IHtmlNodeCountedPtr();
}

IHtmlAttributeCountedPtr GumboHtmlParser::attributeFromData(void * data) const
{
	Q_UNUSED(data);
	ASSERT(!"Not implemented!");
	return IHtmlAttributeCountedPtr();
}

IHtmlNodeCountedPtr GumboHtmlParser::root() const
{
	ASSERT(!"Not implemented!");
	return IHtmlNodeCountedPtr();
}

IHtmlNodeCountedPtr GumboHtmlParser::emptyNode() const
{
	ASSERT(!"Not implemented!");
	return IHtmlNodeCountedPtr();
}

IHtmlAttributeCountedPtr GumboHtmlParser::emptyAttribute() const
{
	ASSERT(!"Not implemented!");
	return IHtmlAttributeCountedPtr();
}

QByteArray GumboHtmlParser::htmlPageContent() const
{
	return m_htmlPage;
}

bool GumboHtmlParser::isEmpty() const
{
	return m_htmlPage.isEmpty();
}

QByteArray GumboHtmlParser::currentPageEncoding() const
{
	return m_currentPageEncoding;
}

}
