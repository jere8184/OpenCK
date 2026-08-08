
#pragma once

#include "Base.hpp"

#include <string>
#include <unordered_map>

namespace openck::simulator
{

struct Terrain : Base<Terrain>
{
	inline void init_field_setters()
	{}

	inline void init_static_objects()
	{}

	inline static StatusCode get_by_name(const Terrain*& derived, const std::string& name)
	{
		return StatusCode::NOT_IMPLIMENTED;
	}
};

}
