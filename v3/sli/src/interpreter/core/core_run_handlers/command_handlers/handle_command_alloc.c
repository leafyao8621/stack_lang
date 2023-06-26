#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_alloc(SLInterpreter *interpreter) {
    bool found = false;
    for (; !found && interpreter->operation_stack.size;) {
        DArraySLToken_pop_back(&interpreter->operation_stack);
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
        case SL_TOKEN_TYPE_INT_VAR:
        case SL_TOKEN_TYPE_CHAR_LITERAL:
        case SL_TOKEN_TYPE_CHAR_VAR:
            break;
        case SL_TOKEN_TYPE_ARR:
            found = true;
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
    }


    return SL_ERR_OK;
}
