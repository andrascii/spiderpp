#pragma once

#include "gumbo.h"
#include "ihtml_parser.h"
#include "gumbo_html_node.h"

namespace spiderpp
{

enum class ResourceSource;

class GumboHtmlParser final : public IHtmlParser
{
public:
	GumboHtmlParser();
	GumboHtmlParser(const GumboOptions* options, const QByteArray& htmlPage);
	virtual ~GumboHtmlParser();

	virtual QByteArray currentPageEncoding() const override;
	virtual QByteArray htmlPageContent() const override;
	virtual bool isEmpty() const override;
	virtual void parseHtmlPage(const QByteArray& htmlPage, const ResponseHeaders& headers) override;
	virtual IHtmlNodeCountedPtr firstMatchNode(IHtmlNode::TagId tagId) const override;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodes(IHtmlNode::TagId tagId) const override;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodesInDepth(IHtmlNode::TagId tagId) const override;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodesInDepth(const std::function<bool(const IHtmlNode&)>& predicate) const override;
	virtual IHtmlNodeCountedPtr findNodeWithAttributeValue(IHtmlNode::TagId tagId, std::pair<const char*, const char*> expectedAttributes) const override;
	virtual IHtmlNodeCountedPtr findNodeWithAttributesValues(IHtmlNode::TagId tagId, const std::map<const char*, const char*>& expectedAttributes) const override;
	virtual IHtmlNodeCountedPtr fromData(void* data) const override;
	virtual IHtmlAttributeCountedPtr attributeFromData(void* data) const override;
	virtual IHtmlNodeCountedPtr root() const override;
	virtual IHtmlNodeCountedPtr emptyNode() const override;
	virtual IHtmlAttributeCountedPtr emptyAttribute() const override;

	// TODO: this method must not be in this interface
	// we need to implement them using matchNodesInDepth method
	virtual std::vector<Url> dofollowAhrefs() const override;
	virtual std::vector<Url> nofollowAhrefs() const override;
	virtual std::vector<Url> hreflangs() const override;

private:
	std::vector<Url> getLinkRelUrl(const GumboNode* node, const char* relValue, const char* requiredAttribute = nullptr, bool getFirstValueOnly = true) const;
	QByteArray encodingFromPage() const;
	QByteArray decodeHtmlPage(const ResponseHeaders& headers);

	std::vector<Url> ahrefs(AhrefsType type) const;

private:
	const GumboOptions* m_gumboOptions;
	GumboOutput* m_gumboOutput;
	QByteArray m_htmlPage;
	GumboHtmlNode m_rootNode;
	QByteArray m_currentPageEncoding;
	QRegularExpression m_regExp;
};

}