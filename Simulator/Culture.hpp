
#pragma once

#include "base.hpp"

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

    template<>
    Base<GraphicalCulture>::FieldSetters Base<GraphicalCulture>::field_setters = 
    {
        {"skin_tone", [](GraphicalCulture* graphical_culture, const Node& node){return node.get_value(graphical_culture->skin_tone);}},
        {"christian_portraits", [](GraphicalCulture* graphical_culture, const Node& node){return node.get_value(graphical_culture->christian_portraits);}}
    };
};