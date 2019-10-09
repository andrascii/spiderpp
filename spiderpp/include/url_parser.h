#pragma once

namespace spiderpp
{

class Url;
class LoadResult;
class IHtmlParser;
enum class ResourceType;

class UrlParser
{
public:
	struct UrlList
	{
		std::vector<Url> dofollowAhrefs;
		std::vector<Url> nofollowAhrefs;
		std::vector<Url> hreflangs;
	};

	UrlParser();

	UrlList urlList(const LoadResult& hop);

private:
	enum class ResourceType
	{
		Html,
		Css,
		JavaScript,
		Image,
		Other
	};

	Url parseBaseTagValue() const;
	ResourceType resourceType(const QString& contentType) const;

private:
	Common::counted_ptr<IHtmlParser> m_htmlParser;
	QRegularExpression m_regExp;
};

}