#pragma once

#include "iworker_page_loader.h"
#include "requester_wrapper.h"

namespace spiderpp
{

class Requester;
class LoadSchedule;
struct DownloadResponse;
struct ResetConnectionsResponse;

class MultiRequestPageLoader final
	: public QObject
	, public IWorkerPageLoader
{
	Q_OBJECT

public:
	MultiRequestPageLoader(LoadSchedule* uniqueLinkStore, QObject* parent = nullptr);

	virtual bool canPullLoading() const override;
	virtual void performLoading(const DataToLoad& crawlerRequest) override;
	virtual void setReceiveState(ReceiveState state) override;
	virtual void clear() override;
	virtual QObject* qobject() override;

signals:
	virtual void pageLoaded(RedirectChain& redirectChain, HttpLoadType requestType) override;

private:
	void onLoadingDone(Requester* requester, DownloadResponse& response);
	void removeRequesterAssociatedData(Requester* requester);
	void pauseAllActiveDownloads() const;
	void unpauseAllPausedDownloads() const;
	void resetAllActiveConnections();
	void onResetConnectionsResponse(Requester* requester, const ResetConnectionsResponse&);

private:
	QMap<Requester*, RequesterWrapper> m_activeRequesters;
	ReceiveState m_state;
	LoadSchedule* m_uniqueLinkStore;

	QMap<Requester*, RequesterWrapper> m_onAboutClearData;
	RequesterWrapper m_clearWrapper;
};

}
