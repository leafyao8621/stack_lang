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

extern inline void SLFunction_finalize(SLFunction *function) {
    HashMapSLVariableTypeNameIdxNode *iter_par_lookup =
        function->par_lookup.data;
    for (
        size_t i = 0;
        i < function->par_lookup.capacity;
        ++i,
        ++iter_par_lookup) {
        if (iter_par_lookup->in_use) {
            DArrayChar_finalize(&iter_par_lookup->key.name);
        }
    }
    HashMapSLVariableTypeNameIdx_finalize(&function->par_lookup);
    HashMapSLVariableTypeNameIdxNode *iter_local_lookup =
        function->local_lookup.data;
    for (
        size_t i = 0;
        i < function->local_lookup.capacity;
        ++i,
        ++iter_local_lookup) {
        if (iter_local_lookup->in_use) {
            DArrayChar_finalize(&iter_local_lookup->key.name);
        }
    }
    HashMapSLVariableTypeNameIdx_finalize(&function->local_lookup);
    DArraySLToken_finalize(&function->code);
}

SLErrCode SLParser_finalize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    DArraySLToken_finalize(&parser->code);
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
    String *iter_str_literals = parser->str_literals.data;
    for (
        size_t i = 0;
        i < parser->str_literals.size;
        ++i,
        ++iter_str_literals) {
        DArrayChar_finalize(iter_str_literals);
    }
    DArrayString_finalize(&parser->str_literals);
    HashMapSLVariableTypeNameIdxNode *iter_function_lookup =
        parser->function_lookup.data;
    for (
        size_t i = 0;
        i < parser->function_lookup.capacity;
        ++i,
        ++iter_function_lookup) {
        if (iter_function_lookup->in_use) {
            DArrayChar_finalize(&iter_function_lookup->key.name);
        }
    }
    HashMapSLVariableTypeNameIdx_finalize(&parser->function_lookup);
    SLFunction *iter_functions = parser->functions.data;
    for (
        size_t i = 0;
        i < parser->functions.size;
        ++i, ++iter_functions) {
        SLFunction_finalize(iter_functions);
    }
    DArraySLFunction_finalize(&parser->functions);
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
    static const char *location_lookup[4] = {
        "SL_VARIABLE_LOCATION_LOCAL",
        "SL_VARIABLE_LOCATION_PARAMETER",
        "SL_VARIABLE_LOCATION_GLOBAL",
        "SL_VARIABLE_LOCATION_DIRECT"
    };
    static const char *type_lookup[13] = {
        "SL_TOKEN_TYPE_INT_LITERAL",
        "SL_TOKEN_TYPE_INT_VAR",
        "SL_TOKEN_TYPE_FLOAT_LITERAL",
        "SL_TOKEN_TYPE_FLOAT_VAR",
        "SL_TOKEN_TYPE_CHAR_LITERAL",
        "SL_TOKEN_TYPE_CHAR_VAR",
        "SL_TOKEN_TYPE_STR_LITERAL",
        "SL_TOKEN_TYPE_STR_VAR",
        "SL_TOKEN_TYPE_ARR",
        "SL_TOKEN_TYPE_ARR_IMMEDIATE",
        "SL_TOKEN_TYPE_OPERATOR",
        "SL_TOKEN_TYPE_COMMAND",
        "SL_TOKEN_TYPE_FUNCTION"
    };
    static const char *operator_lookup[35] = {
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
        "<<=",
        ">>=",
        ">>>=",
        "&=",
        "|=",
        "^=",
        "[]"
    };
    static const char *command_lookup[28] = {
        "print",
        "println",
        "printhex",
        "printe",
        "input",
        "srand",
        "rand",
        "alloc",
        "realloc",
        "if",
        "else",
        "end",
        "while",
        "do",
        "end",
        "for",
        "do",
        "step",
        "end",
        "break",
        "continue",
        "struct",
        "end",
        "def",
        "begin",
        "end",
        "return",
        "halt"
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
    uint64_t *float_int_ptr = 0;
    for (size_t i = 0; i < parser->code.size; ++i, ++iter) {
        fprintf(
            fout,
            "Idx: %lu\nType: %s\n",
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
            float_int_ptr = (uint64_t*)&iter->data.float_literal;
            fprintf(
                fout,
                "DEC: %lE\nHEX: 0x%016lX\n",
                iter->data.float_literal,
                *float_int_ptr
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
                type_lookup[iter->data.arr.type]
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
                operator_lookup[iter->data.operator_type]
            );
            break;
        case SL_TOKEN_TYPE_COMMAND:
            fprintf(
                fout,
                "Command: %s\n",
                command_lookup[iter->data.command.type]
            );
            switch (iter->data.command.type) {
            case SL_COMMAND_TYPE_IF:
            case SL_COMMAND_TYPE_ELSE:
            case SL_COMMAND_TYPE_DO_WHILE:
            case SL_COMMAND_TYPE_END_WHILE:
            case SL_COMMAND_TYPE_DO_FOR:
            case SL_COMMAND_TYPE_END_FOR:
            case SL_COMMAND_TYPE_BREAK:
            case SL_COMMAND_TYPE_CONTINUE:
                fprintf(
                    fout,
                    "Tgt: %lu\n",
                    iter->data.command.tgt
                );
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_FUNCTION:
            fprintf(
                fout,
                "Offset: %lu\n",
                iter->data.function
            );
            break;
        default:
            break;
        }
    }
    fputs("Function Lookup:\n", fout);
    HashMapSLVariableTypeNameIdxNode *iter_function_lookup =
        parser->function_lookup.data;
    for (
        size_t i = 0;
        i < parser->function_lookup.capacity;
        ++i,
        ++iter_function_lookup) {
        if (iter_function_lookup->in_use) {
            fprintf(
                fout,
                "Name: %s\nOffset: %lu\n",
                iter_function_lookup->key.name.data,
                iter_function_lookup->value
            );
        }
    }
    fputs("Functions:\n", fout);
    SLFunction *iter_functions = parser->functions.data;
    for (
        size_t i = 0;
        i < parser->functions.size;
        ++i, ++iter_functions) {
        fprintf(fout, "Idx: %lu\n", i);
        if (iter_functions->ret) {
            fprintf(fout, "Ret: %s\n", type_lookup[iter_functions->ret_type]);
        }
        fputs("Parameters:\n", fout);
        HashMapSLVariableTypeNameIdxNode *iter_par_lookup =
            iter_functions->par_lookup.data;
        for (
            size_t i = 0;
            i < iter_functions->par_lookup.capacity;
            ++i,
            ++iter_par_lookup) {
            if (iter_par_lookup->in_use) {
                fprintf(
                    fout,
                    "Name: %s\nType: %s\nOffset: %lu\n",
                    iter_par_lookup->key.name.data,
                    type_lookup[iter_par_lookup->key.type],
                    iter_par_lookup->value
                );
            }
        }
        fputs("Local Variables:\n", fout);
        HashMapSLVariableTypeNameIdxNode *iter_local_lookup =
            iter_functions->local_lookup.data;
        for (
            size_t i = 0;
            i < iter_functions->local_lookup.capacity;
            ++i,
            ++iter_local_lookup) {
            if (iter_local_lookup->in_use) {
                fprintf(
                    fout,
                    "Name: %s\nType: %s\nOffset: %lu\n",
                    iter_local_lookup->key.name.data,
                    type_lookup[iter_local_lookup->key.type],
                    iter_local_lookup->value
                );
            }
        }
        fputs("Code:\n", fout);
        SLToken *iter = iter_functions->code.data;
        uint64_t *float_int_ptr = 0;
        for (size_t j = 0; j < iter_functions->code.size; ++j, ++iter) {
            fprintf(
                fout,
                "Idx: %lu\nType: %s\n",
                j,
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
                float_int_ptr = (uint64_t*)&iter->data.float_literal;
                fprintf(
                    fout,
                    "DEC: %lE\nHEX: 0x%016lX\n",
                    iter->data.float_literal,
                    *float_int_ptr
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
                    type_lookup[iter->data.arr.type]
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
                    operator_lookup[iter->data.operator_type]
                );
                break;
            case SL_TOKEN_TYPE_COMMAND:
                fprintf(
                    fout,
                    "Command: %s\n",
                    command_lookup[iter->data.command.type]
                );
                switch (iter->data.command.type) {
                case SL_COMMAND_TYPE_IF:
                case SL_COMMAND_TYPE_ELSE:
                case SL_COMMAND_TYPE_DO_WHILE:
                case SL_COMMAND_TYPE_END_WHILE:
                case SL_COMMAND_TYPE_DO_FOR:
                case SL_COMMAND_TYPE_END_FOR:
                case SL_COMMAND_TYPE_BREAK:
                case SL_COMMAND_TYPE_CONTINUE:
                    fprintf(
                        fout,
                        "Tgt: %lu\n",
                        iter->data.command.tgt
                    );
                    break;
                default:
                    break;
                }
                break;
            case SL_TOKEN_TYPE_FUNCTION:
                fprintf(
                    fout,
                    "Offset: %lu\n",
                    iter->data.function
                );
                break;
            default:
                break;
            }
        }
    }
    return SL_ERR_OK;
}
