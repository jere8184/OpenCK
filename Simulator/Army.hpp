
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

        static const std::map<std::string, UnitType*> unit_type_map;
    };

    extern std::vector<UnitType> unit_vector;
}