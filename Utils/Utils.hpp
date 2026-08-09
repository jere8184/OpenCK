
#pragma once

#include <string_view>
#include <meta>

template <typename T>
inline const std::string_view ToString(T t)
{
	return std::meta::display_string_of(^^t);
}