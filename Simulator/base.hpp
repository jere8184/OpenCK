
#pragma once

#include <unordered_map>
#include <string>
#include <functional>
#include <print>

#include "parser/prdx_parser.h"

namespace openck::simulator
{

template <typename Derivived>
struct Base
{
    using Node = parser::Node;

    enum struct DynamicFieldType;
    static const int NOT_SET = 0;

    Base(std::string name) : name(name) {}

    bool init(const Node& node)
    {
        bool was_succes = true;
        for (const Node& child_node : node.children)
        {
            const std::string& child_name = child_node.name;
            if (Base::field_setters.contains(child_name))
            {
                if (Base::field_setters.at(child_name)(static_cast<Derivived*>(this), child_node))
                {
                    continue;
                } 
                else
                {
                    std::print(stderr, "Failed to set field: {} for: {}\n", child_name, this->name);
                    was_succes = false;
                }
            }
            else if (DynamicFieldType type = static_cast<Derivived*>(this)->get_dynamic_field_type(child_node); type != (DynamicFieldType)NOT_SET)
            {
                static_cast<Derivived*>(this)->set_dynamic_field(child_node, type);
            }
            else
            {
                std::print(stderr, "Unknown field: {} for : {}\n", child_name, this->name);
                was_succes = false;
            }
        }
        return true;
    }

    static void allocate_range(const std::vector<parser::Node>& nodes)
    {
        for (const parser::Node& node : nodes)
        {
            Base::allocate(node);
        }   
    }

    static void allocate(const parser::Node& node)
    {
        Base::map.emplace(node.name, Derivived(node.name));
    }


    static bool initalise_range(const std::vector<parser::Node>& nodes)
    {
        bool no_failures = true;

        for (const parser::Node& node : nodes)
        {
            if (initalise(node))
                continue;
            else
                no_failures = false;
        }

        return no_failures;
    }

    static bool initalise(const parser::Node& node)
    {
        return Base::map.at(node.name).init(node);
    }


    
    DynamicFieldType get_dynamic_field_type(const Node& node) {return (DynamicFieldType)NOT_SET;}

    bool set_dynamic_field(const Node& node, DynamicFieldType type) {return false;}

    static std::unordered_map<std::string, Derivived> map;
    
    using FieldSetters = std::unordered_map<std::string, std::function<bool(Derivived*, const parser::Node&)>>;
    
    static Derivived* get_by_name(const std::string& name) { return Base::map.contains(name) ? &Base::map.at(name) : nullptr; }
    
    using Id = size_t;
    
    static FieldSetters field_setters;
    
    std::string name;
    Id id;
};

template<typename Derivived>
std::unordered_map<std::string, Derivived> Base<Derivived>::map;

template<typename Derivived>
Base<Derivived>::FieldSetters Base<Derivived>::field_setters;

}