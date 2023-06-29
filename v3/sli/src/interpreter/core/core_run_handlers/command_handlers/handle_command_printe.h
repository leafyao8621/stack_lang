#ifndef CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_PRINTE_H_
#define CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_PRINTE_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_command_printe(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_command_printe(SLInterpreter *interpreter) {
    double op_float;
    Idx offset;
    op_float = 0;
    DArraySLToken_pop_back(&interpreter->operation_stack);
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        printf("%E", op_float);
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
        printf("%E", op_float);
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}

#endif
