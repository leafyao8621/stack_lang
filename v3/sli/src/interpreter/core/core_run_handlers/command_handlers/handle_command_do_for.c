#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_do_for(SLInterpreter *interpreter) {
    int64_t *op_int_var, op_int_init, op_int_end;
    Idx offset;
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
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
            op_int_var = (int64_t*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (
            interpreter->control_stack.size &&
            (
                interpreter
                    ->control_stack
                    .data[interpreter->control_stack.size - 1] ==
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt
            )) {
            ++(*op_int_var);
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 2]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
                op_int_end =
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size + 2]
                        .data
                        .int_literal;
                break;
            case SL_TOKEN_TYPE_INT_VAR:
                offset =
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size + 2]
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
                    op_int_end = *(int64_t*)(interpreter->global.data + offset);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            if (*op_int_var > op_int_end) {
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
        } else {
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
                op_int_init =
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size + 1]
                        .data
                        .int_literal;
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
                        .data[interpreter->operation_stack.size]
                        .data
                        .int_var
                        .location) {
                case SL_VARIABLE_LOCATION_GLOBAL:
                    op_int_init =
                        *(int64_t*)(interpreter->global.data + offset);
                    break;
                default:
                    break;
                }
                break;
            default:
                break;
            }
            *op_int_var = op_int_init;
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
