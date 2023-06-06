#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_printe(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token) {
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
    case SL_TOKEN_TYPE_FLOAT_LITERAL:
    case SL_TOKEN_TYPE_FLOAT_VAR:
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    return SL_ERR_OK;
}
