#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdbool.h>
#include <ctype.h>

typedef enum
{
    TOKEN_TYPE_QUOTED_STRING,
    TOKEN_TYPE_STRING,
    TOKEN_TYPE_EQUALS,
    TOKEN_TYPE_COMMA,
    TOKEN_TYPE_BLOCK_START,
    TOKEN_TYPE_BLOCK_END,
    TOKEN_TYPE_NOT_SET
}TokenType;

typedef struct 
{
    TokenType type;        
    size_t length;
    const char* string;
}Token;

Token* getNextToken(Token* current_token)
{
    static int i;
    i++;
    printf("%d \n", i);
    current_token++;
    current_token->length = 0;
    current_token->string = NULL;
    current_token->type = TOKEN_TYPE_NOT_SET;
    return current_token;
}

typedef struct 
{
    Token* key;
    Token* value;
} KeyValuePair;

extractKeyValuePairs(Token* tokenBuffer, size_t number_of_tokens, KeyValuePair* kvBuffer, size_t number_of_kv_pairs)
{
    size_t kvs_found = 0;
    KeyValuePair* current_kv = kvBuffer;
    for (size_t i = 1; i < number_of_tokens; i++)
    {
        Token* current_token = &tokenBuffer[i];
        if (current_token->type == TOKEN_TYPE_EQUALS)
        {
            current_kv->key = tokenBuffer - 1;
            current_kv->value = tokenBuffer + 1;
            i += 2;
            kvs_found++;
            current_kv = &kvBuffer[++kvs_found];
        }
    }
}


char* readFile(const char* path)
{
    FILE* File;
    fopen_s(&File, path, "r");
    if (File == NULL)
    {
        fprintf(stderr, "Could not open file \"%s\"", path); 
        return NULL;
    }

    fseek(File, 0, SEEK_END);
    size_t fileSize = ftell(File);
    rewind(File);

    char* buffer = malloc(fileSize + 1);
    size_t bytesRead = fread(buffer, sizeof(char), fileSize, File);
    buffer[bytesRead] = '\0';

    return buffer;
}

//Returns number of tokens written.
size_t scanLine(const char* line, Token* token_buffer, size_t token_count)
{
    if (line == NULL)
        return 0;  

    size_t i = 0;
    size_t tokens_found = 0;
    Token* token = token_buffer;
    token->length = 0;
    token->string = NULL;
    token->type = TOKEN_TYPE_NOT_SET;

    while(line[i] != '\n' && line[i] != '\0' && token_count - tokens_found)
    {
        if (token->type != TOKEN_TYPE_NOT_SET)
        {
            token = getNextToken(token);
            tokens_found++;
        }

        switch (line[i])
        {
            case '\t':
            case ' ':
                break;
           
            case '#':
                return tokens_found;

            case '"':
                char* endQuote = strchr(&line[i+1], '"');
                token->type = TOKEN_TYPE_QUOTED_STRING;
                token->string = &line[i+1];
                token->length = endQuote - token->string;
                i += token->length + 1;
                break;
           
            case '=':
                token->type = TOKEN_TYPE_EQUALS;
                token->length = 1;
                token->string = &line[i];
                break;
            
            case '{':
            case '}':
                token->length = 1;
                token->string = &line[i];
                token->type = (line[i] == '{' ? TOKEN_TYPE_BLOCK_START : TOKEN_TYPE_BLOCK_END);
                break;

            default:
                token->type = TOKEN_TYPE_STRING;
                token->string = &line[i];
                while (isalnum(line[i]) || line[i] == '_' || line[i] == '-' || line[i] == '+')
                {
                    token->length++;
                    i++;
                }
        }
        i++;
    }

    return i;
}

int main(int argc, char** argv)
{
    if (argc != 2)
    {
        puts("please provide a single file to parse");
        return -1;
    }

    char* buffer = readFile(argv[1]);

    size_t token_count = 10000;
    size_t tokens_found = 0;
    Token* tokens = malloc(sizeof(Token) * token_count);
    while (token_count - tokens_found)
    {
        tokens_found += scanLine(buffer, &tokens[tokens_found], token_count - tokens_found);
        buffer = strchr(buffer, '\n');
        
        if (buffer == NULL)
            break;
        
        *buffer = '\0';
        buffer++;
    }



    for (int i = 0; i < tokens_found; i++)
    {
        printf("%d: %.*s ", i,  tokens[i].length, tokens[i].string);
    }

    return 0;
}