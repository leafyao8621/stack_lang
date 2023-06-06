#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_printhex(SLInterpreter *interpreter) {
    int64_t op_int;
    Idx offset;
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
    case SL_TOKEN_TYPE_STR_LITERAL:
        // printf(
        //     "%s",
        //     interpreter
        //         ->parser
        //         .str_literals
        //         .data
        //             [
        //                 interpreter
        //                     ->operation_stack
        //                     .data[interpreter->operation_stack.size]
        //                     .data
        //                     .str_literal
        //             ]
        //         .data
        // );
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
