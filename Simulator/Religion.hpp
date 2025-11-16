

#pragma once

#include <string>
#include "unordered_map"

namespace openck::simulator
{
    struct Charecter;

    struct ReligionGroup
    {
        using Id = size_t;

        std::string name;
        Id id;
        static ReligionGroup* get_religion_group_by_name(const std::string& name) {return religion_groups.contains(name) ? religion_groups.at(name) : nullptr;}
        static std::unordered_map<std::string, ReligionGroup*> religion_groups;
    };

    struct Religion
    {
        ReligionGroup* group;
        int id;
        std::string name;
        float moral_authority;

        static Religion* get_religion_by_name(const std::string& name) {return religion.contains(name) ? religion.at(name) : nullptr;}
        static std::unordered_map<std::string, Religion*> religion;
        Charecter* get_head(){return nullptr;};
    };
}
