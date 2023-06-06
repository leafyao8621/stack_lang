#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_printhex(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_INT_LITERAL:
        printf(
            "%016lX",
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_literal
        );
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
