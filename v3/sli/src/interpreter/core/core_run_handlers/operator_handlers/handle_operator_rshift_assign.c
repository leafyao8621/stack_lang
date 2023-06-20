#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_operator_rshift_assign(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    int64_t *op_a_int_var, op_b_int;
    double *op_a_float_var;
    uint64_t float_buf;
    char *op_a_char_var, op_b_char;
    Idx offset;
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
            *op_a_int_var >>= op_b_int;
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
            default:
                break;
            }
            *op_a_int_var >>= op_b_int;
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
        default:
            break;
        }
        float_buf = *(uint64_t*)op_a_float_var;
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
            float_buf >>= op_b_int;
            *op_a_float_var = *(double*)&float_buf;
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
            default:
                break;
            }
            float_buf >>= op_b_int;
            *op_a_float_var = *(double*)&float_buf;
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
            *op_a_char_var >>= op_b_char;
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
            default:
                break;
            }
            *op_a_char_var >>= op_b_char;
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