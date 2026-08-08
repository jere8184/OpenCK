
#pragma once

#include "Simulator/Base.hpp"

namespace openck::simulator
{

struct Flag : Base<Flag> 
{
	Flag(const std::string& name) : Base(name)
	{
	}

	static StatusCode get_by_name(const Flag*& flag, const std::string& name)
	{
		if (StatusCode::SUCCESS != Base::get_by_name(flag, name))
		{
			Base::map.emplace(name, Flag(name));
		}

		return Base::get_by_name(flag, name);
	}
};

}