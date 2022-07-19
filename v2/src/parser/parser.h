#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>

#include "../util/util.h"

typedef uint8_t Type;

typedef struct Token {
    Type type;
} Token;

DEF_DARRAY(Token)

typedef struct Parser {
    DArrayToken tokens, stack;
} Parser;

#endif
