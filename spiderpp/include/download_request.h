#pragma once

#include "irequest.h"
#include "crawler_request.h"

namespace spiderpp
{

struct DownloadRequest : public IRequest
{
	enum class BodyProcessingCommand
	{
		CommandDownloadBodyAnyway,
		CommandAutoDetectionBodyLoading
	};

	DownloadRequest(const CrawlerRequest& requestInfo,
		BodyProcessingCommand bodyProcessingCommand = BodyProcessingCommand::CommandAutoDetectionBodyLoading,
		bool useTimeout = false,
		bool ignoreMaxParallelConnections = false)
		: requestInfo(requestInfo)
		, turnaround(turnaround)
		, bodyProcessingCommand(bodyProcessingCommand)
		, useTimeout(useTimeout)
		, ignoreMaxParallelConnections(ignoreMaxParallelConnections)
	{
	}

	virtual std::shared_ptr<IRequest> clone() const override
	{
		return std::make_shared<DownloadRequest>(*this);
	}
	virtual RequestType requestType() const noexcept override
	{
		return RequestType::RequestDownload;
	}

	CrawlerRequest requestInfo;
	BodyProcessingCommand bodyProcessingCommand;
	int turnaround;

	//! use timeout set by settings
	bool useTimeout;

	//! this download request will be scheduled immediately ignoring the maximum connection count
	bool ignoreMaxParallelConnections;
};

}