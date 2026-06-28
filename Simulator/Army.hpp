
#pragma once

#include "simulator/base.hpp"

#include <string>
#include <vector>
#include <map>

namespace openck::simulator
{
	// JM: Should this derive from base?
	struct UnitType : public Base<UnitType>
	{
		//using Id = int;
		using Base = Base<UnitType>;

		UnitType(std::string name);

		static void init_field_setters() {};
		static void init_static_objects();

		//Id id;
		//std::string name;

		//static UnitType* get_unit_type_by_name(const std::string name) { return UnitType::unit_types.contains(name) ? UnitType::unit_types.at(name) : nullptr;};
		//static std::map<std::string, UnitType*> unit_types;
	};
}