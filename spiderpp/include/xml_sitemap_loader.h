#pragma once

#include "ispecific_loader.h"
#include "redirect_chain.h"
#include "requester_wrapper.h"
#include "url.h"

namespace spiderpp
{

struct DownloadResponse;
class RobotsTxtLoader;
class Requester;

class XmlSitemapLoader : public QObject, public ISpecificLoader
{
	Q_OBJECT

public:
	XmlSitemapLoader(RobotsTxtLoader* robotsTxtLoader, QObject* parent = nullptr);

	virtual void setHost(const Url& url) override;
	virtual Url host() const noexcept override;
	virtual void load() override;
	virtual const QByteArray& content() const noexcept override;
	virtual void setContent(const QByteArray& content) noexcept override;
	virtual bool isReady() const noexcept override;
	virtual bool isValid() const noexcept override;
	virtual void setValid(bool valid) noexcept override;
	virtual QObject* qobject() override;

signals:
	virtual void ready() override;

private slots:
	void onRobotsTxtLoaderReady();

private:
	void onLoadingDone(Requester* requester, const DownloadResponse& response);

private:
	bool m_isReady;
	bool m_isValid;
	QByteArray m_content;
	RedirectChain m_hopsChain;
	RequesterWrapper m_downloadRequester;
	RobotsTxtLoader* m_robotsTxtLoader;
	Url m_host;
};

}