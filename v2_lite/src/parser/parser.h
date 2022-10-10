#ifndef PARSER_H_
#define PARSER_H_

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
#define TOKEN_FUNCTION_CALL 8

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
#define TOKEN_OPERATOR_XOR 20
#define TOKEN_OPERATOR_INCREMENT 21
#define TOKEN_OPERATOR_INCREMENT_BY 22
#define TOKEN_OPERATOR_DECREMENT 23
#define TOKEN_OPERATOR_DECREMENT_BY 24
#define TOKEN_OPERATOR_IDX 25

#define TOKEN_COMMAND_INPUT 0
#define TOKEN_COMMAND_PRINT 1
#define TOKEN_COMMAND_PRINTLN 2
#define TOKEN_COMMAND_IF 3
#define TOKEN_COMMAND_ELSE 4
#define TOKEN_COMMAND_WHILE 5
#define TOKEN_COMMAND_DO 6
#define TOKEN_COMMAND_END_IF 7
#define TOKEN_COMMAND_END_LOOP 8
#define TOKEN_COMMAND_DEF 9
#define TOKEN_COMMAND_BEGIN 10
#define TOKEN_COMMAND_END_FUNCTION 11
#define TOKEN_COMMAND_SRAND 12
#define TOKEN_COMMAND_RAND 13

typedef unsigned char Type;
typedef char Character;
typedef char *String;
typedef size_t Size;
typedef size_t Idx;
typedef unsigned char Operator;
typedef unsigned char CommandType;

typedef struct Token {
    Type type;
    union {
        int int_lit;
        Idx str_lit;
        String int_name;
        String str_name;
        Character str_char;
        String arr_name;
        Operator operater;
        struct {
            CommandType type;
            union {
                struct {
                    Idx offset;
                    Idx idx;
                } command_if;
                struct {
                    Idx offset;
                    Idx idx;
                } command_else;
                Idx command_while;
                struct {
                    Idx offset;
                    Idx idx;
                } command_do;
                Idx command_end_if;
                struct {
                    Idx offset;
                    Idx idx;
                } command_end_loop;
            } data;
        } command;
        String function_call;
    } data;
} Token;

DEF_STACK(Token, 10)
DEF_STACK(Token, 2000)
DEF_HASHSET(String, 10)

typedef struct Function {
    StackToken10 args;
    StackToken2000 tokens;
    StackToken10 ret_vals;
    HashSetString10 int_name;
    HashSetString10 str_name;
} Function;

DEF_STACK(Idx, 10)
DEF_STACK(Character, 5000)
DEF_HASHSET(String, 200)
DEF_HASHMAP(String, Size, 10)
DEF_HASHMAP(String, Function, 10)
DEF_HASHMAP(String, Idx, 14)

typedef struct Parser {
    unsigned char function_definition;
    size_t idx_if, idx_else, idx_while, idx_do, idx_end_if, idx_end_loop;
    StackToken2000 tokens, *cur_token_buf;
    StackIdx10 stack;
    StackCharacter5000 str_buf;
    HashSetString200 str_lit;
    HashSetString10 int_name;
    HashSetString10 str_name;
    HashMapStringSize10 arr_name;
    HashMapStringFunction10 function_name;
    HashMapStringIdx14 handler_lookup;
    Function *cur_function;
    FILE *fin;
} Parser;

int parser_initialize(Parser *parser, String fn);
int parser_parse(Parser *parser);
int parser_log(Parser *parser, FILE *fout);
int parser_finalize(Parser *parser);

#endif
