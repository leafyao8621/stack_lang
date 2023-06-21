#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_do(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    switch (
        buffer
            ->cur_token_buf
            ->data
            [
                buffer
                    ->control_stack
                    .data[buffer->control_stack.size - 1]
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
                        .data[buffer->control_stack.size - 1]
                ]
                .data
                .command
                .type) {
        case SL_COMMAND_TYPE_WHILE:
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
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
            case SL_TOKEN_TYPE_CHAR_LITERAL:
            case SL_TOKEN_TYPE_CHAR_VAR:
                break;
            default:
                return SL_ERR_INVALID_COMMAND;
            }
            break;
        case SL_COMMAND_TYPE_FOR:
            ret = DArraySLToken_pop_back(&buffer->operation_stack);
            if (ret) {
                return SL_ERR_MISSING_OPERAND;
            }
            ret = DArraySLToken_pop_back(&buffer->operation_stack);
            if (ret) {
                return SL_ERR_MISSING_OPERAND;
            }
            ret = DArraySLToken_pop_back(&buffer->operation_stack);
            if (ret) {
                return SL_ERR_MISSING_OPERAND;
            }
            token->data.command.type = SL_COMMAND_TYPE_DO_FOR;
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size]
                    .type) {
            case SL_TOKEN_TYPE_INT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                    switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                    case SL_TOKEN_TYPE_INT_LITERAL:
                    case SL_TOKEN_TYPE_INT_VAR:
                        break;
                    default:
                        return SL_ERR_INVALID_COMMAND;
                    }
                    break;
                default:
                    return SL_ERR_INVALID_COMMAND;
                }
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_CHAR_LITERAL:
                case SL_TOKEN_TYPE_CHAR_VAR:
                    switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                    case SL_TOKEN_TYPE_CHAR_LITERAL:
                    case SL_TOKEN_TYPE_CHAR_VAR:
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
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
        break;
    default:
        return SL_ERR_INVALID_COMMAND;
    }
    *push_control = true;
    *push_control_extra = false;
    return SL_ERR_OK;
}
