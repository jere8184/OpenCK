

#pragma once

#include "base.hpp"
#include "utils/colour.h"

#include <string>
#include <unordered_map>

namespace openck::simulator
{
    struct Charecter;
    struct GraphicalCulture;

    struct ReligionGroup : Base<ReligionGroup>
    {
        struct Flags
        {
            bool has_coa_on_barony_only = false;
            GraphicalCulture* graphical_culture = nullptr;
            bool crusade_cb = false;
            bool playable = false;
            bool ai_peaceful = false;
            int ai_convert_same_group = 0;
            int ai_convert_other_group = 0;

            openck::util::Colour color;
        };

        Flags flags;
        ReligionGroup(const std::string& name) : Base(name) {}

        static void allocate_range(const std::vector<parser::Node>& nodes);

        static void initalise_range(const std::vector<parser::Node>& nodes);
    };

    struct Religion : Base<Religion>
    {
        ReligionGroup* group;
        float moral_authority;

        Religion(const std::string& name) : Base(name) {}

        Charecter* get_head(){return nullptr;};
    };
}
