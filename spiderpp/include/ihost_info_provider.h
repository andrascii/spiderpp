#pragma once

namespace spiderpp
{

class IHostInfoProvider
{
public:
	virtual ~IHostInfoProvider() = default;

	virtual void handleRequest(RequesterSharedPtr requester) = 0;
	virtual void stopRequestHandling(RequesterSharedPtr requester) = 0;
	virtual QObject* qobject() = 0;
};

}