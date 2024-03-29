#pragma once

#include "ihtml_node.h"

namespace spiderpp
{

class Url;
class ResponseHeaders;
struct LinkInfo;

class IHtmlParser
{
public:
	virtual ~IHtmlParser() = default;

	virtual QByteArray currentPageEncoding() const = 0;
	virtual QByteArray htmlPageContent() const = 0;
	virtual bool isEmpty() const = 0;
	virtual void parseHtmlPage(const QByteArray& htmlPage, const ResponseHeaders& headers) = 0;
	virtual IHtmlNodeCountedPtr firstMatchNode(IHtmlNode::TagId tagId) const = 0;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodes(IHtmlNode::TagId tagId) const = 0;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodesInDepth(IHtmlNode::TagId tagId) const = 0;
	virtual std::vector<IHtmlNodeCountedPtr> matchNodesInDepth(const std::function<bool(const IHtmlNode&)>& predicate) const = 0;
	virtual IHtmlNodeCountedPtr findNodeWithAttributeValue(IHtmlNode::TagId tagId, std::pair<const char*, const char*> expectedAttributes) const = 0;
	virtual IHtmlNodeCountedPtr findNodeWithAttributesValues(IHtmlNode::TagId tagId, const std::map<const char*, const char*>& expectedAttributes) const = 0;
	virtual IHtmlNodeCountedPtr fromData(void* data) const = 0;
	virtual IHtmlAttributeCountedPtr attributeFromData(void* data) const = 0;
	virtual IHtmlNodeCountedPtr root() const = 0;
	virtual IHtmlNodeCountedPtr emptyNode() const = 0;
	virtual IHtmlAttributeCountedPtr emptyAttribute() const = 0;

	// TODO: this method must not be in this interface
	// we need to implement them using matchNodesInDepth method
	virtual std::vector<Url> dofollowAhrefs() const = 0;
	virtual std::vector<Url> nofollowAhrefs() const = 0;
	virtual std::vector<Url> hreflangs() const = 0;

protected:
	enum AhrefsType
	{
		DofollowAhrefs,
		NofollowAhrefs
	};
};

}