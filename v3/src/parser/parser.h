#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>

#include <containers/dstring.h>
#include <containers/darray.h>
#include <containers/hashmap.h>

#include <sliv3/errcode.h>

typedef enum SLTokenType {
    SL_TOKEN_TYPE_INT_LITERAL,
    SL_TOKEN_TYPE_INT_VAR,
    SL_TOKEN_TYPE_FLOAT_LITERAL,
    SL_TOKEN_TYPE_FLOAT_VAR,
    SL_TOKEN_TYPE_STR_LITERAL,
    SL_TOKEN_TYPE_STR_VAR,
    SL_TOKEN_TYPE_ARR,
    SL_TOKEN_TYPE_COMMAND,
    SL_TOKEN_TYPE_FUNCTION
} SLTokenType;

typedef struct SLToken {
    SLTokenType type;
} SLToken;

DEF_DARRAY(SLToken)

typedef int Idx;

DEF_HASHMAP(String, Idx)

typedef HashMapStringIdx VarIdx;

typedef struct SLFunction {
    VarIdx par, local;
    DArrayToken code;
    DArrayToken ret;
} SLFunction;

typedef struct SLParser {
    DArrayToken code;
} SLParser;

SLErrCode slparser_initialize(SLParser *parser);
SLErrCode slparser_parse(SLParser *parser, char *str);
SLErrCode slparser_clear_code(SLParser *parser);
SLErrCode slparser_finalize(SLParser *parser);
SLErrCode slparser_log(FILE *fout);

#endif
