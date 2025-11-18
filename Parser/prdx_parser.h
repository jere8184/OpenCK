
#pragma once

#include <string>
#include <vector>
#include <unordered_map>
#include <concepts>
#include <utility>

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
    enum struct ValueType
    {
        NOT_SET,
        STRING,
        BLOCK
    };

    Node* parent = nullptr;
    std::string name = "";
    std::string value = "";
    std::unordered_map<std::string, std::pair<Node, Token::Type>> children_map = {};
    ValueType value_type = ValueType::NOT_SET;

    void AddChild(Node& child_node, Token::Type assignment_operator)
    {
        child_node.parent = this;
        children_map[child_node.name] = {child_node, assignment_operator};
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

    bool generate_nodes(const std::string &path, std::vector<Node>& nodes);
};