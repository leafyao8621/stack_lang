#ifndef GENERATOR_H_
#define GENERATOR_H_

#include "../../parser/parser.h"

DEF_DARRAY(Type)

typedef struct Generator {
    Parser parser;
    DArrayType stack;
    FILE *fout;
} Generator;

int generator_initialize(Generator *generator, String ifn, String ofn);
int generator_generate(Generator *generator);
int generator_finalize(Generator *generator);

#endif
