
#pragma once

#include <string>
#include <vector>
#include <unordered_map>

namespace openck::parser 
{

enum struct TokenType
{
    NOT_SET,
    QUOTED_STRING,
    STRING,
    EQUALS,
    COMMA,
    NEW_LINE,
    BLOCK_START,
    BLOCK_END
};


struct Token
{
    TokenType type;
    std::string_view text;

    Token(const TokenType& type, const std::string_view& text)
    {
        this->type = type;
        this->text = text;
    }
};


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
    std::unordered_map<std::string, Node> children_map = {};
    ValueType type = ValueType::NOT_SET;

        void AddChild(Node& child_node)
        {
            child_node.parent = this;
            children_map[child_node.name] = child_node;
        }
    };


    bool generate_nodes(const std::string &path, std::vector<Node>& nodes);
};