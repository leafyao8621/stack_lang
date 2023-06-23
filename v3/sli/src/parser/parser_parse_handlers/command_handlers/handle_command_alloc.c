#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_alloc(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    bool found = false;
    size_t cnt = 0;
    for (; !found && buffer->operation_stack.size;) {
        ret = DArraySLToken_pop_back(&buffer->operation_stack);
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
        case SL_TOKEN_TYPE_CHAR_LITERAL:
        case SL_TOKEN_TYPE_CHAR_VAR:
            ++cnt;
            break;
        case SL_TOKEN_TYPE_ARR:
            found = true;
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
    }
    if (!found) {
        return SL_ERR_INVALID_COMMAND;
    }
    if (
        buffer
            ->operation_stack
            .data[buffer->operation_stack.size]
            .data
            .arr
            .dim != cnt) {
        return SL_ERR_INVALID_COMMAND;
    }
    *push_control = false;
    *push_control_extra = false;
    return SL_ERR_OK;
}
