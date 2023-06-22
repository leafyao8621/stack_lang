#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_do_while(SLInterpreter *interpreter) {
    int64_t op_int;
    double op_float;
    char op_char;
    Idx offset;
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
        if (!op_int) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
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
            op_int = *(int64_t*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (!op_int) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
        }
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        if (!op_float) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
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
            op_float = *(double*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (!op_float) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
        }
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
        if (!op_char) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
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
            op_char = *(char*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (!op_char) {
            interpreter->current =
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt;
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
