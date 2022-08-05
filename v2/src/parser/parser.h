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

#define TOKEN_OPERATOR_ADD 0
#define TOKEN_OPERATOR_SUBTRACT 1
#define TOKEN_OPERATOR_MULTIPLY 2
#define TOKEN_OPERATOR_DIVIDE 3
#define TOKEN_OPERATOR_MODULO 4
#define TOKEN_OPERATOR_ASSIGN 5
#define TOKEN_OPERATOR_GT 6
#define TOKEN_OPERATOR_LT 7
#define TOKEN_OPERATOR_GTE 8
#define TOKEN_OPERATOR_LTE 9
#define TOKEN_OPERATOR_EQ 10
#define TOKEN_OPERATOR_NEQ 11
#define TOKEN_OPERATOR_RSHIFT 12
#define TOKEN_OPERATOR_LSHIFT 13
#define TOKEN_OPERATOR_BNOT 14
#define TOKEN_OPERATOR_LNOT 15
#define TOKEN_OPERATOR_BAND 16
#define TOKEN_OPERATOR_LAND 17
#define TOKEN_OPERATOR_BOR 18
#define TOKEN_OPERATOR_LOR 19
#define TOKEN_OPERATOR_IDX 20

#define TOKEN_COMMAND_PRINT 0
#define TOKEN_COMMAND_PRINTLN 1
#define TOKEN_COMMAND_IF 2
#define TOKEN_COMMAND_ELSE 3
#define TOKEN_COMMAND_WHILE 4
#define TOKEN_COMMAND_DO 5
#define TOKEN_COMMAND_END_IF 6
#define TOKEN_COMMAND_END_LOOP 7

typedef uint64_t Type;
typedef char Character;
typedef char *String;
typedef size_t Size;
typedef size_t Idx;
typedef uint8_t Operator;
typedef uint64_t CommandType;
typedef int64_t Offset;

typedef struct Token {
    Type type;
    union {
        int64_t int_lit;
        Idx str_lit;
        String int_name;
        String str_name;
        Character str_char;
        String arr_name;
        Operator operator;
        struct {
            CommandType type;
            union {
                struct {
                    Offset offset;
                    Idx idx;
                } command_if;
                struct {
                    Offset offset;
                    Idx idx;
                } command_else;
                Idx command_while;
                struct {
                    Offset offset;
                    Idx idx;
                } command_do;
                Idx end_if;
                struct {
                    Offset offset;
                    Idx idx;
                } command_end_loop;
            } data;
        } command;
    } data;
} Token;

DEF_DARRAY(Token)
DEF_DARRAY(Character)
DEF_DARRAY(String)
DEF_HASHSET(String)
DEF_HASHMAP(String, Size)
DEF_HASHMAP(String, Idx)

typedef struct Parser {
    size_t idx_if, idx_else, idx_while, idx_do, idx_end_if, idx_end_loop;
    DArrayToken tokens, stack;
    DArrayCharacter str_buf;
    DArrayString str_lit;
    HashSetString int_name;
    HashSetString str_name;
    HashMapStringSize arr_name;
    HashMapStringIdx handler_lookup;
    FILE *fin;
} Parser;

int parser_initialize(Parser *parser, String fn);
int parser_parse(Parser *parser);
int parser_log(Parser *parser, FILE *fout);
int parser_finalize(Parser *parser);

#endif
