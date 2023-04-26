#include "parser.h"

DEF_DARRAY_FUNCTIONS(SLToken)
DEF_DARRAY_FUNCTIONS(String)
DEF_DARRAY_FUNCTIONS(SLFunction)

DEF_HASHMAP_FUNCTIONS(SLVariableTypeName, Idx)

bool eq_sl_variable_type_name(SLVariableTypeName *a, SLVariableTypeName *b);
size_t hash_sl_variable_type_name(SLVariableTypeName *a);

SLErrCode SLParser_initialize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    int ret = DArraySLToken_initialize(&parser->code, 100);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        HashMapSLVariableTypeNameIdx_initialize(
            &parser->function_lookup,
            10,
            eq_sl_variable_type_name,
            hash_sl_variable_type_name
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        HashMapSLVariableTypeNameIdx_initialize(
            &parser->global_lookup,
            10,
            eq_sl_variable_type_name,
            hash_sl_variable_type_name
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArraySLFunction_initialize(&parser->functions, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

SLErrCode SLParser_clear_code(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    DArraySLToken_clear(&parser->code);
    return SL_ERR_OK;
}

SLErrCode SLParser_finalize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    DArraySLToken_finalize(&parser->code);
    HashMapSLVariableTypeNameIdx_finalize(&parser->function_lookup);
    HashMapSLVariableTypeNameIdx_finalize(&parser->global_lookup);
    DArraySLFunction_finalize(&parser->functions);
    return SL_ERR_OK;
}

SLErrCode SLParser_log(SLParser *parser, FILE *fout) {
    fputs("Code:\n", fout);
    SLToken *iter = parser->code.data;
    for (size_t i = 0; i < parser->code.size; ++i, ++iter) {
        switch (iter->type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            fprintf(
                fout,
                "IDX: %lu\nType: INT_LITERAL\nDEC: %ld\nHEX: 0x%016lX\n",
                i,
                iter->data.int_literal,
                iter->data.int_literal
            );
            break;
        case SL_TOKEN_TYPE_FLOAT_LITERAL:
            fprintf(
                fout,
                "IDX: %lu\nType: FLOAT_LITERAL\nDEC: %lE\nHEX: 0x%016lX\n",
                i,
                iter->data.float_literal,
                *(uint64_t*)&iter->data.float_literal
            );
            break;
        default:
            break;
        }
    }
    return SL_ERR_OK;
}
