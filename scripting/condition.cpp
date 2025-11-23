
#include "condition.hpp"

#include "parser/prdx_parser.h"

#include "simulator/charecter.hpp"
#include "simulator/traits.hpp"

#include <string>
#include <unordered_map>
#include <map>
#include <utility>
#include <compare>
#include <cassert>
#include <stack>
#include <memory>

namespace openck::scripting
{

bool CharacterScope::operator()(const Target & target, const ConditionName & condition) const
{
    switch (condition)
    {
    case ConditionName::CONTROLS_RELIGION :
        return this->controls_religion(target);
        break;

    case ConditionName::RELIGION_GROUP :
        return false;

    default:
        assert(false);
        return false;
        break;
    }
    assert(false);
}

CharacterScope::ConditionName CharacterScope::get_condition_name(const parser::Node& node)
{
    static std::unordered_map<std::string, ConditionName> name_map
    {
        {"controls_religion", ConditionName::CONTROLS_RELIGION},
        {"religion_group", ConditionName::RELIGION_GROUP}
    };

    if (name_map.contains(node.name))
        return name_map.at(node.name);
    else
        return ConditionName::NOT_SET;
}

bool CharacterScope::controls_religion(const Target &target) const
{
    return charecter->controls_religion() == target.data.bool_val;
}

bool CharacterScope::religion_group(const Target & target) const
{
    return charecter->get_religion()->group == target.data.religion_group;
}

bool CharacterScope::has_claim(const Target & target) const
{
    return charecter->all_claims.contains(target.data.title);
}

bool CharacterScope::has_combat(const Target & target) const
{
    return false;
}

Target::Target(const parser::Node & node, bool& is_success)
{
    if (dynamic_data_location_map.contains(node.value))
    {
        this->data_location = dynamic_data_location_map.at(node.value);
    }
    else
    {
        this->data_location = Target::DataLocation::STATIC;
        if (!this->read_static_data(node))
        {
            is_success = false;
            return;
        }
    }
}

bool Target::read_static_data(const openck::parser::Node& node)
{

    //one approch is for each condition, we have a bit set of valid target values and use this to chose what data to attempt to load.
    if (node.get_value(this->data.bool_val))
    {
        this->type = Target::Type::BOOL;
    }
    else if (simulator::ReligionGroup* religion_group = simulator::ReligionGroup::get_by_name(node.value))
    {
        this->type = Target::Type::RELIGION_GROUP;
        this->data.religion_group = religion_group;
    }
    else if (simulator::Religion* religion = simulator::Religion::get_by_name(node.value))
    {
        this->type = Target::Type::RELIGION;
        this->data.religion = religion;
    }
    else if (const simulator::Trait* trait = simulator::Trait::get_by_name(node.value))
    {
        this->type = Target::Type::Trait;
        this->data.trait = trait;
    }
    else
    {
        return false;
    }

    return true;
    
    //if (target.type_bitset.test((size_t)Target::Type::RELIGION_GROUP))
    {
        //Dummy function to get religion group from name
        /*simulator::ReligionGroup* religion_group = simulator::ReligionGroup::get_religion_group_by_name(node.value);
        if (religion_group != nullptr)
            target.type = Target::Type::RELIGION_GROUP;
            return true;*/
    }
}

}