
#pragma once

#include "parser/prdx_parser.h"

#include <string>
#include <algorithm>
#include <ranges>
#include <vector>
#include <optional>
#include <unordered_map>
#include <functional>
#include <bitset>
#include <stack>
#include <cassert>
#include <memory>

namespace openck::parser
{
    struct Node;
}

namespace openck::simulator
{
    struct Charecter;
    struct Title;
    struct Province;
    struct War;
    struct Flank;
    struct UnitType;
    struct Trait;
    struct Religion;
    struct Culture;
    struct Society;
    struct Artifact;
    struct ReligionGroup;
}

namespace openck::scripting
{

struct Scope;

struct Target 
{
    struct has_portrait_property 
    {
        int layer;
        int index;
    };

    struct holding_diff 
    {
        bool first_count_vassals;
    };

    union Clause
    {
        has_portrait_property has_portrait_property;
        holding_diff holding_diff;
    };

    union Data //if data location not static, this has to be populated at run time
    {
        bool bool_val;
        const simulator::Charecter* charecter;
        const simulator::Religion* religion;
        const simulator::ReligionGroup* religion_group;
        const simulator::Title* title;
        const simulator::Trait* trait;
        const Scope* dynamic_data;
        int id;
        Clause clause;
    };

    enum class Type
    {
        NOT_SET,
        BOOL,
        CHARECTER,
        RELIGION,
        RELIGION_GROUP,
        Trait
    };

    enum class DataLocation
    {
        NOT_SET,
        STATIC,
        FROM,
        FROMFROM,
        ROOT
    };

    Target(const parser::Node& node, bool& is_success);

    bool read_static_data(const openck::parser::Node& node);

    using ComparisonOperator = bool(std::partial_ordering);
    
    ComparisonOperator* comparison_function;
    DataLocation data_location;
    Type type;
    Data data;
};

static std::unordered_map<std::string, Target::DataLocation> dynamic_data_location_map = 
{
    {"FROM", Target::DataLocation::FROM},
    {"FROMFROM", Target::DataLocation::FROMFROM},
    {"ROOT", Target::DataLocation::ROOT}
};

bool read_static_data(Target& target, const openck::parser::Node& node);

struct Scope
{
    enum class Type
    {
        CHARACTER,
        TITLE,
        PROVINCE,
        WAR,
        FLANK,
        UNIT,
        RELIGION,
        CULTURE,
        SOCIETY,
        ARTIFACT,
        ENUM_SIZE
    };

    Type type;
    Scope* parent_scope = nullptr;

    //if i keep going down this road we will have 700 virt functions coresponding to each condition.
    
    //instead when Condition has its operator called it switches on Scope::Type, union of scopes to get correct subclass (n > 10)
    //calls correct subclass method based on large condition switch, which you pass the target to.
    //
    /*bool adventurer(const Target&) const { assert(false); return false; };
    bool age(const Target&) const { assert(false); return false; };
    bool adventurer(const Target&) const { assert(false); return false; };
    bool character(const Target&) const { assert(false); return false; };
    bool claimed_by(const Target&) const { assert(false); return false; };
    bool religion_group(const Target&) const { assert(false); return false; }
    bool controls_religion(const Target&) const { assert(false); return false; }
    bool de_jure_liege(const Target&) const { assert(false); return false; }*/

};


struct CharacterScope : public Scope
{
    enum struct ConditionName
    {
        NOT_SET,
        RELIGION_GROUP,
        CONTROLS_RELIGION,
    };


    CharacterScope()
    {
        this->type = Scope::Type::CHARACTER;
    }

    static ConditionName get_condition_name(const parser::Node& node);

    //virtual int age() const override;
    bool controls_religion(const Target& target) const;
    bool religion_group(const Target& target) const;
    bool has_claim(const Target& target) const;
    bool has_combat(const Target& target) const;

    bool operator()(const Target& target, const ConditionName& condition) const;

    const simulator::Charecter* charecter;
}; 


template<typename ScopeType> //this means scope cannot change within a condition block, I think this is true
struct ICondition
{
    enum struct Type
    {
        CONDITION,
        BLOCK
    };


    ICondition(const std::string& name, Type type) :
        name(name),
        type(type)
    {

    }
    
    virtual ~ICondition() = default;

    std::string name;
    Type type;
    
    virtual bool operator()(const ScopeType& scope) const = 0;
};


template<typename ScopeType>
struct Condition : ICondition<ScopeType>
{
    using IConditionType = ICondition<ScopeType>;

    Condition(parser::Node node, bool& is_success) :
        IConditionType(node.name, IConditionType::Type::CONDITION), target(node, is_success), condition_name(ScopeType::get_condition_name(node))
    {

    }

    ~Condition() = default;


    //looping over list of ICondition* -> calling virtual bool operator()(); -> function pointer call -> virtual function call -> function pointer call = current approch
                                            //vs.
    //looping over list of ICondition -> switch statment vs static calling bool operator()(); -> switch-statement + function call -> switch-statement + function call -> switch-statment + function call

    bool operator()(const ScopeType& scope) const
    {
        return scope(this->target, condition_name);
    }

    typename ScopeType::ConditionName condition_name;
    
    Target target;
};

template<typename ScopeType>
struct ConditionBlock : ICondition<ScopeType>
{
    enum struct Type
    {
        NOT_SET,
        NOT,
        OR,
        AND
    };
    
    using ConditionBlockType = ConditionBlock<ScopeType>;
    using IConditionType = ICondition<ScopeType>;
    using ConditionType = Condition<ScopeType>;

    Type block_type;
    std::vector<std::unique_ptr<IConditionType>> conditions;

    ConditionBlock(parser::Node node, bool& is_success) : 
        IConditionType(node.name, IConditionType::Type::BLOCK)
    {
        using namespace openck::parser;

        for (const Node& child_node : node.children)
        {
            const std::string& child_name = child_node.name;

            switch (child_node.type)
            {
                case Node::Type::STRING :
                {
                    std::unique_ptr<IConditionType> condition = std::make_unique<ConditionType>(child_node, is_success);
                    if (is_success == false)
                        return;
                    
                    this->conditions.emplace_back(std::move(condition));
                    break;
                }
                case Node::Type::BLOCK :
                {
                    std::unique_ptr<IConditionType> condition_block = std::make_unique<ConditionBlockType>(child_node, is_success);
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

    ~ConditionBlock() = default;

    ConditionBlock(ConditionBlock&&) = default;

    ConditionBlock& operator=(ConditionBlock&& condition_block);

    virtual bool operator()(const ScopeType& scope) const override {return false;};
};

template <typename ScopeType>
inline ConditionBlock<ScopeType> &ConditionBlock<ScopeType>::operator=(ConditionBlock &&condition_block)
{
    this->block_type = condition_block.block_type;
    this->conditions = std::move(condition_block.conditions);
    this->name = std::move(condition_block.name);
    this->type = condition_block.type;
}

}