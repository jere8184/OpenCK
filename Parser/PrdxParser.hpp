
#pragma once

#include "Utils/StatusCode.hpp"
#include "Utils/Utils.hpp"

#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <utility>
#include <filesystem>
#include <cstddef>

namespace openck::parser
{

struct Token
{
	enum struct Type
	{
		NOT_SET,
		QUOTED_STRING,
		STRING,
		EQUAL,
		LESS,
		COMMA,
		NEW_LINE,
		BLOCK_START,
		BLOCK_END
	};

	Type type;
	std::string_view text;

	Token(const Type& type, const std::string_view& text)
	{
		this->type = type;
		this->text = text;
	}
};

template<typename ScopeType>
concept Numeric = std::integral<ScopeType> || std::floating_point<ScopeType>;

struct Node
{
	enum struct Type
	{
		NOT_SET,
		STRING,
		BLOCK
	};

	enum struct Operator
	{
		NOT_SET,
		EQUALS,
		LESS
	};

	Node() = default;

	Node(Node&& other) :
		parent(std::move(other.parent)),
		name(std::move(other.name)),
		value(std::move(other.value)),
		type(std::move(other.type)),
		op(std::move(other.op)),
		debugging(std::move(other.debugging)),
		children(std::move(other.children))
	{
		for (Node& child : children)
		{
			child.parent = this;
		}
	}

	const Node* parent = nullptr;
	std::string name = "";
	std::string value = "";

	std::vector<Node> children;

	Type type = Type::NOT_SET;
	Operator op = Operator::NOT_SET;

	void add_child(Node& child_node)
	{
		child_node.parent = this;
		this->children.push_back(std::move(child_node));
	}

	template<Numeric Val>
	StatusCode get_value(Val& ret) const
	{
		try
		{
			ret = std::stod(value);
		}
		catch (const std::exception& e)
		{
			return StatusCode::FAILURE;
		}
		return StatusCode::SUCCESS;
	}

	StatusCode get_value(bool& ret) const
	{
		if (this->value == "yes")
			ret = true;
		else if (this->value == "no")
			ret = false;
		else
			return StatusCode::FAILURE;

		return StatusCode::SUCCESS;
	}

	StatusCode get_child_by_name(const Node*& node, const std::string& name) const
	{
		for (const Node& child : children)
		{
			if (child.name == name)
			{
				node = &child;
				return StatusCode::SUCCESS; 
			}
		}

		return StatusCode::NOT_FOUND;
	}

	std::string path_to_string() const
	{
		std::string path;
		if (this->parent)
			path += this->parent->path_to_string() + ".";
		
		return path + this->name;
	}

	struct Debugging
	{
		StatusCode statusCode;
		std::string action_name;

		enum::StatusCode store_and_forward_result(enum::StatusCode StatusCode, const std::string& action_name = "")
		{
			this->statusCode = StatusCode;
			this->action_name = action_name;
			return StatusCode;
		}

		template <bool REPORT_CHILD_ERRORS>
		static std::string report_error(const Node& node, int depth = 1)
		{
			std::string error = "";
			if (StatusCode::SUCCESS != node.debugging.statusCode)
			{
				if (node.debugging.action_name.size())
				{
					if (node.value.size())
						error = std::format("<{}> for <{} = {}>, failed with <{}>", node.debugging.action_name, node.path_to_string(), node.value, ToString(node.debugging.statusCode));
					else
						error = std::format("<{}> for <{}>, failed with <{}>", node.debugging.action_name, node.path_to_string(), ToString(node.debugging.statusCode));

				}
				else if (node.value.size())
				{
					error = std::format("<{} = {}> has status code <{}>", node.path_to_string(), node.value, ToString(node.debugging.statusCode));
				}
				else
				{
					error = std::format("<{}> has status code <{}>", node.path_to_string(), ToString(node.debugging.statusCode));
				}
			}

			if constexpr(REPORT_CHILD_ERRORS)
			{
				for (const Node& child : node.children)
				{
					std::string indentation(depth, '\t');
					std::string child_error = report_error<true>(child, depth + 1);
					if (child_error.size())
						error += "\n" + indentation + child_error;
				}
			}

			return error;
		}
	};

	mutable Debugging debugging;
};

bool generate_nodes(const std::filesystem::path &path, std::vector<Node>& nodes);

};