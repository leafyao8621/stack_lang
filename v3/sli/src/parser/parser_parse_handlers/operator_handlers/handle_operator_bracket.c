#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_operator_bracket(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token) {
    int ret = 0;
    SLToken token_res;
    switch ((*iter)[1]) {
    case ']':
        token->data.operator_type = SL_OPERATOR_TYPE_INDEX;
        ++(*iter);
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    size_t prev_dim;
    switch (token->data.operator_type) {
    case SL_OPERATOR_TYPE_INDEX:
        ret = DArraySLToken_pop_back(&buffer->operation_stack);
        if (ret) {
            return SL_ERR_MISSING_OPERAND;
        }
        ret = DArraySLToken_pop_back(&buffer->operation_stack);
        if (ret) {
            return SL_ERR_MISSING_OPERAND;
        }
        switch (
            buffer
                ->operation_stack
                .data[buffer->operation_stack.size]
                .type) {
        case SL_TOKEN_TYPE_STR_LITERAL:
        case SL_TOKEN_TYPE_STR_VAR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
            case SL_TOKEN_TYPE_INT_VAR:
            case SL_TOKEN_TYPE_CHAR_LITERAL:
            case SL_TOKEN_TYPE_CHAR_VAR:
                token_res.type = SL_TOKEN_TYPE_CHAR_LITERAL;
                ret =
                    DArraySLToken_push_back(
                        &buffer->operation_stack,
                        &token_res
                    );
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
            default:
                return SL_ERR_TYPE_MISMATCH;
            }
            break;
        case SL_TOKEN_TYPE_ARR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
            case SL_TOKEN_TYPE_INT_VAR:
            case SL_TOKEN_TYPE_CHAR_LITERAL:
            case SL_TOKEN_TYPE_CHAR_VAR:
                prev_dim =
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size]
                        .data
                        .arr
                        .dim;
                if (prev_dim > 1) {
                    token_res.type = SL_TOKEN_TYPE_ARR;
                    token_res.data.arr.dim = prev_dim - 1;
                    token_res.data.arr.type =
                        buffer
                            ->operation_stack
                            .data[buffer->operation_stack.size]
                            .data
                            .arr
                            .type;
                } else {
                    token_res.type =
                        buffer
                            ->operation_stack
                            .data[buffer->operation_stack.size]
                            .data
                            .arr
                            .type;
                }
                ret =
                    DArraySLToken_push_back(
                        &buffer->operation_stack,
                        &token_res
                    );
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
            default:
                return SL_ERR_TYPE_MISMATCH;
            }
            break;
        default:
            return SL_ERR_TYPE_MISMATCH;
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
