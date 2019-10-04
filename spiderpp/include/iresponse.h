#pragma once

namespace spiderpp
{

enum class ResponseType
{
	ResponseTypeUnknown,
	ResponseTypeDownload,
	ResponseTypeDownloadProgress,
	ResponseTypeUploadProgress,
	ResponseTypeGetHostInfo,
	ResponseTypeTaskResult,
	ResponseTypeCheck404IsProper,
	ResponseSetSerialNumber,
	ResponseGetSerialNumberData,
	ResponseGetSerialNumberState,
	ResponseTakeScreenshot,
	ResponseResetConnections
};

#define DECLARE_RESPONSE_STATIC_TYPE(Type) \
	static spiderpp::ResponseType responseStaticType();

#define DEFINE_RESPONSE_STATIC_TYPE(ObjectType, Type) \
	spiderpp::ResponseType ObjectType::responseStaticType() \
	{ \
		return Type; \
	}

#define DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(Type) \
	static spiderpp::ResponseType responseStaticType() \
	{ \
		return Type; \
	}

class IResponse
{
public:
	virtual ~IResponse() = default;
	virtual ResponseType type() const noexcept = 0;
};

using IResponseSharedPtr = std::shared_ptr<IResponse>;

}

Q_DECLARE_METATYPE(spiderpp::IResponseSharedPtr)
