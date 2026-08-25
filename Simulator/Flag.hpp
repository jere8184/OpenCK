
#pragma once

#include "Simulator/Base.hpp"

namespace openck::simulator
{

struct Flag : Base<Flag> 
{
	Flag(const std::string& name) : Base(name)
	{
	}

	static StatusCode GetByName(const Flag*& flag, const std::string& name)
	{
		if (StatusCode::SUCCESS != Base::GetByName(flag, name))
		{
			Base::map.emplace(name, Flag(name));
		}

		return Base::GetByName(flag, name);
	}
};

}