
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


//is the scope deucable at interpret time, if so we can eliminate switch statement and return the correct function at interpret time.

namespace openck::scripting
{

Condition::Routine* controls_religion_routine_getter(Condition& condition)
{

    switch (condition.enclosing_scope.type)
    {
        case Scope::Type::CHARACTER :
            condition.enclosing_scope.functions.controls_religion = [](const Scope& scope) -> bool
            {
                return scope.data.charecter->controls_religion();
            };
            break;
        
        case Scope::Type::TITLE :
            condition.enclosing_scope.functions.controls_religion = [](const Scope& scope) -> bool
            {
                return scope.data.title->controls_religion();
            };
            break;

        default:
            return nullptr;
            break;
    }

    switch (condition.target.type)
    {
        case Target::Type::BOOL :
            return [](Condition& condition) -> bool
            {
                bool is_head = condition.enclosing_scope.functions.controls_religion(condition.enclosing_scope);
                return condition.target.comparison_function(is_head <=> condition.target.data.bool_val);
            };
            break;
        default:
            return nullptr;
            break;
    }
};

Condition::Routine* religion_group_routine_getter(Condition& condition)
{
    switch (condition.enclosing_scope.type)
    {
        case Scope::Type::CHARACTER :
            condition.enclosing_scope.functions.religion_group = [](const Scope& scope) -> simulator::ReligionGroup*
            {
                return scope.data.charecter->get_religion()->group;
            };
            break;
        
        case Scope::Type::TITLE :
            condition.enclosing_scope.functions.religion_group = [](const Scope& scope) -> simulator::ReligionGroup*
            {
                return scope.data.title->get_religion()->group;
            };
            break;
        
        case Scope::Type::PROVINCE :
            condition.enclosing_scope.functions.religion_group = [](const Scope& scope) -> simulator::ReligionGroup*
            {
                //return scope.data.province->get_religion()->group;
                return nullptr;
            };
            break;
        
        case Scope::Type::RELIGION :
            condition.enclosing_scope.functions.religion_group = [](const Scope& scope) -> simulator::ReligionGroup*
            {
                return scope.data.religion->group;
            };
            break;

        default:
            return nullptr;
            break;
    }
    
    switch (condition.target.type)
    {
        case Target::Type::RELIGION_GROUP :
            condition.target.function.religion_group = [](const Target& target) -> simulator::ReligionGroup*
            {
                return target.data.religion_group;
            };
            break;

        case Target::Type::RELIGION :
            condition.target.function.religion_group = [](const Target& target) -> simulator::ReligionGroup*
            {
                return target.data.religion->group;
            };
            break;

        case Target::Type::CHARECTER :
            condition.target.function.religion_group = [](const Target& target) -> simulator::ReligionGroup*
            {
                return target.data.charecter->get_religion()->group;
            };
            break;
        break;

        default:
            return nullptr;
            break;
    }

    return [](Condition& condition) -> bool
    {
        simulator::ReligionGroup* scope_religion_group = condition.enclosing_scope.functions.religion_group(condition.enclosing_scope);
        simulator::ReligionGroup* target_religion_group = condition.target.function.religion_group(condition.target);
        return condition.target.comparison_function(scope_religion_group->id <=> target_religion_group->id);
    };
};


std::unordered_map<std::string, Condition::RoutineGetter*> Condition::routine_getters = 
{
    {"controls_religion", 
        controls_religion_routine_getter
    },
    {"religion_group",
        religion_group_routine_getter
    }
};

bool Condition::set_routines()
{
    if (Condition::routine_getters.contains(this->name))
    {
        Condition::RoutineGetter* routine_getter = Condition::routine_getters.at(this->name);
        
        Condition::Routine* routine = routine_getter(*this);

        this->routine = routine;

        return this->routine != nullptr;
    }
    else
    {
        return false;
    }
};


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
    if (target.type_bitset.test((size_t)Target::Type::BOOL) && node.get_value(target.data.bool_val))
    {
        target.type = Target::Type::BOOL;
        return true;
    }
    
    if (target.type_bitset.test((size_t)Target::Type::RELIGION_GROUP))
    {
        //Dummy function to get religion group from name
        /*simulator::ReligionGroup* religion_group = simulator::ReligionGroup::get_religion_group_by_name(node.value);
        if (religion_group != nullptr)
            target.type = Target::Type::RELIGION_GROUP;
            return true;*/
    }
    else
        return false;
}

Condition::Condition(parser::Node node, Scope& enclosing_scope, bool& is_success) :
    ICondition(node.name, enclosing_scope, Type::CONDITION)
{
    auto bitset_pair = valid_data_types.at(this->name);
    target.type_bitset = bitset_pair.second;

    if (!bitset_pair.first.test(size_t(this->enclosing_scope.type)))
    {
        is_success = false;
        return;
    }
    
    if (dynamic_data_location_map.contains(node.value))
    {
        target.data_location = dynamic_data_location_map.at(node.value);
    }
    else
    {
        target.data_location = Target::DataLocation::STATIC;
        if (!read_static_data(target, node) || !this->set_routines())
        {
            is_success = false;
            return;
        }
    }
}


ConditionBlock::ConditionBlock(parser::Node node, Scope& enclosing_scope, bool& is_success) : 
    ICondition(node.name, enclosing_scope, Type::BLOCK)
{
    using namespace openck::parser;

    for (const auto& [child_name, child_node] : node.children_map)
    {
        switch (child_node.value_type)
        {
        case Node::ValueType::STRING :
        {
            Condition condition(child_node, enclosing_scope, is_success);
            if (is_success == false)
                return;
            this->conditions.push_back(condition);
            break;
        }
        case Node::ValueType::BLOCK :
            this->conditions.push_back(ConditionBlock(child_node, enclosing_scope, is_success));
            if (is_success == false)
                return;
            break;

        default:
            is_success = false;
            return;
            break;
        }
    }

    is_success = true;
    return;
};

ICondition::ICondition(std::string name, Scope & enclosing_scope, Type type) :
    name(name),
    type(type),
    enclosing_scope(enclosing_scope)
{

}

}