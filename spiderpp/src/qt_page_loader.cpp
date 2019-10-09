#include "qt_page_loader.h"
#include "helpers.h"
#include "download_response.h"
#include "unique_link_store.h"

namespace spiderpp
{

QtPageLoader::QtPageLoader(UniqueLinkStore* uniqueLinkStore)
	: m_state(CanReceivePages)
	, m_uniqueLinkStore(uniqueLinkStore)
{
}

bool QtPageLoader::canPullLoading() const
{
	if (m_requester.get() != nullptr || m_state == CantReceivePages)
	{
		return false;
	}

	if (m_uniqueLinkStore->activeUrlCount() > 50)
	{
		return false;
	}

	return true;
}

void QtPageLoader::performLoading(const DataToLoad& crawlerRequest)
{
	DEBUG_ASSERT(m_state == CanReceivePages);

	DownloadRequest request(crawlerRequest, DownloadRequest::BodyProcessingCommand::CommandAutoDetectionBodyLoading, true);

	m_requester.reset(request, this, &QtPageLoader::onLoadingDone);
	m_requester->start();
}

void QtPageLoader::setReceiveState(ReceiveState state)
{
	DEBUG_ASSERT(thread() == QThread::currentThread());
	ASSERT(state == CanReceivePages || state == CantReceivePages);

	m_state = state;
}

void QtPageLoader::clear()
{
	m_requester.reset();
}

QObject* QtPageLoader::qobject()
{
	return this;
}

void QtPageLoader::onLoadingDone(Requester* requester, DownloadResponse& response)
{
	const DownloadRequest* downloadRequest =
		Common::Helpers::fast_cast<DownloadRequest*>(requester->request());

	emit pageLoaded(response.redirectChain, downloadRequest->requestInfo.requestType);

	m_requester.reset();
}

}
