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
    HashMapSLVariableTypeNameIdxNode *iter_global_lookup =
        parser->global_lookup.data;
    for (
        size_t i = 0;
        i < parser->global_lookup.capacity;
        ++i,
        ++iter_global_lookup) {
        if (iter_global_lookup->in_use) {
            DArrayChar_finalize(&iter_global_lookup->key.name);
        }
    }
    HashMapSLVariableTypeNameIdx_finalize(&parser->global_lookup);
    DArraySLFunction_finalize(&parser->functions);
    return SL_ERR_OK;
}

SLErrCode SLParser_log(SLParser *parser, FILE *fout) {
    static const char *location_lookup[3] = {
        "SL_VARIABLE_LOCATION_LOCAL",
        "SL_VARIABLE_LOCATION_PARAMETER",
        "SL_VARIABLE_LOCATION_GLOBAL"
    };
    static const char *type_lookup[12] = {
        "SL_TOKEN_TYPE_INT_LITERAL",
        "SL_TOKEN_TYPE_INT_VAR",
        "SL_TOKEN_TYPE_FLOAT_LITERAL",
        "SL_TOKEN_TYPE_FLOAT_VAR",
        "SL_TOKEN_TYPE_CHAR_LITERAL",
        "SL_TOKEN_TYPE_CHAR_VAR",
        "SL_TOKEN_TYPE_STR_LITERAL",
        "SL_TOKEN_TYPE_STR_VAR",
        "SL_TOKEN_TYPE_ARR",
        "SL_TOKEN_TYPE_OPERATOR",
        "SL_TOKEN_TYPE_COMMAND",
        "SL_TOKEN_TYPE_FUNCTION"
    };
    fputs("Global Variables:\n", fout);
    HashMapSLVariableTypeNameIdxNode *iter_global_lookup =
        parser->global_lookup.data;
    for (
        size_t i = 0;
        i < parser->global_lookup.capacity;
        ++i,
        ++iter_global_lookup) {
        if (iter_global_lookup->in_use) {
            fprintf(
                fout,
                "Name: %s\nType: %s\nOffset: %lu\n",
                iter_global_lookup->key.name.data,
                type_lookup[iter_global_lookup->key.type],
                iter_global_lookup->value
            );
        }
    }
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
        case SL_TOKEN_TYPE_INT_VAR:
        case SL_TOKEN_TYPE_FLOAT_VAR:
        case SL_TOKEN_TYPE_CHAR_VAR:
            fprintf(
                fout,
                "IDX: %lu\nType: %s\nLocation: %s\nOffset: %lu\n",
                i,
                type_lookup[iter->type],
                location_lookup[iter->data.int_var.location],
                iter->data.int_var.idx
            );
            break;
        default:
            break;
        }
    }
    return SL_ERR_OK;
}
