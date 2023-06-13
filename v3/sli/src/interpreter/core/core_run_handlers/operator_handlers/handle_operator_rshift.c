#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_operator_rshift(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    SLToken res;
    int64_t op_a_int, op_b_int;
    double op_a_float;
    char op_a_char, op_b_char;
    Idx offset;
    uint64_t float_buf;
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_INT_LITERAL:
        op_a_int =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_literal;
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
            res.type = SL_TOKEN_TYPE_INT_LITERAL;
            res.data.int_literal = op_a_int << op_b_int;
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
            res.type = SL_TOKEN_TYPE_INT_LITERAL;
            res.data.int_literal = op_a_int >> op_b_int;
            break;
        default:
            break;
        }
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
            op_a_int = *(int64_t*)(interpreter->global.data + offset);
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
            res.type = SL_TOKEN_TYPE_INT_LITERAL;
            res.data.int_literal = op_a_int >> op_b_int;
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
            res.type = SL_TOKEN_TYPE_INT_LITERAL;
            res.data.int_literal = op_a_int >> op_b_int;
            break;
        default:
            break;
        }
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_a_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
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
            res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
            float_buf = (*(uint64_t*)&op_a_float) >> op_b_int;
            res.data.float_literal = *(double*)&float_buf;
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
            res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
            float_buf = (*(uint64_t*)&op_a_float) >> op_b_int;
            res.data.float_literal = *(double*)&float_buf;
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
            op_a_float = *(double*)(interpreter->global.data + offset);
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
            res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
            float_buf = (*(uint64_t*)&op_a_float) >> op_b_int;
            res.data.float_literal = *(double*)&float_buf;
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
            res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
            float_buf = (*(uint64_t*)&op_a_float) >> op_b_int;
            res.data.float_literal = *(double*)&float_buf;
            break;
        default:
            break;
        }
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_a_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
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
            break;
        default:
            break;
        }
        res.type = SL_TOKEN_TYPE_CHAR_LITERAL;
        res.data.char_literal = op_a_char >> op_b_char;
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
            op_a_char = *(char*)(interpreter->global.data + offset);
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
            break;
        default:
            break;
        }
        res.type = SL_TOKEN_TYPE_CHAR_LITERAL;
        res.data.char_literal = op_a_char >> op_b_char;
        break;
    default:
        break;
    }
    int ret = DArraySLToken_push_back(&interpreter->operation_stack, &res);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
