
#include <string>
#include <print>
#include <vector>
#include <unordered_map>
#include <stack>

namespace openck
{

enum struct TokenType
{
    NOT_SET,
    QUOTED_STRING,
    STRING,
    EQUALS,
    COMMA,
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
    std::vector<Node*> children_vector = {};
    ValueType type = ValueType::NOT_SET;

    void AddChild(Node& child_node)
    {
        child_node.parent = this;
        children_vector.push_back(&(children_map[child_node.name] = child_node));
    }
};

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
            case '\n':
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

        }
        i++;
    }
    return;
}

bool create_nodes(const std::vector<Token>& tokens, std::vector<Node>& root_nodes)
{
    std::stack<Node*> scope_stack;
    Node* current_node = nullptr;

    bool was_last_token_equals = false;

    int node_count = 0; // for debugging
    int token_count = 0;
    for (const Token& token : tokens)
    {
        token_count++;
        switch (token.type)
        {
        case TokenType::STRING:
            if(was_last_token_equals)
            {
                current_node->value = token.text;
                current_node->type = Node::ValueType::STRING;
                if (scope_stack.size())
                {
                    scope_stack.top()->AddChild(*current_node);
                }
                current_node = nullptr;
                was_last_token_equals = false;
            }
            else
            {
                node_count++;
                nodes.emplace_back(Node());
                current_node = &nodes.back();
                current_node->name = token.text;
            }
            break;

        case TokenType::QUOTED_STRING:
            if (current_node && was_last_token_equals)
            {
                current_node->value = token.text;
                current_node->type = Node::ValueType::STRING;

                if (scope_stack.size())
                    scope_stack.top()->AddChild(*current_node);
                
                current_node = nullptr;
                was_last_token_equals = false;
            }
            else
            {
                return false;
            }
            break;

        case TokenType::EQUALS:
            if (current_node)
                was_last_token_equals = true;
            else
                return false;
            break;

        case TokenType::BLOCK_START:
            if (current_node && was_last_token_equals)
            {
                current_node->type = Node::ValueType::BLOCK;

                if (scope_stack.size())
                {
                    scope_stack.top()->AddChild(*current_node);
                }

                scope_stack.push(current_node);
                current_node = nullptr;
                was_last_token_equals = false;
            }
            else
            {
                return false;
            }
            break;

        case TokenType::BLOCK_END:
            scope_stack.pop();
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
        fprintf(stderr, "Could not open file \"%s\"", path); 
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

    nodes.reserve(tokens.size()); //we dont want 'nodes' to be resized during 'create_nodes()' as it leads to pointer invalidation.

    res = create_nodes(tokens, nodes);

    return 0;
}

} //openck