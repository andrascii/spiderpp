#pragma once

#include "data_to_load.h"
#include "download_request.h"
#include "redirect_chain.h"

namespace spiderpp
{

struct SpiderOptionsData;
struct DataToLoad;

class IWorkerPageLoader
{
public:
	enum ReceiveState
	{
		CanReceivePages,
		CantReceivePages
	};

	virtual ~IWorkerPageLoader() = default;

	virtual bool canPullLoading() const = 0;
	virtual void performLoading(const DataToLoad& crawlerRequest) = 0;
	virtual void setReceiveState(ReceiveState state) = 0;
	virtual void clear() = 0;

	virtual QObject* qobject() = 0;

	// signals
	virtual void pageLoaded(RedirectChain& redirectChain, HttpLoadType requestType) = 0;
};

}
