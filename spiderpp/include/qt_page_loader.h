#pragma once

#include "iworker_page_loader.h"
#include "requester_wrapper.h"

namespace spiderpp
{

class Requester;
class LoadSchedule;
struct DownloadResponse;

class QtPageLoader : public QObject, public IWorkerPageLoader
{
	Q_OBJECT

public:
	QtPageLoader(LoadSchedule* uniqueLinkStore);

	virtual bool canPullLoading() const override;
	virtual void performLoading(const DataToLoad& crawlerRequest) override;
	virtual void setReceiveState(ReceiveState state) override;
	virtual void clear() override;

	virtual QObject* qobject() override;

signals:
	virtual void pageLoaded(RedirectChain& redirectChain, HttpLoadType requestType) override;

private:
	void onLoadingDone(Requester* requester, DownloadResponse& response);

private:
	ReceiveState m_state;
	LoadSchedule* m_uniqueLinkStore;
	RequesterWrapper m_requester;
};

}