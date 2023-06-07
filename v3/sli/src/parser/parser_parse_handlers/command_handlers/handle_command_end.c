#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_end(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_control) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    int ret = DArrayIdx_pop_back(&buffer->control_stack);
    if (ret) {
        return SL_ERR_MISSING_OPERAND;
    }
    switch (
        buffer
            ->cur_token_buf
            ->data
            [
                buffer
                    ->control_stack
                    .data[buffer->control_stack.size]
            ]
            .type) {
    case SL_TOKEN_TYPE_COMMAND:
        switch (
            buffer
                ->cur_token_buf
                ->data
                [
                    buffer
                        ->control_stack
                        .data[buffer->control_stack.size]
                ]
                .data
                .command
                .type) {
        case SL_COMMAND_TYPE_IF:
        case SL_COMMAND_TYPE_ELSE:
            buffer
                ->cur_token_buf
                ->data
                [
                    buffer
                        ->control_stack
                        .data[buffer->control_stack.size]
                ]
                .data
                .command
                .tgt = buffer->cur_token_buf->size;
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
        break;
    default:
        return SL_ERR_INVALID_COMMAND;
    }
    *push_control = false;
    return SL_ERR_OK;
}
