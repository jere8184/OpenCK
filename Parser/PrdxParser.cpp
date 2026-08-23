
#include "PrdxParser.hpp"

#include <cctype>
#include <string>
#include <print>
#include <vector>
#include <unordered_map>
#include <stack>
#include <cwctype>
#include <cstdio>


namespace openck::parser
{
	
//Returns number of tokens written.
void TokeniseText(const std::string& source, std::vector<Token>& tokens)
{
	size_t i = 0;
	size_t length = 0;

	while (i < source.length())
	{
		switch (source[i])
		{
			case '\t':
			case ' ':
			{
				break;
			}
			case '\n':
			{
				tokens.emplace_back(Token::Type::NEW_LINE, std::string_view(&source[i], 1));
				break;
			}
			case '#':
			{
				while (source[i] != '\n' && source[i] != '\0')
				{
					i++;
				}
				break;
			}
			case '=':
			{
				tokens.emplace_back(Token::Type::EQUAL, std::string_view(&source[i], 1));
				break;
			}
			case '{':
			{
				tokens.emplace_back(Token::Type::BLOCK_START, std::string_view(&source[i], 1));
				break;
			}
			case '}':
			{
				tokens.emplace_back(Token::Type::BLOCK_END, std::string_view(&source[i], 1));
				break;
			}
			case ',':
			{
				tokens.emplace_back(Token::Type::COMMA, std::string_view(&source[i], 1));
				break;
			}
			case '"':
			{
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
				tokens.emplace_back(Token::Type::QUOTED_STRING, std::string_view(&source[i-length], length));
				break;	
			}
			default:
			{
				length = 0;
				if (source[i] == '+' || source[i] == '-')
				{
					length++;
					i++;
				}

				

				while (!(std::iscntrl(source[i]) || std::ispunct(source[i]) || std::isspace(source[i])) || source[i] == '_' || source[i] ==  '.' || source[i] ==  ':')
				{
					length++;
					i++;
				}

				if (length)
				{
					tokens.emplace_back(Token::Type::STRING, std::string_view(&source[i-length], length));
					continue;
				}
			}
		}
		i++;
	}
	return;
}

bool is_operator(Token::Type type)
{
	switch (type)
	{
	case Token::Type::EQUAL :
	case Token::Type::LESS :
		return true;
		break;

	default:
		return false;
		break;
	}
}

Node::Operator token_type_to_node_operator(Token::Type type)
{
	if (!is_operator(type)) return Node::Operator::NOT_SET;

	switch (type)
	{
	case Token::Type::EQUAL :
		return Node::Operator::EQUALS;
		break;

	case Token::Type::LESS :
		return Node::Operator::LESS;
		break;

	default:
		return Node::Operator::NOT_SET;
		break;
	}
}

bool create_nodes(const std::vector<Token>& tokens, std::vector<Node>& root_nodes)
{
	std::stack<std::shared_ptr<Node>> scopeStack;
	std::shared_ptr<Node> currentNode = nullptr;
	Token::Type previousTokenType = Token::Type::NOT_SET;

	int nodeCount = 0; // for debugging
	int tokenCount = 0;
	for (const Token& token : tokens)
	{
		tokenCount++;
		switch (token.type)
		{
		case Token::Type::STRING:
			if(is_operator(previousTokenType))
			{
				currentNode->value = token.text;
				currentNode->type = Node::Type::STRING;
				currentNode->op = token_type_to_node_operator(previousTokenType);
				if (scopeStack.size())
				{
					scopeStack.top()->add_child(*currentNode);
					currentNode.reset();
				}
				currentNode = nullptr;
			}
			else
			{
				if (currentNode != nullptr && currentNode->value.empty() && scopeStack.size()) //lists
				{
					currentNode->type = Node::Type::STRING;
					scopeStack.top()->add_child(*currentNode);
					currentNode.reset();
				}

				nodeCount++;
				currentNode = std::make_shared<Node>();
				currentNode->name = token.text;
			}
			previousTokenType = Token::Type::STRING;
			break;

		case Token::Type::QUOTED_STRING:
			if (currentNode && is_operator(previousTokenType))
			{
				currentNode->value = token.text;
				currentNode->type = Node::Type::STRING;
				currentNode->op = token_type_to_node_operator(previousTokenType);
				if (scopeStack.size())
				{
					scopeStack.top()->add_child(*currentNode);
					currentNode.reset();
				}
				currentNode = nullptr;
			}
			else
			{
				return false;
			}
			previousTokenType = Token::Type::QUOTED_STRING;
			break;

		case Token::Type::EQUAL:
			if (!currentNode)
				return false;
			previousTokenType = Token::Type::EQUAL;
			break;

		case Token::Type::NEW_LINE:
			if (Token::Type::EQUAL == previousTokenType)
				continue;

			if (currentNode && scopeStack.size())
			{
				scopeStack.top()->add_child(*currentNode);
				currentNode.reset();
			}

			previousTokenType = Token::Type::NEW_LINE;
			break;

		case Token::Type::BLOCK_START:
			if (currentNode && is_operator(previousTokenType))
			{
				currentNode->type = Node::Type::BLOCK;
				currentNode->op = token_type_to_node_operator(previousTokenType);
				scopeStack.push(currentNode);
				currentNode.reset();
			}
			else
			{
				return false;
			}
			previousTokenType = Token::Type::BLOCK_START;
			break;

		case Token::Type::BLOCK_END:
		{
			std::shared_ptr<Node> completedBlock = scopeStack.top();
			scopeStack.pop();

			if (scopeStack.size() == 0)
			{
				root_nodes.push_back(std::move(*completedBlock));
			}
			else
			{
				if (currentNode)
				{
					completedBlock->add_child(*currentNode);
					currentNode.reset();
				}
				scopeStack.top()->add_child(*completedBlock);
			}
			completedBlock.reset();
			previousTokenType = Token::Type::BLOCK_END;
			break;
		}
		default:
			break;
		}
	}
	return true;
}

std::string read_file(const std::filesystem::path& path)
{
	std::FILE* File = std::fopen(path.string().c_str(), "r");
	if (File == nullptr)
	{
		std::print(stderr, "Could not open file \"{}\"", path.string());
		return "";
	}

	fseek(File, 0, SEEK_END);
	size_t fileSize = ftell(File);
	rewind(File);

	std::string fileContents;
	fileContents.resize(fileSize);

	size_t bytes_read = std::fread(fileContents.data(), sizeof(char), fileSize, File);
	fileContents.resize(bytes_read);
	return fileContents;
}

bool generate_nodes(const std::filesystem::path& path, std::vector<Node>& nodes)
{
	const std::string& fileContents = read_file(path);

	std::vector<Token> tokens;
	TokeniseText(fileContents, tokens);

	nodes.reserve(tokens.size());

	return create_nodes(tokens, nodes);
}

bool generate_chunks();

} //openck