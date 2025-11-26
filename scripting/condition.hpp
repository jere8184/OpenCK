
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

template<typename Derived>
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
        //const Scope* dynamic_data;
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


template <typename Derived>
struct Scope
{
    /*enum class Type
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

    Type type;*/

    enum struct EnumCondition;

    enum struct EnumScopeChange;

    using EnumConditionMap = std::unordered_map<std::string, EnumCondition>;

    using EnumScopeChangeMap = std::unordered_map<std::string, EnumScopeChange>;


    static EnumConditionMap condition_enum_map;

    static EnumScopeChangeMap scope_change_enum_map;

    static const int NOT_SET = 0;

    Scope* parent_scope = nullptr;

    bool operator()(const Target& target, const EnumCondition& condition) const;

    bool populate(Scope& scope, EnumScopeChange name) const
    {
        assert(false);
        return false;
    }

    static EnumCondition get_condition_enum(const parser::Node& node)
    {
        if (condition_enum_map.contains(node.name))
            return condition_enum_map.at(node.name);
        else
            return (EnumCondition)NOT_SET;
    }

    static EnumScopeChange get_scope_change_enum(const parser::Node& node)
    {
        if (scope_change_enum_map.contains(node.name))
            return scope_change_enum_map.at(node.name);
        else
            return (EnumScopeChange)NOT_SET;
    }
};

struct BloodLineScope;

struct CharacterScope : public Scope<CharacterScope>
{
    const simulator::Charecter* charecter;
    
    bool populate(BloodLineScope& blood_line_scope, EnumScopeChange name) const
    {
        return false;
    }

    using Scope::populate;

    bool controls_religion(const Target& target) const;
    bool religion_group(const Target& target) const;
    bool has_claim(const Target& target) const;
    bool has_combat(const Target& target) const;
}; 

template<>
enum struct Scope<CharacterScope>::EnumCondition
{
    NOT_SET,
    RELIGION_GROUP,
    CONTROLS_RELIGION,
};

template<>
enum struct Scope<CharacterScope>::EnumScopeChange 
{
    NOT_SET,
    ANY_OWNED_BLOODLINE
};

template<>
Scope<CharacterScope>::EnumConditionMap Scope<CharacterScope>::condition_enum_map 
{
    {"controls_religion", EnumCondition::CONTROLS_RELIGION},
    {"religion_group", EnumCondition::RELIGION_GROUP}
};

template<>
Scope<CharacterScope>::EnumScopeChangeMap Scope<CharacterScope>::scope_change_enum_map 
{
    {"any_owned_bloodline", EnumScopeChange::ANY_OWNED_BLOODLINE}
};


struct BloodLineScope : public Scope<BloodLineScope>
{
    bool operator()(const Target& target, const EnumCondition& condition) const;
};


template<>
enum struct Scope<BloodLineScope>::EnumCondition
{
    NOT_SET,
    HAS_BLOODLINE_FLAG,
    HAD_BLOODLINE_FLAG,
};

template<>
Scope<BloodLineScope>::EnumConditionMap Scope<BloodLineScope>::condition_enum_map 
{
    {"has_bloodline_flag", EnumCondition::HAS_BLOODLINE_FLAG},
    {"had_bloodline_flag", EnumCondition::HAD_BLOODLINE_FLAG}
};

template<>
Scope<BloodLineScope>::EnumScopeChangeMap Scope<BloodLineScope>::scope_change_enum_map 
{

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


    typename ScopeType::EnumCondition condition_enum;
    
    Target target;
    
    Condition(parser::Node node, bool& is_success) :
        IConditionType(node.name, IConditionType::Type::CONDITION), target(node, is_success), condition_enum(ScopeType::get_condition_enum(node))
    {

    }

    ~Condition() = default;


    //looping over list of ICondition* -> calling virtual bool operator()(); -> function pointer call -> virtual function call -> function pointer call = current approch
                                            //vs.
    //looping over list of ICondition -> switch statment vs static calling bool operator()(); -> switch-statement + function call -> switch-statement + function call -> switch-statment + function call

    virtual bool operator()(const ScopeType& scope) const override
    {
        return scope(this->target, this->condition_enum);
    }
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
    

    Type block_type;
    std::vector<std::unique_ptr<ICondition<ScopeType>>> conditions;

    template <typename Deriveved = ConditionBlock, typename ChildScopeType = ScopeType>
    ConditionBlock(const parser::Node& node, bool& is_success, std::type_identity<Deriveved> = {}, std::type_identity<ChildScopeType> = {}) : 
        ICondition<ScopeType>(node.name, ICondition<ScopeType>::Type::BLOCK)
    {
        using namespace openck::parser;

        for (const Node& child_node : node.children)
        {
            const std::string& child_name = child_node.name;

            switch (child_node.type)
            {
                case Node::Type::STRING :
                {
                    std::unique_ptr<ICondition<ChildScopeType>> condition = std::make_unique<Condition<ChildScopeType>>(child_node, is_success);
                    if (is_success == false)
                        return;
                    
                    static_cast<Deriveved*>(this)->conditions.emplace_back(std::move(condition));
                    break;
                }
                case Node::Type::BLOCK :
                {
                    std::unique_ptr<ICondition<ChildScopeType>> condition_block = condition_block_factory<ChildScopeType>(child_node, is_success);
                    if (is_success == false)
                        return;
                    
                    static_cast<Deriveved*>(this)->conditions.emplace_back(std::move(condition_block));
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

    template<typename ChildScopeType>
    std::unique_ptr<ICondition<ChildScopeType>> condition_block_factory(const parser::Node& node, bool& is_success);

    virtual bool operator()(const ScopeType& scope) const override {return false;};
};


template <typename ScopeType>
struct OrBlock : public ConditionBlock<ScopeType>
{
    OrBlock(const parser::Node& node, bool& is_success) : 
        ConditionBlock<ScopeType>(node, is_success) {}
    
    virtual bool operator()(const ScopeType& scope) const override 
    {
        for (const std::unique_ptr<ICondition<ScopeType>>& condition : this->conditions)
        {
            if ((*condition)(scope))
                return true;
        }
        return false;
    };
};


template <typename ScopeType>
struct AndBlock : public ConditionBlock<ScopeType>
{
    AndBlock(const parser::Node& node, bool& is_success) : 
        ConditionBlock<ScopeType>(node, is_success) {}

    virtual bool operator()(const ScopeType& scope) const override 
    {
        for (const std::unique_ptr<ICondition<ScopeType>>& condition : this->conditions)
        {
            if (!(*condition)(scope))
                return false;
        }
        return true;
    };
};


template <typename ScopeType>
struct NotBlock : public ConditionBlock<ScopeType>
{
    NotBlock(const parser::Node& node, bool& is_success) : 
        ConditionBlock<ScopeType>(node, is_success) {}

    virtual bool operator()(const ScopeType& scope) const override 
    {
        for (const std::unique_ptr<ICondition<ScopeType>>& condition : this->conditions)
        {
            if ((*condition)(scope))
                return false;
        }
        return true;
    };
};


template <typename ScopeType, typename ChildScopeType>
struct ScopeChangeBlock : public ConditionBlock<ScopeType>
{
    std::vector<std::unique_ptr<ICondition<ChildScopeType>>> conditions;
    ScopeType::EnumScopeChange scope_change_name;


    ScopeChangeBlock(const parser::Node& node, bool& is_success) : 
        ConditionBlock<ScopeType>(node, is_success, std::type_identity<ScopeChangeBlock>{}, std::type_identity<ChildScopeType>{}) 
    {
        this->scope_change_name = ScopeType::get_scope_change_enum(node);
    }


    virtual bool operator()(const ScopeType& scope) const override 
    {
        ChildScopeType child_scope;
        while (scope.populate(child_scope, this->scope_change_name))
        {
            bool is_success = true;
            for (const std::unique_ptr<ICondition<ChildScopeType>>& condition : this->conditions)
            {
                if (!(*condition)(child_scope))
                {
                    is_success = false;
                    break;
                }
            }
            if (is_success == true)
                return true;
        }
        return false;
    };
};


template <typename ScopeType>
inline ConditionBlock<ScopeType> &ConditionBlock<ScopeType>::operator=(ConditionBlock &&condition_block)
{
    this->block_type = condition_block.block_type;
    this->conditions = std::move(condition_block.conditions);
    this->name = std::move(condition_block.name);
    this->type = condition_block.type;

    return *this;
}


template <typename ScopeType>
template <typename ChildScopeType>
std::unique_ptr<ICondition<ChildScopeType>> ConditionBlock<ScopeType>::condition_block_factory(const parser::Node& node, bool& is_success)
{
    if (node.name == "NOT")
        return std::make_unique<NotBlock<ChildScopeType>>(node, is_success);
    else if (node.name == "AND")
        return std::make_unique<AndBlock<ChildScopeType>>(node, is_success);
    else if (node.name == "OR")
        return std::make_unique<OrBlock<ChildScopeType>>(node, is_success);
    else if (node.name == "any_owned_bloodline")
        return std::make_unique<ScopeChangeBlock<ScopeType, ChildScopeType>>(node, is_success);
    else
        return std::make_unique<AndBlock<ChildScopeType>>(node, is_success);
}

}