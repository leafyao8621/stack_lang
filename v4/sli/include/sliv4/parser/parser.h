#ifndef PARSER_PARSER_H_
#define PARSER_PARSER_H_

#include <stdbool.h>

#include <containers/dstring.h>
#include <containers/hashmap.h>

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

#endif
