

#include "religion.hpp"
#include "culture.hpp"

namespace openck::simulator
{

    template<>
    Base<ReligionGroup>::FieldSetters Base<ReligionGroup>::field_setters = 
    {
        {"has_coa_on_barony_only",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.has_coa_on_barony_only);}},
        {"playable",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.playable);}},
        
        {"crusade_cb",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.crusade_cb);}},
        {"graphical_culture", [](ReligionGroup* religion_group, const Node& node){Culture* culture = Culture::get_by_name(node.name); religion_group->flags.graphical_culture = culture; return culture != nullptr;}},
        
        {"ai_peaceful",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_peaceful);}},
        {"ai_convert_same_group",  [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_convert_same_group);}},
        {"ai_convert_other_group", [](ReligionGroup* religion_group, const Node& node){return node.get_value(religion_group->flags.ai_convert_other_group);}}
    };

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