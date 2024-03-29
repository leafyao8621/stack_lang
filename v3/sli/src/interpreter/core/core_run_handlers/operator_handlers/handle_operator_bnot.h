#ifndef CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_BNOT_H_
#define CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_BNOT_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_operator_bnot(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_operator_bnot(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    SLToken res;
    int64_t op_a_int;
    double op_a_float;
    char op_a_char;
    Idx offset;
    uint64_t *float_int;
    op_a_int = op_a_float = op_a_char = offset = 0;
    float_int = NULL;
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
        res.type = SL_TOKEN_TYPE_INT_LITERAL;
        res.data.int_literal = ~op_a_int;
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_int =
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
        res.type = SL_TOKEN_TYPE_INT_LITERAL;
        res.data.int_literal = ~op_a_int;
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_a_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
        float_int = (uint64_t*)&op_a_float;
        *float_int = ~(*float_int);
        res.data.float_literal = *(double*)float_int;
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_float =
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
        res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
        float_int = (uint64_t*)&op_a_float;
        *float_int = ~(*float_int);
        res.data.float_literal = *(double*)float_int;
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_a_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
        res.type = SL_TOKEN_TYPE_CHAR_LITERAL;
        res.data.char_literal = ~op_a_char;
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
        case SL_VARIABLE_LOCATION_DIRECT:
            op_a_char =
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
        res.type = SL_TOKEN_TYPE_CHAR_LITERAL;
        res.data.char_literal = ~op_a_char;
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

#endif
