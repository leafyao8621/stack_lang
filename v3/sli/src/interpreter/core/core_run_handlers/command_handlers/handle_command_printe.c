#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_printe(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        printf(
            "%E",
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal
        );
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
