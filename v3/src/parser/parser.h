#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <containers/darray.h>

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
} Token;

DEF_DARRAY(Token)

typedef struct SLParser {
    DArrayToken code;
} SLParser;

int parser_initialize(SLParser *parser);
int parser_parse(SLParser *parser, char *str);
int parser_clear_code(SLParser *parser);
int parser_finalize(SLParser *parser);

#endif
