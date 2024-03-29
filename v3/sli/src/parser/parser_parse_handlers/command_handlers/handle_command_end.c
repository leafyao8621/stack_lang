#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_end(
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
        if (!buffer->global && buffer->name && !buffer->par) {
            if (parser->functions.data[buffer->cur_function].ret) {
                if (buffer->operation_stack.size != 1) {
                    return SL_ERR_FUNCTION_RET_MISMATCH;
                }
                switch (parser->functions.data[buffer->cur_function].ret_type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                    switch (buffer->operation_stack.data->type) {
                    case SL_TOKEN_TYPE_INT_LITERAL:
                    case SL_TOKEN_TYPE_INT_VAR:
                        break;
                    default:
                        return SL_ERR_FUNCTION_RET_MISMATCH;
                    }
                    break;
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                    switch (buffer->operation_stack.data->type) {
                    case SL_TOKEN_TYPE_FLOAT_LITERAL:
                    case SL_TOKEN_TYPE_FLOAT_VAR:
                        break;
                    default:
                        return SL_ERR_FUNCTION_RET_MISMATCH;
                    }
                    break;
                case SL_TOKEN_TYPE_CHAR_LITERAL:
                    switch (buffer->operation_stack.data->type) {
                    case SL_TOKEN_TYPE_CHAR_LITERAL:
                    case SL_TOKEN_TYPE_CHAR_VAR:
                        break;
                    default:
                        return SL_ERR_FUNCTION_RET_MISMATCH;
                    }
                    break;
                case SL_TOKEN_TYPE_STR_LITERAL:
                    switch (buffer->operation_stack.data->type) {
                    case SL_TOKEN_TYPE_STR_LITERAL:
                    case SL_TOKEN_TYPE_STR_VAR:
                        break;
                    default:
                        return SL_ERR_FUNCTION_RET_MISMATCH;
                    }
                    break;
                default:
                    return SL_ERR_INVALID_COMMAND;
                }
                DArraySLToken_clear(&buffer->operation_stack);
            } else {
                if (buffer->operation_stack.size) {
                    return SL_ERR_FUNCTION_RET_MISMATCH;
                }
            }
            buffer->global = true;
            buffer->name = false;
            *push_token = false;
            *push_control = false;
            *push_control_extra = false;
            SLToken token;
            token.type = SL_TOKEN_TYPE_COMMAND;
            token.data.command.type = SL_COMMAND_TYPE_RETURN;
            int ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            buffer->cur_token_buf = &parser->code;
            return SL_ERR_OK;
        }
        return SL_ERR_UNBALANCED;
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
                        return SL_ERR_UNBALANCED;
                    }
                }
                DArrayIdx_clear(&buffer->control_extra_stack);
                break;
            default:
                return SL_ERR_UNBALANCED;
            }
            break;
        case SL_COMMAND_TYPE_DO_FOR:
            ret = DArrayIdx_pop_back(&buffer->control_stack);
            if (ret) {
                return SL_ERR_UNBALANCED;
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
                        return SL_ERR_UNBALANCED;
                    }
                }
                DArrayIdx_clear(&buffer->control_extra_stack);
                break;
            default:
                return SL_ERR_UNBALANCED;
            }
            break;
        default:
            return SL_ERR_UNBALANCED;
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
