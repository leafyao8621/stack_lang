#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>

#include "../util/util.h"

typedef struct Token {
    uint8_t type;
} Token;

DEF_DARRAY(Token)

struct Parser {
    DArrayToken tokens;
};

#endif
