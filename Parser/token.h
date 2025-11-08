#ifndef TOKEN_H
#define TOKEN_H

#include <string_view>
#include <string>

#include "token_type.h"


namespace prdx2cpp
{
    struct token 
    {
        token_type type;
        std::string_view text;

        token(const token_type& type, const std::string_view& text)
        {
            this->type = type;
            this->text = text;
        }
    };

    struct string_token : token
    {
        string_token(const token_type& type, const std::string_view& text) : token(type, text), value(text) {};
        std::string value;
    };

    struct numeric_token : token 
    {
        numeric_token(const token_type& type, const std::string_view& text, bool is_negative) : token(type, text), value(std::stoi(std::string(text))), is_negative(is_negative) {};
        int value;
        bool is_negative;
    };


}

#endif