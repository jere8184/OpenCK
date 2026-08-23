
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

template <typename KeyType, typename ValType>
std::unordered_map<ValType, KeyType> Reverse(std::unordered_map<KeyType, ValType> map)
{
	std::unordered_map<ValType, KeyType> newMap;
	for (const auto& [key, val] : map)
	{
		newMap[val]  = key;
	}

	return newMap;
}

inline std::string_view ToString(StatusCode statusCode)
{
	return statusCodeToNameMap.at(statusCode);
}

#endif