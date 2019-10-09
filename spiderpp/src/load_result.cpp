#include "load_result.h"

namespace spiderpp
{

LoadResult::LoadResult(const Url& url, const Url& redirectUrl, Common::StatusCode statusCode, const QByteArray& body, const ResponseHeaders& responseHeaders, int elapsedTime)
	: m_url(url)
	, m_redirectUrl(redirectUrl)
	, m_statusCode(statusCode)
	, m_body(body)
	, m_responseHeaders(responseHeaders)
	, m_elapsedTime(elapsedTime)
{
}

const Url& LoadResult::url() const noexcept
{
	return m_url;
}

Url& LoadResult::url() noexcept
{
	return m_url;
}

void LoadResult::setUrl(const Url& url) noexcept
{
	m_url = url;
}

const Url& LoadResult::redirectUrl() const noexcept
{
	return m_redirectUrl;
}

Url& LoadResult::redirectUrl() noexcept
{
	return m_redirectUrl;
}

void LoadResult::setRedirectUrl(const Url& redirectUrl) noexcept
{
	m_redirectUrl = redirectUrl;
}

Common::StatusCode LoadResult::statusCode() const noexcept
{
	return m_statusCode;
}

void LoadResult::setStatusCode(Common::StatusCode statusCode) noexcept
{
	m_statusCode = statusCode;
}

const QByteArray& LoadResult::body() const noexcept
{
	return m_body;
}

QByteArray& LoadResult::body() noexcept
{
	return m_body;
}

void LoadResult::setBody(const QByteArray& body) noexcept
{
	m_body = body;
}

const ResponseHeaders& LoadResult::responseHeaders() const noexcept
{
	return m_responseHeaders;
}

ResponseHeaders& LoadResult::responseHeaders() noexcept
{
	return m_responseHeaders;
}

void LoadResult::setResponseHeaders(const ResponseHeaders& responseHeaders) noexcept
{
	m_responseHeaders = responseHeaders;
}

int LoadResult::elapsedTime() const
{
	return m_elapsedTime;
}

void LoadResult::setElapsedTime(int elapsedTime)
{
	m_elapsedTime = elapsedTime;
}

}
