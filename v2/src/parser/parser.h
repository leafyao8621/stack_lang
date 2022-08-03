#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>

#include "../util/util.h"

#define TOKEN_INT_LIT 0
#define TOKEN_STR_LIT 1
#define TOKEN_INT_NAME 2
#define TOKEN_STR_NAME 3
#define TOKEN_STR_CHAR 4
#define TOKEN_ARR_NAME 5
#define TOKEN_OPERATOR 6
#define TOKEN_COMMAND 7

typedef uint64_t Type;
typedef char Character;
typedef char *String;
typedef size_t Size;
typedef uint8_t Operator;
typedef uint8_t Command;

typedef struct Token {
    Type type;
    union {
        int64_t int_lit;
        String str_lit;
        String int_name;
        String str_name;
        Character str_char;
        String arr_name;
        Operator operator;
        Command command;
    } data;
} Token;

DEF_DARRAY(Token)
DEF_DARRAY(Character)
DEF_HASHSET(String)
DEF_HASHMAP(String, Size)

typedef struct Parser {
    DArrayToken tokens, stack;
    DArrayCharacter str_buf;
    HashSetString int_name;
    HashSetString str_name;
    HashMapStringSize arr_name;
    FILE *fin;
} Parser;

int parser_initialize(Parser *parser, String fn);
int parser_parse(Parser *parser);
int parser_log(Parser *parser, FILE *fout);
int parser_finalize(Parser *parser);

#endif
