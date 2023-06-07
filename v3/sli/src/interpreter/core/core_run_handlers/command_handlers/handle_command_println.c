#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_println(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    int64_t op_int;
    double op_float;
    char op_char;
    Idx offset;
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
        printf("%ld\n", op_int);
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
        printf("%ld\n", op_int);
        break;
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
        op_float =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .float_literal;
        printf("%lf\n", op_float);
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
        printf("%lf\n", op_float);
        break;
    case SL_TOKEN_TYPE_CHAR_LITERAL:
        op_char =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .char_literal;
        printf("%c\n", op_char);
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
        printf("%c\n", op_char);
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
