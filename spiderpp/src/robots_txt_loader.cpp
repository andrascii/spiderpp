#include "robots_txt_loader.h"
#include "status_code.h"
#include "download_request.h"
#include "download_response.h"
#include "url.h"

namespace spiderpp
{

RobotsTxtLoader::RobotsTxtLoader(QObject* parent)
	: QObject(parent)
	, m_isReady(false)
	, m_isValid(false)
{
}

void RobotsTxtLoader::setHost(const Url& url)
{
	m_host = url;
}

Url RobotsTxtLoader::host() const noexcept
{
	return m_host;
}

void RobotsTxtLoader::load()
{
	const Url robotsTxtUrl = m_host.scheme() + "://" + m_host.host() + QStringLiteral("/robots.txt");

	if (m_redirectChain.hasRedirectTo(robotsTxtUrl))
	{
		emit ready();
		return;
	}

	CrawlerRequest requestInfo{ robotsTxtUrl, DownloadRequestType::RequestTypeGet };
	DownloadRequest request(requestInfo);
	m_downloadRequester.reset(request, this, &RobotsTxtLoader::onLoadingDone);
	m_downloadRequester->start();
}

const QByteArray& RobotsTxtLoader::content() const noexcept
{
	return m_content;
}

void RobotsTxtLoader::setContent(const QByteArray& content) noexcept
{
	m_content = content;
}

bool RobotsTxtLoader::isReady() const noexcept
{
	return m_isReady;
}

bool RobotsTxtLoader::isValid() const noexcept
{
	return m_isValid;
}

void RobotsTxtLoader::setValid(bool valid) noexcept
{
	m_isReady = true;
	m_isValid = valid;
}

QObject* RobotsTxtLoader::qobject()
{
	return this;
}

spiderpp::Url RobotsTxtLoader::robotsTxtUrl() const
{
	return !m_redirectChain.empty() ? m_redirectChain.lastLoadResult().url() : Url();
}

void RobotsTxtLoader::onLoadingDone(Requester* requester, const DownloadResponse& response)
{
	Q_UNUSED(requester);

	const Common::StatusCode statusCode = response.redirectChain.lastLoadResult().statusCode();

	m_isValid = statusCode == Common::StatusCode::Ok200;

	m_content = response.redirectChain.lastLoadResult().body();
	m_isReady = true;

	m_redirectChain = response.redirectChain;

	emit ready();
}

}
