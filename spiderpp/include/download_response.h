#pragma once

#include "iresponse.h"
#include "response_headers.h"
#include "redirect_chain.h"

namespace spiderpp
{

struct DownloadResponse : public IResponse
{
	virtual ResponseType type() const noexcept override
	{
		return ResponseType::ResponseTypeDownload;
	}

	DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(ResponseType::ResponseTypeDownload)

	RedirectChain redirectChain;
};

struct DownloadProgressResponse : public IResponse
{
	virtual ResponseType type() const noexcept override
	{
		return ResponseType::ResponseTypeDownloadProgress;
	}

	DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(ResponseType::ResponseTypeDownloadProgress)

	quint64 bytesReceived;
	quint64 bytesTotal;
};

struct UploadProgressResponse : public IResponse
{
	virtual ResponseType type() const noexcept override
	{
		return ResponseType::ResponseTypeUploadProgress;
	}

	DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(ResponseType::ResponseTypeUploadProgress)

	quint64 bytesSent;
	quint64 bytesTotal;
};


}