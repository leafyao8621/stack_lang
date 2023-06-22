#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_printhex(SLInterpreter *interpreter) {
    int64_t op_int;
    double op_float;
    char op_char, *op_str;
    Idx offset;
    uint64_t *float_int_ptr;
    op_str = NULL;
    op_int = op_float = op_char = 0;
    DArraySLToken_pop_back(&interpreter->operation_stack);
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_INT_LITERAL:
        op_int =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_literal;
        printf("%016lX", op_int);
        break;
    case SL_TOKEN_TYPE_INT_VAR:
        offset =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_var
                .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            op_int = *(int64_t*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        printf("%016lX", op_int);
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        float_int_ptr = (uint64_t*)&op_float;
        printf("%016lX", *float_int_ptr);
        break;
    case SL_TOKEN_TYPE_FLOAT_VAR:
        offset =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_var
                .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            op_float = *(double*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        float_int_ptr = (uint64_t*)&op_float;
        printf("%016lX", *float_int_ptr);
        printf("%016lX", *float_int_ptr);
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
        printf("%02hhX", *(uint8_t*)&op_char);
        break;
    case SL_TOKEN_TYPE_CHAR_VAR:
        offset =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_var
                .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            op_char = *(char*)(interpreter->global.data + offset);
            break;
        case SL_VARIABLE_LOCATION_DIRECT:
            op_char =
                *(char*)
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .char_var
                        .direct;
        default:
            break;
        }
        printf("%02hhX", *(uint8_t*)&op_char);
        break;
    case SL_TOKEN_TYPE_STR_LITERAL:
        op_str =
            interpreter
                ->parser
                .str_literals
                .data
                    [
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size]
                            .data
                            .str_literal
                    ]
                .data;
        for (char *i = op_str; *i; ++i) {
            printf("%02hhX", *i);
        }
        break;
    case SL_TOKEN_TYPE_STR_VAR:
        offset =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .str_var
                .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .str_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            op_str = (*(String**)(interpreter->global.data + offset))->data;
            break;
        default:
            break;
        }
        for (char *i = op_str; *i; ++i) {
            printf("%02hhX", *i);
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
