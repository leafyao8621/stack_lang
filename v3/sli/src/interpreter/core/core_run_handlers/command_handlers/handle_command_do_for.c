#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_do_for(SLInterpreter *interpreter) {
    // int64_t *op_int_var;
    // char *op_char_var;
    // Idx offset;
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_INT_VAR:
        // offset =
        //     interpreter
        //         ->operation_stack
        //         .data[interpreter->operation_stack.size]
        //         .data
        //         .int_var
        //         .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .int_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            // op_int_var = (int64_t*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        break;
    case SL_TOKEN_TYPE_CHAR_VAR:
        // offset =
        //     interpreter
        //         ->operation_stack
        //         .data[interpreter->operation_stack.size]
        //         .data
        //         .char_var
        //         .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            // op_char_var = (char*)(interpreter->global.data + offset);
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
