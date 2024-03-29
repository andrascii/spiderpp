#include "requester.h"
#include "handler_registry.h"
#include "thread_message_dispatcher.h"

namespace spiderpp
{

Requester::Requester(const IRequest& request)
	: m_request(request.clone())
	, m_state(StateClear)
{
	HandlerRegistry& handlerRegistry = HandlerRegistry::instance();
	m_handler = handlerRegistry.handlerForRequest(request);

	ASSERT(m_handler && "Handler for this request was not registered");

	qRegisterMetaType<Requester>("Requester");
	qRegisterMetaType<RequesterSharedPtr>("RequesterSharedPtr");
}

Requester::Requester(const Requester& other)
	: m_request(other.m_request)
	, m_delegate(other.m_delegate)
	, m_handler(other.m_handler)
	, m_state(other.m_state)
{
}

QObject* Requester::handler() const noexcept
{
	return m_handler;
}

IRequest* Requester::request() const noexcept
{
	return m_request.get();
}

Requester::State Requester::state() const noexcept
{
	return m_state;
}

void Requester::start()
{
	m_state = StateWorking;

	ThreadMessageDispatcher::forCurrentThread()->startRequest(shared_from_this());
}

void Requester::stop()
{
	m_state = StateStopped;

	ThreadMessageDispatcher::forCurrentThread()->stopRequest(shared_from_this());
}

void Requester::processResponse(const IResponse& response) const
{
	if (m_state == StateStopped)
	{
		return;
	}

	m_delegate(response);

	HandlerRegistry& handlerRegistry = HandlerRegistry::instance();

	const std::vector<HandlerRegistry::Subscription> subscriptions =
		handlerRegistry.subscriptionsFor(m_handler, response.type());

	for (const HandlerRegistry::Subscription& subscription : subscriptions)
	{
		if (!subscription.subscriber)
		{
			continue;
		}

		const Qt::ConnectionType connectionType = subscription.subscriber->thread() == QThread::currentThread() ?
			Qt::DirectConnection : Qt::QueuedConnection;

		VERIFY(subscription.method.invoke(subscription.subscriber, connectionType, Q_ARG(const IResponse&, response)));
	}
}

}