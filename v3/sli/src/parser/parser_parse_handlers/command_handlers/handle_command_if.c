#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_if(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    int ret = DArraySLToken_pop_back(&buffer->operation_stack);
    if (ret) {
        return SL_ERR_MISSING_OPERAND;
    }
    switch (
        buffer
            ->operation_stack
            .data[buffer->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_INT_LITERAL:
    case SL_TOKEN_TYPE_INT_VAR:
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
    case SL_TOKEN_TYPE_FLOAT_VAR:
    case SL_TOKEN_TYPE_CHAR_LITERAL:
    case SL_TOKEN_TYPE_CHAR_VAR:
        break;
    default:
        return SL_ERR_INVALID_COMMAND;
    }
    *push_token = true;
    *push_control = true;
    *push_control_extra = false;
    return SL_ERR_OK;
}
