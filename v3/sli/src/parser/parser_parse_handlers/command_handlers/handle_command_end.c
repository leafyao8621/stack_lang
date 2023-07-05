#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_end(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    int ret = DArrayIdx_pop_back(&buffer->control_stack);
    if (ret) {
        return SL_ERR_MISSING_OPERAND;
    }
    Idx *iter_control_extra;
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
        case SL_COMMAND_TYPE_DO_WHILE:
            ret = DArrayIdx_pop_back(&buffer->control_stack);
            if (ret) {
                return SL_ERR_INVALID_COMMAND;
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
                    .data
                    .command
                    .type) {
            case SL_COMMAND_TYPE_WHILE:
                token->data.command.type = SL_COMMAND_TYPE_END_WHILE;
                token->data.command.tgt =
                    buffer
                        ->control_stack
                        .data[buffer->control_stack.size];
                buffer
                    ->cur_token_buf
                    ->data
                    [
                        buffer
                            ->control_stack
                            .data[buffer->control_stack.size + 1]
                    ]
                    .data
                    .command
                    .tgt = buffer->cur_token_buf->size;
                iter_control_extra =
                    buffer->control_extra_stack.data;
                for (
                    size_t i = 0;
                    i < buffer->control_extra_stack.size;
                    ++i, ++iter_control_extra) {
                    switch (
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .type) {
                    case SL_COMMAND_TYPE_BREAK:
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .tgt = buffer->cur_token_buf->size;
                        break;
                    case SL_COMMAND_TYPE_CONTINUE:
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .tgt =
                                buffer
                                    ->control_stack
                                    .data[buffer->control_stack.size];
                        break;
                    default:
                        return SL_ERR_INVALID_COMMAND;
                    }
                }
                DArrayIdx_clear(&buffer->control_extra_stack);
                break;
            default:
                return SL_ERR_INVALID_COMMAND;
            }
            break;
        case SL_COMMAND_TYPE_DO_FOR:
            ret = DArrayIdx_pop_back(&buffer->control_stack);
            if (ret) {
                return SL_ERR_INVALID_COMMAND;
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
                    .data
                    .command
                    .type) {
            case SL_COMMAND_TYPE_FOR:
                token->data.command.type = SL_COMMAND_TYPE_END_FOR;
                token->data.command.tgt =
                    buffer
                        ->control_stack
                        .data[buffer->control_stack.size];
                buffer
                    ->cur_token_buf
                    ->data
                    [
                        buffer
                            ->control_stack
                            .data[buffer->control_stack.size + 1]
                    ]
                    .data
                    .command
                    .tgt = buffer->cur_token_buf->size;
                iter_control_extra =
                    buffer->control_extra_stack.data;
                for (
                    size_t i = 0;
                    i < buffer->control_extra_stack.size;
                    ++i, ++iter_control_extra) {
                    switch (
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .type) {
                    case SL_COMMAND_TYPE_BREAK:
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .tgt = buffer->cur_token_buf->size;
                        break;
                    case SL_COMMAND_TYPE_CONTINUE:
                        buffer
                            ->cur_token_buf
                            ->data[*iter_control_extra]
                            .data
                            .command
                            .tgt =
                                buffer
                                    ->control_stack
                                    .data[buffer->control_stack.size];
                        break;
                    default:
                        return SL_ERR_INVALID_COMMAND;
                    }
                }
                DArrayIdx_clear(&buffer->control_extra_stack);
                break;
            default:
                return SL_ERR_INVALID_COMMAND;
            }
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
        break;
    default:
        return SL_ERR_INVALID_COMMAND;
    }
    *push_token = true;
    *push_control = false;
    *push_control_extra = false;
    return SL_ERR_OK;
}
