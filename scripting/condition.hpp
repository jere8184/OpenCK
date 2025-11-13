
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
    struct Unit;
    struct Religion;
    struct Culture;
    struct Society;
    struct Artifact;
    
    struct ReligionGroup;
}

namespace openck::scripting
{

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


    union Data
    {
        simulator::Charecter* charecter;
        simulator::Title* title;
        simulator::Province* province;
        simulator::War* war;
        simulator::Flank* flank;
        simulator::Unit* unit;
        simulator::Religion* religion;
        simulator::Culture* culture;
        simulator::Society* society;
        simulator::Artifact* artifact;
    };

    struct Functions
    {
        //simulator::Charecter* (* charecter)();
        //simulator::Title* (* title)();
        //simulator::Religion* (* religion)();
        simulator::ReligionGroup* (* religion_group)(const Scope& scope);
        bool (* controls_religion)(const Scope& scope);
    };

    using TypeBitSet = std::bitset<(size_t)Type::ENUM_SIZE>;

    Type type;
    Data data;
    Functions functions;
    Scope* parent_scope = nullptr;
};

using ComparisonOperator = bool(std::partial_ordering);

struct Target 
{
    enum class Type
    {
        BOOL,
        CHARECTER,
        RELIGION,
        RELIGION_GROUP,
        ENUM_SIZE
    };

    enum class DataLocation
    {
        NOT_SET,
        STATIC,
        FROM,
        FROMFROM,
        ROOT
    };

    struct Data
    {
        bool bool_val;
        simulator::Charecter* charecter;
        simulator::Religion* religion;
        simulator::ReligionGroup* religion_group;
    };
    
    union Function
    {
        //simulator::Charecter* (* charecter)(const Target& target);
        //simulator::Religion* (* religion)(const Target& target);
        simulator::ReligionGroup* (* religion_group)(const Target& target);
    };



    using TypeBitSet = std::bitset<(size_t)Type::ENUM_SIZE>;

    TypeBitSet type_bitset = {};
    DataLocation data_location;
    Type type;
    Data data;
    Function function;
    ComparisonOperator* comparison_function;
};


struct ICondition
{
    enum struct Type
    {
        CONDITION,
        BLOCK
    };

    ICondition(std::string name, Scope& enclosing_scope, Type type);


    bool operator()();
    
    Scope& enclosing_scope;
    std::string name;
    Type type;
};


struct Condition : ICondition
{

    using Routine = bool (Condition&);
    
    using RoutineGetter = Routine* (Condition&);
    
    Condition(parser::Node node, Scope& enclosing_scope, bool& is_success);
    
    static std::unordered_map<std::string, RoutineGetter*> routine_getters;

    Target target;
    Routine* routine;
    
    bool set_routines();

    bool operator()();
};



struct ConditionBlock : ICondition

{
    ConditionBlock(parser::Node condition_block_node, Scope& enclosing_scope, bool& is_success);

    std::vector<ICondition> conditions;
    
    bool operator()();
};


/*
    template <typename T, typename Scope, typename Target = TARGET_NOT_SET, typename Container = CONTAINER_NOT_SET>
    struct ConditionImpl : Condition
    {
        enum class Operator
        {
            NOT_SET
            EQUAL,
            NOT_EQUAL,
            CONTAIN
        };

        Condition(T (Scope::_scope_getter*)() = nullptr, T (Target::_target_getter*)() = nullptr, Container<T> (Scope::_scope_container_getter*)() = nullptr) :
            scope_getter(_scope_getter),
            target_getter(_target_getter),
            scope_container_getter(_scope_container_getter)
        {}


        std::string name;

        Scope& enclosing_scope;

        T (Scope::scope_getter*)() = nullptr;
        T (Target::target_getter*)() = nullptr;
        std::set<T> (Scope::scope_container_getter*)() = nullptr;

        bool operator()(Scope& scope, Target& target, Operator op)
        {
            case (op)
            {
                switch (Operator::EQUALS) :
                    return scope_getter(scope) == target_getter(target);
                    break;

                switch (Operator::NOT_EQUAL)
                    return scope_getter(scope) != target_getter(target);
                    break;

                switch (Operator::CONTAIN)
                    return scope_set_getter(target_getter(target))
            }
        }

        bool operator()(Scope& scope, T val, Operator op)
        {
            case (op)
            {
                switch (Operator::EQUALS) :
                    return scope_getter(scope) == val;
                    break;

                switch (Operator::NOT_EQUAL)
                    return scope_getter(scope) != val;
                    break;

                switch (Operator::CONTAIN)
                    return std::ranges::contains(scope_container_getter, val);
                    break;
            }
        }
    };

    auto controls_religion = [](auto& scope, bool b) -> bool { return scope.controls_religion() == b; };

    auto controlled_by = [](auto& scope, auto& target) -> bool { return scope.controlled_by() == target.get_charecter(); };*/
}