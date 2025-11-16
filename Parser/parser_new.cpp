
#include <string>
#include <fstream>
#include <iostream>
#include <sstream>
#include <iterator>
#include <filesystem>
#include <expected>
#include <print>
#include <variant>

namespace openck::parser
{
    struct Token 
    {
        enum struct Type
        {
            // Single-character tokens.
            LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
            COMMA, DOT, MINUS, PLUS, SEMICOLON, SLASH, STAR,

            // One or two character tokens.
            BANG, BANG_EQUAL,
            EQUAL, EQUAL_EQUAL,
            GREATER, GREATER_EQUAL,
            LESS, LESS_EQUAL,

            // Literals.
            IDENTIFIER, QUOTED_STRING, NUMBER,

            NEW_LINE, END_OF_FILE
        };

        struct Location
        {
            int line;
            int offset; //offset from begining of source
        };

        std::string lexeme;
        Location location;

        using Literal = std::variant<std::string, double>;
        Literal literal;
        Type type;

        Token(Type type, const std::string& lexeme, Token::Literal literal, Location location);

        /*friend std::ostream& operator<<(std::ostream& os, const Token& token) 
        {
            std::print(os, "type:{} - {}", token.type, token.lexeme);
        }*/
    };


    Token::Token(Token::Type type, const std::string& lexeme, Token::Literal literal, Token::Location location) :
        type(type), lexeme(lexeme), location(location), literal(literal){}

    //general parser, we should impliment a ck2 parser which consumes the output of this parser and
    class Parser
    {
        public:
            struct Error
            {
                int line;
                std::string where;
                std::string message;

                void report() const;
            };

            Parser(const std::string& source_path);    
            
            const std::string& get_source() const {return source;};

        private:
            bool read_file(const std::string& path);

            bool is_at_end(int offset = 0);

            void scan_tokens();

            void scan_token();

            void add_token(Token::Type type, Token::Literal literal);

            void add_token(Token::Type type);

            void string();

            void number(bool is_negative);
            
            char advance();

            bool match(char c);

            char peek(int offset = 0);

            bool is_digit(char c);

            bool is_alpha(char c);

            bool is_alph_num(char c);

            void identifer();

            void increment_line();

            std::string get_selection();

            std::string get_current_line();
            
            std::string source;
            
            std::vector<Token> tokens;

            int start = 0; //index of first char of lexeme in source
            int current = 0; //index of the charecter currently being evaluated in source
            int line = 1; //the line of the charecter that current points at
            int current_line_start = 0; //index of first char of line

            bool was_error = false;
    };


    Parser::Parser(const std::string& source_path)
    {
        this->read_file(source_path);
        this->scan_tokens();
    }

    bool Parser::read_file(const std::string& path)
    {
        std::ifstream fs(path);
        
        if (fs.fail())
        {
            std::cout << "failed to read file: " << path << std::endl;
            return false;
        }
        
        fs >> std::noskipws;
        this->source = {std::istream_iterator<char>(fs), std::istream_iterator<char>()};
        return true;
    }

    bool Parser::is_at_end(int offset)
    {
        return (this->current + offset) >= source.length();
    }

    void Parser::scan_tokens()
    {
        while (!this->is_at_end())
        {
            this->start = this->current;
            this->scan_token();
        }

        this->tokens.emplace_back(Token::Type::END_OF_FILE, "", Token::Literal(), Token::Location(line, start));
    }

    void Parser::scan_token()
    {
        char c = this->advance();
        switch (c) 
        {
            case '{' : this->add_token(Token::Type::LEFT_BRACE); break;
            case '}' : this->add_token(Token::Type::RIGHT_BRACE); break;
            case ',' : this->add_token(Token::Type::COMMA); break;
            case '+' : this->is_digit(this->peek()) ? this->number(false) : this->add_token(Token::Type::PLUS); break;
            case '-' : this->is_digit(this->peek()) ? this->number(true) : this->add_token(Token::Type::MINUS); break;
            case '.' : this->add_token(Token::Type::DOT); break; 
            case '=' : this->add_token(this->match('=') ? Token::Type::EQUAL_EQUAL : Token::Type::EQUAL); break;
            case '>' : this->add_token(this->match('=') ? Token::Type::GREATER_EQUAL : Token::Type::GREATER); break;
            case '<' : this->add_token(this->match('=') ? Token::Type::LESS_EQUAL : Token::Type::LESS); break;
            case '!' : this->add_token(this->match('=') ? Token::Type::BANG_EQUAL : Token::Type::BANG); break;
            case '#' : {while(this->peek() != '\n' && !this->is_at_end()) {this->advance();} break;}
            case ' ' : case '\r': case '\t': break;
            case '\n': this->increment_line(); this->add_token(Token::Type::NEW_LINE); break;
            case '"' : this->string(); break;
            default :
            {
                if (this->is_alpha(c))
                {
                    this->identifer();
                }
                else if (this->is_digit(c))
                {
                    this->number(false);
                    break;
                }
                else
                {
                    Error{this->line, this->get_current_line(), std::format("unexpected charecter: {}", c)}.report();
                    this->was_error = true;
                    break;
                }
            }
        }
    }

    void Parser::add_token(Token::Type type, Token::Literal literal)
    {
        this->tokens.emplace_back(type, this->get_selection(), literal, Token::Location{this->line, this->current});
    }

    void Parser::add_token(Token::Type type)
    {
        this->add_token(type, {});
    }

    void Parser::string()
    {
        char next_char = this->peek();
        while (next_char != '"' && !this->is_at_end())
        {
            if (next_char == '\n') this->increment_line();
            next_char = this->advance();
        }

        if (this->is_at_end())
        {
            Error{this->line, {}, "Unterminated string"}.report();
            was_error = true;
        }
        else if (next_char == '"')
        {
            this->advance();
        }
        else
        {
            Error{this->line, {}, "Unterminated string?"}.report();
            was_error = true;
        }

        this->add_token(Token::Type::QUOTED_STRING, get_selection());
    }

    void Parser::number(bool is_negative)
    {
        while (this->is_digit(this->peek())) this->advance();
        
        if (this->peek() == '.' && this->is_digit(this->peek(1))) 
        {
            advance();
            while (this->is_digit(this->peek())) advance();
        }
        
        double number = std::stod(get_selection());
        
        if (is_negative)
            number = -number;
        
        this->add_token(Token::Type::NUMBER, number);
    }

    char Parser::advance()
    {
        return this->source.at(this->current++);
    }

    bool Parser::match(char expected_char)
    {
        if (this->is_at_end()) return false;
        if (this->peek() != expected_char) return false;
        else this->advance(); return true;
    }

    char Parser::peek(int offset)
    {
        if (this->is_at_end(offset))
            return '\0';
        else
            return this->source.at(this->current + offset);
    }

    bool Parser::is_digit(char c)
    {
        return c >= '0' && c <= '9';
    }

    bool Parser::is_alpha(char c)
    {
        return (c >= 'a' && c <= 'z') || (c >= 'A' && c <= 'Z') || c == '_';
    }

    bool Parser::is_alph_num(char c)
    {
        return this->is_alpha(c) || this->is_digit(c);
    }

    void Parser::identifer()
    {
        while (this->is_alph_num(this->peek())) advance();

        this->add_token(Token::Type::IDENTIFIER, get_selection());
    }

    void Parser::increment_line()
    {
        this->line++;
        this->current_line_start = this->current;
    }

    std::string Parser::get_selection()
    {
        return this->source.substr(this->start, this->current - this->start);
    }

    std::string Parser::get_current_line()
    {
        return std::string(this->source.substr(current_line_start, current_line_start - source.find('\n', this->current_line_start)));
    }

    void Parser::Error::report() const
    {
        std::print("[{}] Error {}: {}", this->line, this->where, this->message);
    }
}


int main()
{
    openck::parser::Parser parser("00_traits.txt");
    std::cout << parser.get_source();
}