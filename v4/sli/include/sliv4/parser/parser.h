#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdio.h>
#include <stdbool.h>

#include <containers/dstring.h>
#include <containers/hashmap.h>

#include <sliv4/util/errcode.h>

typedef size_t Offset;

typedef enum SLParserState {
    SL_PARSER_STATE_GLOBAL,
    SL_PARSER_STATE_DEFINITION_INT64
} SLParserState;

typedef enum SLValueType {
    SL_VALUE_TYPE_CHAR,
    SL_VALUE_TYPE_INT8,
    SL_VALUE_TYPE_UINT8,
    SL_VALUE_TYPE_INT16,
    SL_VALUE_TYPE_UINT16,
    SL_VALUE_TYPE_INT32,
    SL_VALUE_TYPE_UINT32,
    SL_VALUE_TYPE_INT64,
    SL_VALUE_TYPE_UINT64,
    SL_VALUE_TYPE_FLOAT32,
    SL_VALUE_TYPE_FLOAT64
} SLValueType;

typedef enum SLValueVariableLocation {
    SL_VALUE_VARIABLE_LOCATION_STR,
    SL_VALUE_VARIABLE_LOCATION_GLOBAL,
    SL_VALUE_VARIABLE_LOCATION_TEMP,
    SL_VALUE_VARIABLE_LOCATION_LOCAL,
    SL_VALUE_VARIABLE_LOCATION_FUNCTION
} SLValueVariableLocation;

typedef struct SLValue {
    bool is_literal;
    union {
        struct {
            SLValueType type;
            union {
                char chr;
                int8_t int8;
                uint8_t uint8;
                int16_t int16;
                uint16_t uint16;
                int32_t int32;
                uint32_t uint32;
                int64_t int64;
                uint64_t uint64;
                float float32;
                double float64;
                size_t offset;
            } data;
        } literal;
        struct {
            SLValueType type;
            union {
                void *handle;
                struct {
                    bool is_reference;
                    SLValueVariableLocation location;
                    size_t offset;
                } variable;
            } data;
        } identifier;
    } data;
} SLValue;

DEF_DARRAY(SLValue)

typedef struct SLParser {
    SLParserState state, prev_state;
    String buf;
    DArraySLValue value_stack;
    size_t global_offset, temp_offset, temp_offset_max;
} SLParser;

typedef struct SLSymbolTableValue {
    SLValueType type;
    SLValueVariableLocation location;
    size_t offset;
} SLSymbolTableValue;

DEF_DARRAY(SLSymbolTableValue)

typedef struct SLSymbolTable {
    String symbol_buf;
    DArraySLSymbolTableValue table;
} SLSymbolTable;

typedef enum SLInstructionOperator {
    SL_INSTRUCTION_OPERATOR_ADD,
    SL_INSTRUCTION_OPERATOR_ASSIGN,
} SLInstructionOperator;

typedef struct SLInstruction {
    SLInstructionOperator oprator;
    union {
        SLValue unary;
        struct {
            SLValue operand1;
            SLValue operand2;
        } binary;
        size_t par_list;
    } operand;
} SLInstruction;

DEF_DARRAY(SLInstruction)

typedef struct SLCode {
    String str;
    DArraySLInstruction code;
} SLCode;

typedef struct SLModule {
    SLSymbolTable symbol_table;
    SLCode code;
} SLModule;

SLErr SLParser_initialize(SLParser *parser);
SLErr SLParser_finalize(SLParser *parser);
SLErr SLParser_parse_module_text(
    String *code, SLParser *parser, SLModule *module);

SLErr SLModule_initialize(SLModule *module);
SLErr SLModule_finalize(SLModule *module);
SLErr SLModule_log(SLModule *module, FILE *fout);
SLErr SLModule_find_identifier(
    SLModule *module, char *token, SLSymbolTableValue *symbol);

#endif
