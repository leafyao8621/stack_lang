#ifndef CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_ASSIGN_H_
#define CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_ASSIGN_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_operator_assign(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_operator_assign(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    int64_t *op_a_int_var, op_b_int;
    double *op_a_float_var, op_b_float;
    char *op_a_char_var, op_b_char;
    String **op_a_str_var, *op_b_str;
    Idx offset;
    op_b_int = op_b_float = op_b_char = 0;
    op_a_int_var = NULL;
    op_a_float_var = NULL;
    op_a_char_var = NULL;
    op_a_str_var = NULL;
    op_b_str = NULL;
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
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
            op_a_int_var = (int64_t*)(interpreter->global.data + offset);
            break;
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_int_var =
                (int64_t*)
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
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_literal;
            *op_a_int_var = op_b_int;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(int64_t*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .int_var
                            .direct;
                break;
            default:
                break;
            }
            *op_a_int_var = op_b_int;
            break;
        case SL_TOKEN_TYPE_FLOAT_LITERAL:
            op_b_float =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_literal;
            *op_a_int_var = op_b_float;
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_float = *(double*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_float =
                    *(double*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .float_var
                            .direct;
                break;
            default:
                break;
            }
            *op_a_int_var = op_b_float;
            break;
        default:
            break;
        }
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
            op_a_float_var = (double*)(interpreter->global.data + offset);
            break;
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_float_var =
                (double*)
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
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .data
                .int_literal;
            *op_a_float_var = op_b_int;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(int64_t*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .int_var
                            .direct;
                break;
            default:
                break;
            }
            *op_a_float_var = op_b_int;
            break;
        case SL_TOKEN_TYPE_FLOAT_LITERAL:
            op_b_float =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_literal;
            *op_a_float_var = op_b_float;
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .float_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_float = *(double*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_float =
                    *(double*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .float_var
                            .direct;
                break;
            default:
                break;
            }
            *op_a_float_var = op_b_float;
            break;
        default:
            break;
        }
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
            op_a_char_var = (char*)(interpreter->global.data + offset);
            break;
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_char_var =
                (char*)
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .char_var
                        .direct;
        default:
            break;
        }
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_b_char =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_literal;
            *op_a_char_var = op_b_char;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_char = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_char =
                    *(char*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .char_var
                            .direct;
            default:
                break;
            }
            *op_a_char_var = op_b_char;
            break;
        default:
            break;
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
            op_a_str_var = (String**)(interpreter->global.data + offset);
            break;
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_str_var =
                (String**)
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .str_var
                        .direct;
            break;
        default:
            break;
        }
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_STR_LITERAL:
            *op_a_str_var =
                interpreter
                    ->parser
                    .str_literals
                    .data +
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .str_literal;
            break;
        case SL_TOKEN_TYPE_STR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .str_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .str_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_str = *(String**)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_str =
                    *(String**)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .str_var
                            .direct;
                break;
            default:
                break;
            }
            *op_a_str_var = op_b_str;
            break;
        default:
            break;
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}

#endif
