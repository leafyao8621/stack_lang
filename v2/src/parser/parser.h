#ifndef PARSER_H_
#define PARSER_H_

#include "../util/util.h"

typedef uint8_t Type;
typedef char Character;
typedef char *String;

typedef struct Token {
    Type type;
} Token;

DEF_DARRAY(Token)
DEF_DARRAY(Character)
DEF_HASHSET(String)
DEF_HASHMAP(String, String)

typedef struct Parser {
    DArrayToken tokens, stack;
    DArrayCharacter str_buf;
    HashSetString int_name, arr_name;
    HashMapStringString str_name;
} Parser;

int parser_initialize(Parser *parser);
int parser_finalize(Parser *parser);

#endif
