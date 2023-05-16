#include <containers/eq.h>
#include <containers/hash.h>

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
    ret = DArrayString_initialize(&parser->str_literals, 10);
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
    String *iter_str_literals = parser->str_literals.data;
    for (
        size_t i = 0;
        i < parser->str_literals.size;
        ++i,
        ++iter_str_literals) {
        DArrayChar_finalize(iter_str_literals);
    }
    DArrayString_finalize(&parser->str_literals);
    return SL_ERR_OK;
}

void str_repr_log(char *str, FILE *fout) {
    for (char *i = str; *i; ++i) {
        if (*i >= 32 && !(*i & 0x80)) {
            if (*i == '"') {
                fputs("\\\"", fout);
            } else {
                fputc(*i, fout);
            }
        } else {
            switch (*i) {
            case '\n':
                fputs("\\n", fout);
                break;
            case '\t':
                fputs("\\t", fout);
                break;
            default:
                fprintf(fout, "\\x%02hhX", *i);
                break;
            }
        }
    }
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
    static const char *array_type_lookup[4] = {
        "SL_ARRAY_TYPE_INT",
        "SL_ARRAY_TYPE_FLOAT",
        "SL_ARRAY_TYPE_CHAR",
        "SL_ARRAY_TYPE_STR"
    };
    static const char *operator_lookup[29] = {
        "+",
        "-",
        "*",
        "/",
        "%",
        "<<",
        ">>",
        ">>>",
        "~",
        "&",
        "|",
        "^",
        "!",
        "&&",
        "||",
        "==",
        "!=",
        "<",
        ">",
        "<=",
        ">=",
        "=",
        "++",
        "--",
        "+=",
        "-=",
        "*=",
        "/=",
        "[]"
    };
    fputs("String Literals:\n", fout);
    String *iter_str_literals = parser->str_literals.data;
    for (
        size_t i = 0;
        i < parser->str_literals.size;
        ++i,
        ++iter_str_literals) {
        fprintf(
            fout,
            "Idx: %lu\nValue: \"",
            i
        );
        str_repr_log(iter_str_literals->data, fout);
        fputs("\"\n", fout);
    }
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
        fprintf(
            fout,
            "IDX: %lu\nType: %s\n",
            i,
            type_lookup[iter->type]
        );
        switch (iter->type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            fprintf(
                fout,
                "DEC: %ld\nHEX: 0x%016lX\n",
                iter->data.int_literal,
                iter->data.int_literal
            );
            break;
        case SL_TOKEN_TYPE_FLOAT_LITERAL:
            fprintf(
                fout,
                "DEC: %lE\nHEX: 0x%016lX\n",
                iter->data.float_literal,
                *(uint64_t*)&iter->data.float_literal
            );
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            fprintf(
                fout,
                "Location: %s\nOffset: %lu\n",
                location_lookup[iter->data.int_var.location],
                iter->data.int_var.idx
            );
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            fprintf(
                fout,
                "Location: %s\nOffset: %lu\n",
                location_lookup[iter->data.float_var.location],
                iter->data.float_var.idx
            );
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            fprintf(
                fout,
                "Location: %s\nOffset: %lu\n",
                location_lookup[iter->data.char_var.location],
                iter->data.char_var.idx
            );
            break;
        case SL_TOKEN_TYPE_STR_VAR:
            fprintf(
                fout,
                "Location: %s\nOffset: %lu\n",
                location_lookup[iter->data.str_var.location],
                iter->data.str_var.idx
            );
            break;
        case SL_TOKEN_TYPE_ARR:
            fprintf(
                fout,
                "Location: %s\nOffset: %lu\nDim: %lu\nType: %s\n",
                location_lookup[iter->data.arr.var_data.location],
                iter->data.arr.var_data.idx,
                iter->data.arr.dim,
                array_type_lookup[iter->data.arr.type]
            );
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            fprintf(
                fout,
                "CHAR: %s%c\nHEX: 0x%02hhX\n",
                iter->data.char_literal >= 32 &&
                !(iter->data.char_literal & 0x80) ?
                "" : "NON_PRINTABLE",
                iter->data.char_literal >= 32 &&
                !(iter->data.char_literal & 0x80) ?
                iter->data.char_literal :
                '*',
                iter->data.char_literal
            );
            break;
        case SL_TOKEN_TYPE_STR_LITERAL:
            fprintf(
                fout,
                "Offset: %lu\n",
                iter->data.str_literal
            );
            break;
        case SL_TOKEN_TYPE_OPERATOR:
            fprintf(
                fout,
                "Operator: %s\n",
                operator_lookup[iter->data.operator]
            );
        default:
            break;
        }
    }
    return SL_ERR_OK;
}
