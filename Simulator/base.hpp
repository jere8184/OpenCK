
#pragma once

#include "utils/status_code.hpp"
#include "parser/prdx_parser.h"

#include <unordered_map>
#include <string>
#include <functional>
#include <print>

namespace openck::simulator
{

template <typename Derived>
struct Base
{
	using Node = parser::Node;

	enum struct DynamicFieldType;
	static const int NOT_SET = 0;

	Base(std::string name) : name(name)
	{}

	bool init(const Node& node)
	{
		bool was_succes = true;
		for (const Node& child : node.children)
		{
			if (field_setters.contains(child.name))
			{
				// JM: we need to eventually error on StatusCode::NOT_IMPLIMENTED
				if (StatusCode statusCode = child.debugging.store_and_forward_result(
						field_setters.at(child.name)(static_cast<Derived*>(this), child),
						"set field"
					);
					StatusCode::SUCCESS == statusCode || StatusCode::NOT_IMPLIMENTED == statusCode)
				{
					;
				}
				else
				{
					//std::println(stderr, "set field <{}> for <{}>, failed with <{}>", child.name, this->name, status_code_to_string(statusCode));
					std::println(stderr, "{}", Node::Debugging::report_error<true>(child));
					was_succes = false;
				}
			}
			else if (DynamicFieldType type = static_cast<Derived*>(this)->get_dynamic_field_type(child); type != (DynamicFieldType)NOT_SET)
			{
				static_cast<Derived*>(this)->set_dynamic_field(child, type);
			}
			else
			{
				if (child.value.size())
					std::println(stderr, "Unknown field <{} = {}> for : <{}>", child.path_to_string(), child.value, this->name);
				else
					std::println(stderr, "Unknown field <{}> for : <{}>", child.path_to_string(), this->name);
				was_succes = false;
			}
		}
		return was_succes;
	}

	static void allocate_range(const std::vector<Node>& nodes)
	{
		for (const Node& node : nodes)
		{
			allocate(node);
		}
	}

	static void allocate(const Node& node)
	{
		map.emplace(node.name, Derived(node.name));
	}

	static bool initalise_range(const std::vector<Node>& nodes)
	{
		bool no_failures = true;


		for (const Node& node : nodes)
		{
			if (initalise(node))
				continue;
			else
				no_failures = false;
		}

		return no_failures;
	}

	static bool initalise(const Node& node)
	{
		return map.at(node.name).init(node);
	}

	static void initalise_static_fields()
	{
		Derived::init_field_setters();
		Derived::init_static_objects();
	}

	DynamicFieldType get_dynamic_field_type(const Node& node) {return (DynamicFieldType)NOT_SET;}

	StatusCode set_dynamic_field(const Node& node, DynamicFieldType type) {return StatusCode::NOT_IMPLIMENTED;}

	static StatusCode get_by_name(const Derived*& derived, const std::string& name)
	{
		if (map.contains(name))
		{
			derived = &map.at(name);
			return StatusCode::SUCCESS;
		}
		else
		{
			return StatusCode::NOT_FOUND;
		}
	}

	using Id = size_t;

	//maps node names to their functions
	using FieldSetters = std::unordered_map<std::string, std::function<StatusCode(Derived*, const Node&)>>;

	static FieldSetters field_setters;
	static std::unordered_map<std::string, Derived> map;
	std::string name;
	Id id;
};


template<typename Derived>
std::unordered_map<std::string, Derived> Base<Derived>::map;

template<typename Derived>
Base<Derived>::FieldSetters Base<Derived>::field_setters;

}