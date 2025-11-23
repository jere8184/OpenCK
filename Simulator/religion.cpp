

#include "religion.hpp"
#include "culture.hpp"

#include "utils/colour.h"

namespace openck::simulator
{

using openck::util::Colour;

template<>
Base<ReligionGroup>::FieldSetters Base<ReligionGroup>::field_setters = 
{
    {"has_coa_on_barony_only",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.has_coa_on_barony_only);}},
    {"playable",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.playable);}},
    {"crusade_cb",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.crusade_cb);}},
    {
        "graphical_culture", 
        [](ReligionGroup* religion_group, const Node& node)
        {
            GraphicalCulture* grapical_culture = GraphicalCulture::get_by_name(node.name);
            religion_group->flags.graphical_culture = grapical_culture;
            return grapical_culture != nullptr;
        }
    },
    
    {"ai_peaceful",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_peaceful);}},
    {"ai_convert_same_group",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_convert_same_group);}},
    {"ai_convert_other_group", [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_convert_other_group);}},
    
    {"colour", [](ReligionGroup* religion_group, const Node& node){return Colour::create_from_node(node, religion_group->flags.color);}}
};

void ReligionGroup::allocate_range(const std::vector<parser::Node>& nodes)
{
    for (const parser::Node& node : nodes)
    {
        if (node.name == "secret_religion_visibility_trigger")
        {
            ; //do something
        }
        else
        {
            ReligionGroup::allocate(node);

            for (const Node& child : node.children)
            {
                if (child.type == Node::Type::BLOCK && !ReligionGroup::field_setters.contains(child.name))
                    Religion::allocate(child);
            }
                
        }
    }
}

void ReligionGroup::initalise_range(const std::vector<parser::Node>& nodes)
{
    for (const parser::Node& node : nodes)
    {
        if (node.name == "secret_religion_visibility_trigger")
        {
            ; //do something
        }
        else
        {
            ReligionGroup::initalise(node);
            //Religion::initalise_range(node.children);
        }
    }
}



template<>
enum struct Base<ReligionGroup>::DynamicFieldType
{
    NOT_SET
};

template<>
enum struct Base<Religion>::DynamicFieldType
{
    NOT_SET
};

}