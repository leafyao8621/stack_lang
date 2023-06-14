#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_operator_dec(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    int64_t *op_a_int;
    char *op_a_char;
    Idx offset;
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
            op_a_int = (int64_t*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        --(*op_a_int);
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
            op_a_char = (char*)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        --(*op_a_char);
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
