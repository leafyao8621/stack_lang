#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "../../parser/parser.h"

DEF_DARRAY(Type)

typedef struct Generator {
    Parser parser;
    DArrayType stack;
} Generator;

#endif
