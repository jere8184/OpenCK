
#pragma once

#include "StatusCode.hpp" 
#include <string_view>

#ifdef LINUX


#include <meta>

template <typename T>
inline const std::string_view ToString(T t)
{
	return std::meta::display_string_of(^^t);
}

#else

std::string_view ToString(StatusCode statusCode)
{
	static std::unordered_map<StatusCode, std::string> map
	{
		STATUS_CODES(GET_PAIR)
	};

	return map.at(statusCode);
}

#endif