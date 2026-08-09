
#pragma once

#include "Base.hpp"

#include <string>

namespace openck::simulator
{
	struct Culture : public Base<Culture>
	{
	};

	struct GraphicalCulture : public Base<GraphicalCulture>
	{
		int skin_tone;
		bool christian_portraits; //this should probably be a dynamic field <religion_group>_portraits
	};
}