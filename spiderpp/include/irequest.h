#pragma once

namespace spiderpp
{

enum class RequestType
{
	RequestDownload,
	RequestGetHostInfo,
	RequestTypeRunTask,
	RequestCheck404IsProper,
	RequestSetSerialNumber,
	RequestGetSerialNumberData,
	RequestGetSerialNumberState,
	RequestTakeScreenshot,
	RequestPauseConnections,
	RequestUnpauseConnections,
	RequestResetConnections
};

class IRequest
{
public:
	virtual ~IRequest() = default;
	virtual std::shared_ptr<IRequest> clone() const = 0;
	virtual RequestType requestType() const noexcept = 0;
};

}