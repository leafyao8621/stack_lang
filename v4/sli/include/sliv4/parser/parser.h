#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdbool.h>

#include <containers/dstring.h>
#include <containers/hashmap.h>

#include <sliv4/util/errcode.h>

typedef size_t Offset;

typedef enum SLParserState {
    SLPARSER_STATE_GLOBAL
} SLParserState;

typedef struct SLParser {
    SLParserState state;
} SLParser;

typedef struct SLSymbolTableValue {
    bool is_global;
} SLSymbolTableValue;

DEF_HASHMAP(Offset, SLSymbolTableValue)

typedef struct SLSymbolTable {
    String symbol_buffer;
    HashMapOffsetSLSymbolTableValue table;
} SLSymbolTable;

typedef struct SLCode {
    String data;
    String code;
} SLCode;

typedef struct SLModule {
    SLSymbolTable symbol_table;
    SLCode code;
} SLModule;

SLErr SLParser_parse_module_text(
    String *code, SLParser *parser, SLModule *module);
SLErr SLParse_load_module_byte(
    String *code, SLParser *parser, SLModule *module);

#endif
