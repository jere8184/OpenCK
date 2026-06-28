
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
	NOT_IMPLIMENTED
};

const std::unordered_map<StatusCode, const std::string> status_code_to_string_map
{
	{StatusCode::SUCCESS,	"SUCCESS"},
	{StatusCode::FAILURE,	"FAILURE"},
	{StatusCode::CONTINUE,	"CONTINUE"},
	{StatusCode::NOT_FOUND,	"NOT_FOUND"},
	{StatusCode::NOT_HANDLED,	"NOT_HANDLED"},
	{StatusCode::NOT_IMPLIMENTED,	"NOT_IMPLIMENTED"}
};

inline const std::string& status_code_to_string(StatusCode status_code)
{
	return status_code_to_string_map.at(status_code);
}

#define RETURN_RESULT_IF(status_code, op, expression)\
	do\
	{\
		StatusCode result = expression;\
		if (status_code op result)\
			return result;\
	} while(false)