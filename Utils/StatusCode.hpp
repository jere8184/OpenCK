
#pragma once

#include <string>
#include <unordered_map>


enum class StatusCode
{
	SUCCESS,
	FAILURE,
	CONTINUE,
	NOT_FOUND,
	NOT_HANDLED,
	NOT_IMPLIMENTED,
	EMPTY
};

const std::unordered_map<StatusCode, const std::string> StatusCode_to_string_map
{
	{StatusCode::SUCCESS,	"SUCCESS"},
	{StatusCode::FAILURE,	"FAILURE"},
	{StatusCode::CONTINUE,	"CONTINUE"},
	{StatusCode::NOT_FOUND,	"NOT_FOUND"},
	{StatusCode::NOT_HANDLED,	"NOT_HANDLED"},
	{StatusCode::NOT_IMPLIMENTED,	"NOT_IMPLIMENTED"}
};

inline const std::string& StatusCode_to_string(StatusCode StatusCode)
{
	return StatusCode_to_string_map.at(StatusCode);
}

#define RETURN_RESULT_IF_3(StatusCodeVal, op, expression)\
	do\
	{\
		StatusCode result = expression;\
		if (StatusCodeVal op result)\
			return result;\
	} while(false)

#define RETURN_RESULT_IF_4(StatusCode, op, expression, node)\
	RETURN_RESULT_IF_5(StatusCode, op, expression, node, "")

#define RETURN_RESULT_IF_5(StatusCodeVal, op, expression, node, action)\
	do\
	{\
		StatusCode result = expression;\
		if (StatusCodeVal op result)\
		{\
			return node.debugging.store_and_forward_result(result, action);\
		}\
	} while(false)

#define EXPAND(x)	x
#define GET_MACRO(_1, _2, _3, _4, _5, name, ...)    name
#define RETURN_RESULT_IF(...)    EXPAND( GET_MACRO(__VA_ARGS__, RETURN_RESULT_IF_5, RETURN_RESULT_IF_4, RETURN_RESULT_IF_3)(__VA_ARGS__) )