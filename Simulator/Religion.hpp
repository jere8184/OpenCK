

#pragma once

#include <string>
#include "unordered_map"

namespace openck::simulator
{
    struct Charecter;

    struct ReligionGroup
    {
        std::string name;
        int id;
        static ReligionGroup* get_religion_group_by_name(const std::string& name) {static ReligionGroup c; return &c; /*Dummy function*/ }
        //static const std::unordered_map<std::string, ReligionGroup*> religion_groups {{"muslim", (ReligionGroup*)1 }};
    };

    struct Religion
    {
        ReligionGroup* group;
        int id;
        std::string name;
        float moral_authority;
        Charecter* get_head(){return nullptr;};
    };
}
