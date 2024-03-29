#include "qt_based_download_handler.h"
#include "download_request.h"
#include "download_response.h"
#include "thread_message_dispatcher.h"
#include "page_parser_helpers.h"
#include "status_code.h"
#include "load_result.h"
#include "service_locator.h"
#include "inotification_service.h"
#include "random_interval_range_timer.h"
#include "helpers.h"
#include "finally.h"

namespace spiderpp
{

QtBasedDownloadHandler::QtBasedDownloadHandler()
	: m_networkAccessor(new QNetworkAccessManager(this))
	, m_timeoutTimer(new QTimer(this))
	, m_timeout(-1)
{
	VERIFY(connect(m_networkAccessor, SIGNAL(finished(QNetworkReply*)), SLOT(urlDownloaded(QNetworkReply*)), Qt::DirectConnection));

	VERIFY(connect(m_networkAccessor, SIGNAL(proxyAuthenticationRequired(const QNetworkProxy&, QAuthenticator*)),
		this, SLOT(proxyAuthenticationRequired()), Qt::DirectConnection));

	m_timeoutTimer->setInterval(100);

	VERIFY(connect(m_timeoutTimer, &QTimer::timeout,
		this, &QtBasedDownloadHandler::onTimeoutTimerTicked, Qt::DirectConnection));
}

void QtBasedDownloadHandler::setTimeout(int msecs)
{
	m_timeout = msecs;

	if (msecs <= 0)
	{
		m_timeoutTimer->stop();
	}
	else
	{
		m_timeoutTimer->start();
	}
}

void QtBasedDownloadHandler::setUserAgent(const QByteArray& userAgent)
{
	DEBUG_ASSERT(thread() == QThread::currentThread() && "This method should be called from the same thread");
	m_userAgent = userAgent;
}

void QtBasedDownloadHandler::setProxy(const QString& proxyHostName, int proxyPort, const QString& proxyUser,
	const QString& proxyPassword)
{
	DEBUG_ASSERT(thread() == QThread::currentThread() && "This method should be called from the same thread");

	QNetworkProxy proxy;
	proxy.setType(QNetworkProxy::HttpProxy);
	proxy.setHostName(proxyHostName);
	proxy.setPort(proxyPort);

	if(!proxyUser.isEmpty() && !proxyPassword.isEmpty())
	{
		proxy.setUser(proxyUser);
		proxy.setPassword(proxyPassword);
	}

	m_networkAccessor->setProxy(proxy);
}

void QtBasedDownloadHandler::resetProxy()
{
	m_networkAccessor->setProxy(QNetworkProxy::DefaultProxy);
}

void QtBasedDownloadHandler::stopRequestHandling(RequesterSharedPtr requester)
{
	const auto iter = m_activeRequestersReplies.find(requester);

	if (iter == m_activeRequestersReplies.end())
	{
		removeRequesterFromQueue(requester);
		return;
	}

	QNetworkReply* reply = iter->second;
	const QSignalBlocker signalBlocker(reply);
	reply->abort();

	m_activeRequestersReplies.erase(iter);

	int requestId = -1;

	for(auto first = m_requesters.begin(), last = m_requesters.end(); first != last; ++first)
	{
		if (first.value().lock() == requester)
		{
			requestId = first.key();
			break;
		}
	}

	if (requestId != -1)
	{
		m_requesters.remove(requestId);
		m_activeRequests.removeOne(requestId);
	}
}

QObject* QtBasedDownloadHandler::qobject()
{
	return this;
}

void QtBasedDownloadHandler::onTimeoutTimerTicked()
{
	const qint64 currentMsecsSinceEpoch = QDateTime::currentMSecsSinceEpoch();

	QSet<QNetworkReply*> timeoutReplies;
	QSet<QNetworkReply*> destroyedReplies;
	foreach (QNetworkReply* reply, m_activeReplies.keys())
	{
		if (m_activeReplies.value(reply).first.isNull())
		{
			destroyedReplies.insert(reply);
			continue;
		}

		if (currentMsecsSinceEpoch - m_activeReplies.value(reply).second > static_cast<qint64>(m_timeout))
		{
			timeoutReplies.insert(reply);
		}
	}

	foreach(QNetworkReply* reply, timeoutReplies)
	{
		reply->setProperty("timeout", true);
		reply->abort();
		ASSERT(reply->property("processed").isValid());
		ASSERT(!m_activeReplies.contains(reply));
	}

	foreach(QNetworkReply* reply, destroyedReplies)
	{
		m_activeReplies.remove(reply);
	}
}

void QtBasedDownloadHandler::onAboutDownloadProgress(qint64 bytesReceived, qint64 bytesTotal)
{
	QNetworkReply* reply = qobject_cast<QNetworkReply*>(sender());

	ASSERT(reply);

	const int requestId = reply->property("requestId").toInt();
	const RequesterSharedPtr requester = m_requesters[requestId].lock();

	std::shared_ptr<DownloadProgressResponse> downloadProgressResponse = std::make_shared<DownloadProgressResponse>();
	downloadProgressResponse->bytesReceived = bytesReceived;
	downloadProgressResponse->bytesTotal = bytesTotal;

	ThreadMessageDispatcher::forThread(requester->thread())->postResponse(requester, downloadProgressResponse);
}

bool QtBasedDownloadHandler::isAutoDetectionBodyProcessing(QNetworkReply* reply) const
{
	const int requestId = reply->property("requestId").toInt();
	const RequesterSharedPtr requester = m_requesters[requestId].lock();

	DownloadRequest* request = Common::Helpers::fast_cast<DownloadRequest*>(requester->request());
	return request->bodyProcessingCommand == DownloadRequest::BodyProcessingCommand::CommandAutoDetectionBodyLoading;
}

void QtBasedDownloadHandler::urlDownloaded(QNetworkReply* reply)
{
	processReply(reply);
}

void QtBasedDownloadHandler::metaDataChanged(QNetworkReply* reply)
{
	if (isReplyProcessed(reply))
	{
		return;
	}

	const bool nonHtmlResponse = !PageParserHelpers::isHtmlOrPlainContentType(
		reply->header(QNetworkRequest::ContentTypeHeader).toString());

	const Common::StatusCode statusCode = replyStatusCode(reply);
	const bool redirectionStatusCode = statusCode == Common::StatusCode::MovedPermanently301 ||
		statusCode == Common::StatusCode::MovedTemporarily302;

	if (isAutoDetectionBodyProcessing(reply) && nonHtmlResponse && !redirectionStatusCode)
	{
		processReply(reply);
		reply->abort();
	}
}

void QtBasedDownloadHandler::queryError(QNetworkReply* reply, QNetworkReply::NetworkError code)
{
	ERRLOG << reply->url().toDisplayString() << reply->errorString() << code;
}

void QtBasedDownloadHandler::processReply(QNetworkReply* reply)
{
	DEBUGLOG << "Loaded:" << reply->url().toDisplayString();

	if (isReplyProcessed(reply))
	{
		return;
	}

	Common::Finally replyGuard([reply] { reply->deleteLater(); });

	markReplyAsProcessed(reply);
	reply->disconnect(this);

	const HttpLoadType requestType = static_cast<HttpLoadType>(reply->property("crawlerRequestType").toInt());
	Common::StatusCode statusCode = replyStatusCode(reply);
	const int requestId = reply->property("requestId").toInt();

	if (!m_requesters.contains(requestId))
	{
		m_responses.remove(requestId);
		return;
	}

	const RequesterSharedPtr requester = m_requesters[requestId].lock();

	if (!requester)
	{
		return;
	}

	std::shared_ptr<DownloadResponse> response = responseFor(requestId);

	QByteArray body = statusCode == Common::StatusCode::Timeout ? QByteArray() : readBody(reply);
	const Url redirectUrlAddress = redirectUrl(reply);

	if (statusCode == Common::StatusCode::MovedPermanently301 ||
		statusCode == Common::StatusCode::MovedTemporarily302)
	{
		if (response->redirectChain.length() == static_cast<size_t>(maxRedirectsToProcess()))
		{
			statusCode = Common::StatusCode::TooManyRedirections;
			body = QByteArray();
		}
		else
		{
			int urlsInChain = 0;
			for (size_t i = 0; i < response->redirectChain.length(); ++i)
			{
				if (response->redirectChain[i].url() == redirectUrlAddress)
				{
					urlsInChain += 1;
				}
			}

			if (urlsInChain <= 2)
			{
				const DataToLoad redirectKey{ redirectUrlAddress, requestType };
				loadHelper(redirectKey, requestId, reply->property("useTimeout").isValid());
				response->redirectChain.addLoadResult(LoadResult{ reply->url(), redirectUrlAddress, statusCode, body, reply->rawHeaderPairs(), -1 });
				return;
			}
		}
	}

	response->redirectChain.addLoadResult(LoadResult(reply->url(), redirectUrlAddress, statusCode, body, reply->rawHeaderPairs(), -1));
	ThreadMessageDispatcher::forThread(requester->thread())->postResponse(requester, response);

	const auto iter = m_activeRequestersReplies.find(requester);

	m_responses.remove(requestId);
	m_requesters.remove(requestId);
	m_activeRequestersReplies.erase(iter);

	ASSERT(m_activeRequests.removeOne(requestId));
}

bool QtBasedDownloadHandler::isReplyProcessed(QNetworkReply* reply) const noexcept
{
	if (reply == nullptr)
	{
		// was already destroyed by deleteLater()
		return true;
	}

	const QVariant alreadyProcessed = reply->property("processed");

	return alreadyProcessed.isValid();
}

void QtBasedDownloadHandler::markReplyAsProcessed(QNetworkReply* reply) noexcept
{
	ASSERT(reply != nullptr);

	reply->setProperty("processed", true);

	m_activeReplies.remove(reply);
}

void QtBasedDownloadHandler::load(RequesterSharedPtr requester)
{
	DownloadRequest* request = Common::Helpers::fast_cast<DownloadRequest*>(requester->request());

	const auto[requestId, reply] = loadHelper(request->requestInfo, -1, request->useTimeout);

	m_requesters[requestId] = requester;
	m_activeRequestersReplies[requester] = reply;
}

std::pair<int, QNetworkReply*> QtBasedDownloadHandler::loadHelper(const DataToLoad& request, int parentRequestId, bool useTimeout)
{
	static int s_request_id = 0;
	QNetworkReply* reply = nullptr;
	QNetworkRequest networkRequest(request.url);
	networkRequest.setRawHeader("User-Agent", m_userAgent);

	switch (request.requestType)
	{
		case HttpLoadType::RequestTypeGet:
		{
			reply = m_networkAccessor->get(networkRequest);
			break;
		}
		case HttpLoadType::RequestTypeHead:
		{
			reply = m_networkAccessor->head(networkRequest);
			break;
		}
		default:
		{
			ASSERT(!"Unsupported request type");
			break;
		}
	}

	if (useTimeout)
	{
		m_activeReplies[reply] = QPair<QPointer<QNetworkReply>, qint64>(reply, QDateTime::currentMSecsSinceEpoch());
		reply->setProperty("useTimeout", true);
	}

	reply->setProperty("crawlerRequestType", static_cast<int>(request.requestType));

	const int resultRequestId = parentRequestId == -1 ? s_request_id : parentRequestId;
	reply->setProperty("requestId", resultRequestId);

	if (parentRequestId == -1)
	{
		m_activeRequests.push_back(resultRequestId);
	}

	++s_request_id;

	VERIFY(connect(reply, &QNetworkReply::metaDataChanged, this, [this, reply]() { metaDataChanged(reply); }, Qt::QueuedConnection));

	VERIFY(connect(reply, QOverload<QNetworkReply::NetworkError>::of(&QNetworkReply::error), this,
		[this, reply](QNetworkReply::NetworkError code) { queryError(reply, code); }, Qt::QueuedConnection));

	VERIFY(connect(reply, SIGNAL(downloadProgress(qint64, qint64)), SLOT(onAboutDownloadProgress(qint64, qint64))));

	return std::make_pair(resultRequestId, reply);
}

std::shared_ptr<DownloadResponse> QtBasedDownloadHandler::responseFor(int requestId)
{
	if (!m_responses.contains(requestId))
	{
		m_responses[requestId] = std::make_shared<DownloadResponse>();
	}

	return m_responses[requestId];
}

void QtBasedDownloadHandler::pauseRequesters(const QList<Requester*>&)
{
}

void QtBasedDownloadHandler::unpauseRequesters(const QList<Requester*>&)
{
}

void QtBasedDownloadHandler::resetRequesters(const QList<Requester*>&)
{
}

QByteArray QtBasedDownloadHandler::readBody(QNetworkReply* reply) const
{
	QByteArray body;

	if (isAutoDetectionBodyProcessing(reply))
	{
		const bool processBody = PageParserHelpers::isHtmlOrPlainContentType(
			reply->header(QNetworkRequest::ContentTypeHeader).toString()
		);

		body = processBody ? reply->readAll() : QByteArray();
	}
	else
	{
		body = reply->readAll();
		body.squeeze();
	}

	return body;
}

Common::StatusCode QtBasedDownloadHandler::replyStatusCode(QNetworkReply* reply) const
{
	if (reply->property("timeout").isValid())
	{
		return Common::StatusCode::Timeout;
	}

	const QNetworkReply::NetworkError error = reply->error();

	Common::StatusCode code = Common::StatusCode::Undefined;

	if (error == QNetworkReply::NoError)
	{
		code = static_cast<Common::StatusCode>(reply->attribute(QNetworkRequest::HttpStatusCodeAttribute).toInt());
	}
	else
	{
		switch (error)
		{
			case QNetworkReply::UnknownNetworkError:
			{
				code = Common::StatusCode::UnknownNetworkError;
				break;
			}
			case QNetworkReply::ContentNotFoundError:
			{
				code = Common::StatusCode::NotFound404;
				break;
			}
			case QNetworkReply::ContentAccessDenied:
			{
				code = Common::StatusCode::Forbidden403;
				break;
			}
			case QNetworkReply::TimeoutError:
			case QNetworkReply::ProxyTimeoutError:
			{
				code = Common::StatusCode::Timeout;
				break;
			}
			case QNetworkReply::InternalServerError:
			{
				code = Common::StatusCode::InternalServerError500;
				break;
			}
		}
	}

	return code;
}

Url QtBasedDownloadHandler::redirectUrl(QNetworkReply* reply) const
{
	Url redirectUrl = reply->attribute(QNetworkRequest::RedirectionTargetAttribute).toUrl();

	if (redirectUrl.isValid() && redirectUrl.isRelative())
	{
		redirectUrl = PageParserHelpers::resolveRelativeUrl(redirectUrl, reply->url());
	}

	return redirectUrl;
}

}