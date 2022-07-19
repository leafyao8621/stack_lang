#include "generator.h"

DEF_DRRAY_FUNCTIONS(Type)

int generator_initialize(Generator *generator) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    int ret = DArrayType_initialize(&generator->stack);
    if (ret) {
        return ret;
    }
    ret = parser_initialize(&generator->parser);
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
    return 0;
}
