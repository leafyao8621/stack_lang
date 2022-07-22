#include "generator.h"

DEF_DRRAY_FUNCTIONS(Type)

int generator_initialize(Generator *generator, String ifn, String ofn) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    generator->fout = fopen(ofn, "w");
    if (!generator->fout) {
        return ERR_FILE_IO;
    }
    int ret = DArrayType_initialize(&generator->stack, 1000);
    if (ret) {
        return ret;
    }
    ret = parser_initialize(&generator->parser, ifn);
    if (ret) {
        DArrayType_finalize(&generator->stack);
        return ret;
    }
    
    return 0;
}

int generator_finalize(Generator *generator) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    int ret = DArrayType_finalize(&generator->stack);
    if (ret) {
        return ret;
    }
    ret = parser_finalize(&generator->parser);
    if (ret) {
        return ret;
    }
    fclose(generator->fout);
    return 0;
}

int generator_generate(Generator *generator) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    int ret = parser_parse(&generator->parser);
    if (ret) {
        return ret;
    }
    return 0;
}
