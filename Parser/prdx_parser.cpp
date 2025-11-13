
#include <string>
#include <print>
#include <vector>
#include <unordered_map>
#include <stack>

#include "prdx_parser.h"

namespace openck::parser
{
//Returns number of tokens written.
void tokenise_text(const std::string& source, std::vector<Token>& tokens)
{ 
    size_t i = 0;
    size_t length = 0;

    while (i < source.length())
    {
        switch (source[i])
        {
            case '\t':
            case ' ':
                break;

            case '\n':
                tokens.emplace_back(TokenType::NEW_LINE, std::string_view(&source[i], 1));
                break;
           
            case '#':
                while (source[i] != '\n' && source[i] != '\0')
                {
                    i++;
                }
                break;

            case '=':
                tokens.emplace_back(TokenType::EQUALS, std::string_view(&source[i], 1));
                break;

            case '{':
                tokens.emplace_back(TokenType::BLOCK_START, std::string_view(&source[i], 1));
                break;
    
            case '}':
                tokens.emplace_back(TokenType::BLOCK_END, std::string_view(&source[i], 1));
                break;

            case ',':
                tokens.emplace_back(TokenType::COMMA, std::string_view(&source[i], 1));
                break;

            case '"':
                i++;
                length = 0;
                while (source[i] != '"')
                {
                    i++;
                    length++;

                    if (source[i] == '\n' || source[i] == '\0')
                    {
                        std::puts("unterminated quote");
                        break;
                    }
                }
                if (length)
                    tokens.emplace_back(TokenType::QUOTED_STRING, std::string_view(&source[i-length], length));
                break;

            default:
                length = 0;
                if (source[i] == '+' || source[i] == '-')
                {
                    length++;
                    i++;
                }

                while (isalnum(source[i]) || source[i] == '_' || source[i] ==  '.')
                {
                    length++;
                    i++;
                }

                if (length)
                    tokens.emplace_back(TokenType::STRING, std::string_view(&source[i-length], length));
                
                if (source[i] == '\n')
                    tokens.emplace_back(TokenType::NEW_LINE, std::string_view(&source[i], 1));

        }
        i++;
    }
    return;
}

bool create_nodes(const std::vector<Token>& tokens, std::vector<Node>& root_nodes)
{
    std::stack<std::shared_ptr<Node>> scope_stack;
    std::shared_ptr<Node> current_node = nullptr;
    std::shared_ptr<Node> completed_block = nullptr;
    TokenType previous_token_type = TokenType::NOT_SET;

    int node_count = 0; // for debugging
    int token_count = 0;
    for (const Token& token : tokens)
    {
        token_count++;
        switch (token.type)
        {
        case TokenType::STRING:
            if(previous_token_type == TokenType::EQUALS)
            {
                current_node->value = token.text;
                current_node->value_type = Node::ValueType::STRING;
                if (scope_stack.size())
                {
                    scope_stack.top()->AddChild(*current_node);
                    current_node.reset();
                }
                current_node = nullptr;
            }
            else
            {
                node_count++;
                current_node = std::make_shared<Node>();
                current_node->name = token.text;
            }
            previous_token_type = TokenType::STRING;
            break;

        case TokenType::QUOTED_STRING:
            if (current_node && previous_token_type == TokenType::EQUALS)
            {
                current_node->value = token.text;
                current_node->value_type = Node::ValueType::STRING;
                if (scope_stack.size())
                {
                    scope_stack.top()->AddChild(*current_node);
                    current_node.reset();
                }
                current_node = nullptr;
            }
            else
            {
                return false;
            }
            previous_token_type = TokenType::QUOTED_STRING;
            break;

        case TokenType::EQUALS:
            if (!current_node)
                return false;
            previous_token_type = TokenType::EQUALS;
            break;

        case TokenType::NEW_LINE:
            if (current_node && scope_stack.size())
            {
                scope_stack.top()->AddChild(*current_node);
                current_node.reset();
            }

            previous_token_type = TokenType::NEW_LINE;
            break;

        case TokenType::BLOCK_START:
            if (current_node && previous_token_type == TokenType::EQUALS)
            {
                current_node->value_type = Node::ValueType::BLOCK;
                scope_stack.push(current_node);
                current_node.reset();
            }
            else
            {
                return false;
            }
            previous_token_type = TokenType::BLOCK_START;
            break;

        case TokenType::BLOCK_END:
            completed_block = scope_stack.top();
            scope_stack.pop();
            if (scope_stack.size() == 0)
            {
                root_nodes.push_back(std::move(*completed_block));
            }
            else
            {
                scope_stack.top()->AddChild(*completed_block);
            }
            completed_block.reset();
            previous_token_type = TokenType::BLOCK_END;
            break;

        default:
            break;
        }
    }
    return true;
}


std::string read_file(const std::string& path)
{
    FILE* File;
    fopen_s(&File, path.c_str(), "r");
    if (File == nullptr)
    {
        std::print(stderr, "Could not open file \"{}\"", path); 
        return "";
    }

    fseek(File, 0, SEEK_END);
    size_t file_size = ftell(File);
    rewind(File);

    std::string file_contents;
    file_contents.resize(file_size);

    size_t bytes_read = std::fread(file_contents.data(), sizeof(char), file_size, File);
    file_contents.resize(bytes_read);

    return file_contents;
}


bool generate_nodes(const std::string &path, std::vector<Node>& nodes)
{
    bool res = true;

    const std::string& file_contents = read_file(path.c_str());
    
    std::vector<Token> tokens;
    tokenise_text(file_contents, tokens);

    nodes.reserve(tokens.size());

    res = create_nodes(tokens, nodes);

    return 0;
}

} //openck