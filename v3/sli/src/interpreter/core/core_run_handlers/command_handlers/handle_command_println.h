#ifndef CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_PRINTLN_H_
#define CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_PRINTLN_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_command_println(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_command_println(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    int64_t op_int;
    double op_float;
    char op_char, *op_str;
    Idx offset;
    op_int = op_float = op_char = 0;
    op_str = NULL;
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
        printf("%ld\n", op_int);
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_int =
                *(int64_t*)
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .int_var
                        .direct;
            break;
        default:
            break;
        }
        printf("%ld\n", op_int);
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        printf("%lf\n", op_float);
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_float =
                *(double*)
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .float_var
                        .direct;
            break;
        default:
            break;
        }
        printf("%lf\n", op_float);
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
        printf("%c\n", op_char);
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
            break;
        default:
            break;
        }
        printf("%c\n", op_char);
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
        printf("%s\n", op_str);
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_str =
                (
                    *(String**)
                        (
                            interpreter
                                ->operation_stack
                                .data[interpreter->operation_stack.size]
                                .data
                                .str_var
                                .direct
                        )
                )->data;
            break;
        default:
            break;
        }
        printf("%s\n", op_str);
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}

#endif
