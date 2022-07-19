#ifndef PARSER_H_
#define PARSER_H_

#include <stdint.h>

struct Parser {
    struct Token {
        uint8_t type;
    }*tokens;
};

#endif
