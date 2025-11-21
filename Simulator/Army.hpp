
#pragma once

#include <string>
#include <vector>
#include <map>

namespace openck::simulator 
{
    struct UnitType 
    {
        //using Id = int;

        //Id id;
        std::string name;

        static UnitType* get_unit_type_by_name(const std::string name) { return UnitType::unit_types.contains(name) ? UnitType::unit_types.at(name) : nullptr;};
        static std::map<std::string, UnitType*> unit_types;
    };

    extern std::vector<UnitType> unit_vector;
}