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
    SL_TOKEN_TYPE_OPERATOR,
    SL_TOKEN_TYPE_COMMAND,
    SL_TOKEN_TYPE_FUNCTION
} SLTokenType;

typedef size_t Idx;

typedef enum SLArrayType {
    SL_ARRAY_TYPE_INT,
    SL_ARRAY_TYPE_FLOAT,
    SL_ARRAY_TYPE_CHAR,
    SL_ARRAY_TYPE_STR
} SLArrayType;

typedef enum SLOperatorType {
    SL_OPERATOR_TYPE_ADD,
    SL_OPERATOR_TYPE_SUBTRACT,
    SL_OPERATOR_TYPE_MULTIPLY,
    SL_OPERATOR_TYPE_DIVIDE,
    SL_OPERATOR_TYPE_MODULO,
    SL_OPERATOR_TYPE_LSHIFT,
    SL_OPERATOR_TYPE_RSHIFT,
    SL_OPERATOR_TYPE_RLSHIFT,
    SL_OPERATOR_TYPE_BNOT,
    SL_OPERATOR_TYPE_BAND,
    SL_OPERATOR_TYPE_BOR,
    SL_OPERATOR_TYPE_BXOR,
    SL_OPERATOR_TYPE_LNOT,
    SL_OPERATOR_TYPE_LAND,
    SL_OPERATOR_TYPE_LOR,
    SL_OPERATOR_TYPE_EQ,
    SL_OPERATOR_TYPE_NEQ,
    SL_OPERATOR_TYPE_LT,
    SL_OPERATOR_TYPE_GT,
    SL_OPERATOR_TYPE_LEQ,
    SL_OPERATOR_TYPE_GEQ,
    SL_OPERATOR_TYPE_ASSIGN,
    SL_OPERATOR_TYPE_INC,
    SL_OPERATOR_TYPE_DEC,
    SL_OPERATOR_TYPE_ADD_ASSIGN,
    SL_OPERATOR_TYPE_SUBTRACT_ASSIGN,
    SL_OPERATOR_TYPE_MULTIPLY_ASSIGN,
    SL_OPERATOR_TYPE_DIVIDE_ASSIGN,
    SL_OPERATOR_TYPE_LSHIFT_ASSIGN,
    SL_OPERATOR_TYPE_RSHIFT_ASSIGN,
    SL_OPERATOR_TYPE_RLSHIFT_ASSIGN,
    SL_OPERATOR_TYPE_BAND_ASSIGN,
    SL_OPERATOR_TYPE_BOR_ASSIGN,
    SL_OPERATOR_TYPE_BXOR_ASSIGN,
    SL_OPERATOR_TYPE_INDEX
} SLOperatorType;

typedef enum SLCommandType {
    SL_COMMAND_TYPE_PRINT,
    SL_COMMAND_TYPE_PRINTLN,
    SL_COMMAND_TYPE_INPUT,
    SL_COMMAND_TYPE_SRAND,
    SL_COMMAND_TYPE_RAND,
    SL_COMMAND_TYPE_ALLOC,
    SL_COMMAND_TYPE_REALLOC,
    SL_COMMAND_TYPE_IF,
    SL_COMMAND_TYPE_ELSE,
    SL_COMMAND_TYPE_END,
    SL_COMMAND_TYPE_WHILE,
    SL_COMMAND_TYPE_DO,
    SL_COMMAND_TYPE_FOR,
    SL_COMMAND_TYPE_BREAK,
    SL_COMMAND_TYPE_CONTINUE,
    SL_COMMAND_TYPE_DEF,
    SL_COMMAND_TYPE_RETURN
} SLCommandType;

typedef enum SLVariableLocation {
    SL_VARIABLE_LOCATION_LOCAL,
    SL_VARIABLE_LOCATION_PARAMETER,
    SL_VARIABLE_LOCATION_GLOBAL
} SLVariableLocation;

typedef struct SLVariableData {
    SLVariableLocation location;
    Idx idx;
} SLVariableData;

typedef struct SLToken SLToken;

struct SLToken {
    SLTokenType type;
    union {
        int64_t int_literal;
        SLVariableData int_var;
        double float_literal;
        SLVariableData float_var;
        char char_literal;
        SLVariableData char_var;
        Idx str_literal;
        SLVariableData str_var;
        struct {
            SLVariableData var_data;
            Idx dim;
            SLArrayType type;
        } arr;
        SLOperatorType operator_type;
        struct {
            SLCommandType type;
            SLToken *tgt;
        } command;
        Idx function;
    } data;
};

DEF_DARRAY(SLToken)

typedef struct SLVariableTypeName {
    SLTokenType type;
    String name;
} SLVariableTypeName;

DEF_HASHMAP(SLVariableTypeName, Idx)

typedef struct SLFunction {
    HashMapSLVariableTypeNameIdx par_lookup, local_lookup;
    DArraySLToken code;
    DArraySLToken ret;
} SLFunction;

DEF_DARRAY(String)
DEF_DARRAY(SLFunction)

typedef struct SLParser {
    HashMapSLVariableTypeNameIdx function_lookup, global_lookup;
    DArrayString str_literals;
    DArraySLFunction functions;
    DArraySLToken code;
} SLParser;

SLErrCode SLParser_initialize(SLParser *parser);
SLErrCode SLParser_parse(SLParser *parser, char *str);
SLErrCode SLParser_clear_code(SLParser *parser);
SLErrCode SLParser_finalize(SLParser *parser);
SLErrCode SLParser_log(SLParser *parser, FILE *fout);

DEF_DARRAY(Idx)

DEF_HASHMAP(String, Idx)

struct SLParserBuffer {
    String token_buf;
    HashMapStringIdx str_literal_lookup;
    DArraySLToken *cur_token_buf;
    bool global;
    Idx cur_function, global_offset;
    DArrayIdx parameter_offsets, local_offsets;
    DArraySLToken operation_stack;
};

#endif
