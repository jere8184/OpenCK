
#pragma once

#include <string>
#include <unordered_map>

#define STATUS_CODES(d)\
	d(SUCCESS),\
	d(FAILURE),\
	d(CONTINUE),\
	d(NOT_FOUND),\
	d(NOT_HANDLED),\
	d(NOT_IMPLIMENTED),\
	d(EMPTY)

#define GET_NAME(e) e

enum class StatusCode
{
	STATUS_CODES(GET_NAME)
};

#define GET_PAIR(e) {StatusCode::e, #e}

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