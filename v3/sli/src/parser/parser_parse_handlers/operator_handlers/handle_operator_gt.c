#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_operator_gt(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token) {
    int ret = 0;
    SLToken token_res;
    switch ((*iter)[1]) {
    case ' ':
    case '\t':
    case '\n':
    case '\0':
        token->data.operator_type = SL_OPERATOR_TYPE_GT;
        break;
    case '=':
        token->data.operator_type = SL_OPERATOR_TYPE_GEQ;
        ++(*iter);
        break;
    case '>':
        switch ((*iter)[2]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token->data.operator_type = SL_OPERATOR_TYPE_RSHIFT;
            ++(*iter);
            break;
        case '=':
            token->data.operator_type = SL_OPERATOR_TYPE_RSHIFT_ASSIGN;
            *iter += 2;
            break;
        case '>':
            switch ((*iter)[3]) {
            case ' ':
            case '\t':
            case '\n':
            case '\0':
                token->data.operator_type = SL_OPERATOR_TYPE_RLSHIFT;
                *iter += 2;
                break;
            case '=':
                token->data.operator_type = SL_OPERATOR_TYPE_RLSHIFT_ASSIGN;
                *iter += 3;
                break;
            default:
                return SL_ERR_INVALID_OPERATOR;
            }
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    switch (token->data.operator_type) {
    case SL_OPERATOR_TYPE_RSHIFT:
    case SL_OPERATOR_TYPE_RLSHIFT:
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
        case SL_TOKEN_TYPE_INT_LITERAL:
        case SL_TOKEN_TYPE_INT_VAR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
            case SL_TOKEN_TYPE_INT_VAR:
                token_res.type = SL_TOKEN_TYPE_INT_LITERAL;
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
        case SL_TOKEN_TYPE_CHAR_LITERAL:
        case SL_TOKEN_TYPE_CHAR_VAR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
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
        default:
            return SL_ERR_TYPE_MISMATCH;
        }
        break;
    case SL_OPERATOR_TYPE_GT:
    case SL_OPERATOR_TYPE_GEQ:
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
        case SL_TOKEN_TYPE_INT_LITERAL:
        case SL_TOKEN_TYPE_INT_VAR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
            case SL_TOKEN_TYPE_INT_VAR:
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
        case SL_TOKEN_TYPE_CHAR_LITERAL:
        case SL_TOKEN_TYPE_CHAR_VAR:
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
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
        default:
            return SL_ERR_TYPE_MISMATCH;
        }
        break;
    case SL_OPERATOR_TYPE_RSHIFT_ASSIGN:
    case SL_OPERATOR_TYPE_RLSHIFT_ASSIGN:
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
                return SL_ERR_TYPE_MISMATCH;
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
                break;
            default:
                return SL_ERR_TYPE_MISMATCH;
            }
            break;
        default:
            return SL_ERR_TYPE_MISMATCH;
        }
    default:
        break;
    }
    return SL_ERR_OK;
}
