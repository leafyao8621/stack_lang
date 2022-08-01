#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>

#include "../util/util.h"

typedef uint8_t Type;
typedef char Character;
typedef char *String;
typedef size_t Size;

typedef struct Token {
    Type type;
} Token;

DEF_DARRAY(Token)
DEF_DARRAY(Character)
DEF_HASHSET(String)
DEF_HASHMAP(String, String)
DEF_HASHMAP(String, Size)

typedef struct Parser {
    DArrayToken tokens, stack;
    DArrayCharacter str_buf;
    HashSetString int_name;
    HashMapStringString str_name;
    HashMapStringSize arr_name;
    FILE *fin;
} Parser;

int parser_initialize(Parser *parser, String fn);
int parser_parse(Parser *parser);
int parser_finalize(Parser *parser);

#endif
