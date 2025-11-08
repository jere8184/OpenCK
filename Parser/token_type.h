#ifndef TOKEN_TYPE_H
#define TOKEN_TYPE_H

namespace prdx2cpp
{
    enum struct token_type
    {
        QUOTED_STRING,
        STRING,
        EQUALS,
        COMMA,
        BLOCK_START,
        BLOCK_END,
        NOT_SET
    };
}

#endif