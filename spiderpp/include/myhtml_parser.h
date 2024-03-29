#pragma once

#include "ihtml_parser.h"
#include "myhtml/api.h"
#include "myhtml_node.h"

namespace spiderpp
{

enum class ResourceSource;

class MyHtmlParser final : public IHtmlParser
{
public:
	MyHtmlParser();
	virtual ~MyHtmlParser();

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
	void initRootNode();

	std::vector<Url> getLinkRelUrl(
		const char* relValue,
		const char* requiredAttribute = nullptr,
		bool getFirstValueOnly = true) const;

	myencoding_t htmlSetEncoding(const ResponseHeaders& headers);
	QByteArray encodingFromPage() const;
	QByteArray encodingString(myencoding_t encoding) const;
	std::vector<Url> ahrefs(AhrefsType type) const;

private:
	myhtml_t* m_myHtml;
	myhtml_tree_t* m_tree;
	MyHtmlNode m_rootNode;
	QByteArray m_htmlPage;
	QRegularExpression m_regExp;
	QRegularExpression m_quotesRegExp;
	QByteArray m_currentPageEncoding;
};

}