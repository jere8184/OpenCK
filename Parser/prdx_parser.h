
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <utility>
#include <filesystem>

namespace openck::parser 
{


struct Token
{
    enum struct Type
    {
        NOT_SET,
        QUOTED_STRING,
        STRING,
        EQUALS,
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

template<typename T>
concept Numeric = std::integral<T> || std::floating_point<T>;

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

    Node* parent = nullptr;
    std::string name = "";
    std::string value = "";
    std::unordered_map<std::string, Node> children_map = {};
    Type type = Type::NOT_SET;
    Operator op = Operator::NOT_SET;

    void AddChild(Node& child_node)
    {
        child_node.parent = this;
        children_map[child_node.name] = child_node;
    }
    
    template<Numeric Val>
    bool get_value(Val& ret) const
    {
        try
        {
            ret = std::stod(value);
        }
        catch (const std::exception& e)
        {
            return false;
        }
        return true;
    }

    bool get_value(bool& ret) const
    {
        if (this->value == "yes")
            ret = true;
        else if (this->value == "no")
            ret = false;
        else
            return false;

        return true;
    }
};

    bool generate_nodes(const std::filesystem::path &path, std::vector<Node>& nodes);
};