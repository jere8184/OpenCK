

#pragma once

#include <string>

#include "base.hpp"
#include "unordered_map"

namespace openck::simulator
{
    struct Charecter;
    struct Culture;

    struct ReligionGroup : Base<ReligionGroup>
    {
        struct Flags
        {
            bool has_coa_on_barony_only = false;
            Culture* graphical_culture = nullptr;
            bool crusade_cb = false;
            bool playable = false;
            bool ai_peaceful = false;
            int ai_convert_same_group = 0;
            int ai_convert_other_group = 0;
        };

        Flags flags;        
        ReligionGroup(const std::string& name) : Base(name) {}
    };

    struct Religion : Base<Religion>
    {
        ReligionGroup* group;
        float moral_authority;

        Religion(const std::string& name) : Base(name) {}

        Charecter* get_head(){return nullptr;};
    };
}
