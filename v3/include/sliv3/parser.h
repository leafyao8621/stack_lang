#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include <containers/dstring.h>
#include <containers/darray.h>
#include <containers/hashmap.h>

#include <sliv3/errcode.h>

typedef enum SLTokenType {
    SL_TOKEN_TYPE_INT_LITERAL,
    SL_TOKEN_TYPE_INT_VAR,
    SL_TOKEN_TYPE_FLOAT_LITERAL,
    SL_TOKEN_TYPE_FLOAT_VAR,
    SL_TOKEN_TYPE_CHAR_LITERAL,
    SL_TOKEN_TYPE_CHAR_VAR,
    SL_TOKEN_TYPE_STR_LITERAL,
    SL_TOKEN_TYPE_STR_VAR,
    SL_TOKEN_TYPE_ARR,
    SL_TOKEN_TYPE_COMMAND,
    SL_TOKEN_TYPE_FUNCTION
} SLTokenType;

typedef size_t Idx;

typedef enum SLArrayType {
    SL_ARRAY_TYPE_INT,
    SL_ARRAY_TYPE_FLOAT,
    SL_ARRAY_TYPE_CHAR,
    SL_ARRAY_TYPE_STR,
    SL_ARRAY_TYPE_ARR
} SLArrayType;

typedef struct SLToken {
    SLTokenType type;
    union {
        int64_t int_literal;
        Idx int_var;
        double float_literal;
        Idx float_var;
        char char_literal;
        Idx char_var;
        Idx str_literal;
        Idx str_var;
        struct {
            Idx idx;
            SLArrayType type;
        } arr;
    } data;
} SLToken;

DEF_DARRAY(SLToken)

DEF_HASHMAP(String, Idx)

typedef struct SLFunction {
    HashMapStringIdx par_lookup, local_lookup;
    DArraySLToken code;
    DArraySLToken ret;
} SLFunction;

DEF_DARRAY(SLFunction)

typedef struct SLParser {
    HashMapStringIdx function_lookup;
    DArraySLFunction functions;
    DArraySLToken code;
} SLParser;

SLErrCode slparser_initialize(SLParser *parser);
SLErrCode slparser_parse(SLParser *parser, char *str);
SLErrCode slparser_clear_code(SLParser *parser);
SLErrCode slparser_finalize(SLParser *parser);
SLErrCode slparser_log(FILE *fout);

#endif
