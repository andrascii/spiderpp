#pragma once

#include "iresponse.h"

namespace CrawlerEngine
{

struct GetHostInfoResponse : IResponse
{
	GetHostInfoResponse(QHostInfo hostInfo)
		: hostInfo(hostInfo)
	{
	}

	DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(ResponseType::ResponseTypeGetHostInfo)

	virtual ResponseType type() const noexcept override
	{
		return ResponseType::ResponseTypeGetHostInfo;
	}

	QHostInfo hostInfo;
	Url url;
};

}
