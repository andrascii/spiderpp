#pragma once

#include "requester.h"

namespace spiderpp
{

class IDownloadHandler
{
public:
	virtual ~IDownloadHandler() = default;

	virtual void setPauseRange(int from, int to) = 0;
	virtual void setTimeout(int msecs) = 0;
	virtual void setMaxRedirects(int redirects) = 0;
	virtual void setMaxParallelConnections(int connections) = 0;
	virtual void resetPauseRange() = 0;
	virtual void setUserAgent(const QByteArray& userAgent) = 0;
	virtual void setProxy(const QString& proxyHostName, int proxyPort, const QString& proxyUser, const QString& proxyPassword) = 0;
	virtual void resetProxy() = 0;
	virtual void handleRequest(RequesterSharedPtr requester) = 0;
	virtual void stopRequestHandling(RequesterSharedPtr requester) = 0;
	virtual QObject* qobject() = 0;
};

}