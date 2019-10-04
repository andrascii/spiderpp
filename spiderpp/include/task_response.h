#pragma once

#include "iresponse.h"

namespace spiderpp
{

struct TaskResponseResult
{
	virtual ~TaskResponseResult() = default;
};

struct TaskResponse : public IResponse
{
	TaskResponse(std::shared_ptr<TaskResponseResult> result)
		: result(result)
	{
	}
	
	virtual ResponseType type() const noexcept override
	{
		return ResponseType::ResponseTypeTaskResult;
	}

	DEFINE_RESPONSE_STATIC_TYPE_IN_CLASS(ResponseType::ResponseTypeTaskResult)

	std::shared_ptr<TaskResponseResult> result;
};

}