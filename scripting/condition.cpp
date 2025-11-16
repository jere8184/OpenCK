
#include "condition.hpp"

#include "parser/prdx_parser.h"

#include "simulator/charecter.hpp"

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

bool Condition::religion_group(const Scope& scope)
{
    simulator::ReligionGroup* scope_religion_group = scope.religion_group();
    simulator::ReligionGroup* target_religion_group = this->target.data.religion_group;
    return this->target.comparison_function(scope_religion_group <=> target_religion_group);
}

bool Condition::controls_religion(const Scope& scope)
{
    bool is_head = scope.controls_religion();
    return this->target.comparison_function(is_head <=> this->target.data.bool_val);
}


std::unordered_map<std::string, Target::DataLocation> dynamic_data_location_map = 
{
    {"FROM", Target::DataLocation::FROM},
    {"FROMFROM", Target::DataLocation::FROMFROM},
    {"ROOT", Target::DataLocation::ROOT}
};

std::unordered_map<std::string, std::pair<Scope::TypeBitSet, Target::TypeBitSet>> valid_data_types =
{
    {"controls_religion", 
        {Scope::TypeBitSet().flip((size_t)Scope::Type::TITLE).flip((size_t)Scope::Type::CHARACTER),
        Target::TypeBitSet().flip((size_t)Target::Type::BOOL)}
    },
    {"religion_group",
        {Scope::TypeBitSet().flip((size_t)Scope::Type::TITLE).flip((size_t)Scope::Type::CHARACTER).flip((size_t)Scope::Type::PROVINCE).flip((size_t)Scope::Type::RELIGION),
        Target::TypeBitSet().flip((size_t)Target::Type::RELIGION_GROUP).flip((size_t)Target::Type::RELIGION).flip((size_t)Target::Type::CHARECTER)}

    }
};

bool read_static_data(Target& target, const openck::parser::Node& node)
{
    if (node.get_value(target.data.bool_val))
    {
        target.type = Target::Type::BOOL;
        return true;
    }
    else if (simulator::ReligionGroup* religion_group = simulator::ReligionGroup::get_religion_group_by_name(node.value))
    {
        target.type = Target::Type::RELIGION_GROUP;
        target.data.religion_group = religion_group;
        return true;
    }
    else if (simulator::Religion* religion = simulator::Religion::get_religion_by_name(node.value))
    {
        target.type = Target::Type::RELIGION;
        target.data.religion = religion;
        return true;
    }
    else
    {
        return false;
    }
    
    //if (target.type_bitset.test((size_t)Target::Type::RELIGION_GROUP))
    {
        //Dummy function to get religion group from name
        /*simulator::ReligionGroup* religion_group = simulator::ReligionGroup::get_religion_group_by_name(node.value);
        if (religion_group != nullptr)
            target.type = Target::Type::RELIGION_GROUP;
            return true;*/
    }
    return false;
}

Condition::Condition(parser::Node node, bool& is_success) :
    ICondition(node.name, Type::CONDITION)
{
    if (dynamic_data_location_map.contains(node.value))
    {
        target.data_location = dynamic_data_location_map.at(node.value);
    }
    else
    {
        target.data_location = Target::DataLocation::STATIC;
        if (!read_static_data(target, node))
        {
            is_success = false;
            return;
        }
    }
    //need to set routine to call here

}


ConditionBlock::ConditionBlock(parser::Node node, bool& is_success) : 
    ICondition(node.name, ICondition::Type::BLOCK)
{
    using namespace openck::parser;

    for (const auto& [child_name, child_node_and_operator] : node.children_map)
    {
        const auto& [child_node, assignment_op] = child_node_and_operator;
        
        switch (child_node.value_type)
        {
            case Node::ValueType::STRING :
            {
                std::unique_ptr<ICondition> condition = std::make_unique<Condition>(child_node, is_success);
                if (is_success == false)
                    return;
                
                this->conditions.emplace_back(std::move(condition));
                break;
            }
            case Node::ValueType::BLOCK :
            {
                std::unique_ptr<ICondition> condition_block = std::make_unique<ConditionBlock>(child_node, is_success);
                if (is_success == false)
                    return;
                
                this->conditions.emplace_back(std::move(condition_block));
                break;
            }
            default:
                is_success = false;
                return;
                break;
        }
    }

    is_success = true;
    return;
}

bool ConditionBlock::run_not_block(const Scope& scope) const
{
    bool ret_val = true;
    for (const auto& condition : this->conditions)
    {
        if ((*condition)(scope))
            ret_val = false;
    }

    return ret_val;
}

bool ConditionBlock::operator()(const Scope& scope) const
{ //NOT
    switch (this->block_type)
    {
    case Type::NOT :
        return run_not_block(scope);
        break;
    
    default:
        break;
    }
}

ICondition::ICondition(const std::string& name, Type type) :
    name(name),
    type(type)
{

}

int CharacterScope::age() const
{
    return charecter->age;
}

}