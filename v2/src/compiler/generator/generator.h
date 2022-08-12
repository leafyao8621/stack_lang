#ifndef GENERATOR_GENERATOR_H_
#define GENERATOR_GENERATOR_H_

#include "../../parser/parser.h"

typedef int64_t Architecture;

#define ARCHITECTURE_X86_64_LINUX 0

DEF_DARRAY(Type)

typedef struct Generator {
    Architecture architecture;
    Parser parser;
    DArrayType stack;
    String ofn;
} Generator;

int generator_initialize(
    Generator *generator,
    Architecture architecture,
    String ifn,
    String ofn
);
int generator_generate(Generator *generator);
int generator_finalize(Generator *generator);

#endif
