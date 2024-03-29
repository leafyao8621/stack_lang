#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_else(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token || !parser) {
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
    *push_token = true;
    *push_control = true;
    *push_control_extra = false;
    return SL_ERR_OK;
}
