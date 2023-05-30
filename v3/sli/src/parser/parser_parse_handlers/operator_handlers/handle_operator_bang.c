#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_variable(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_operator_bang(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *early_exit) {
    int ret = 0;
    SLErrCode err = SL_ERR_OK;
    SLToken token_res;
    if (
        ((*iter)[1] >= 'A' && (*iter)[1] <= 'Z') ||
        ((*iter)[1] >= 'a' && (*iter)[1] <= 'z')) {
        err = handle_variable(parser, buffer, iter);
        *early_exit = true;
        return err;
    }
    switch ((*iter)[1]) {
    case ' ':
    case '\t':
    case '\n':
    case '\0':
        token->data.operator_type = SL_OPERATOR_TYPE_LNOT;
        break;
    case '=':
        token->data.operator_type = SL_OPERATOR_TYPE_NEQ;
        ++(*iter);
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    ret = DArraySLToken_pop_back(&buffer->operation_stack);
    if (ret) {
        return SL_ERR_MISSING_OPERAND;
    }
    switch (token->data.operator_type) {
    case SL_OPERATOR_TYPE_LNOT:
        switch (
            buffer
                ->operation_stack
                .data[buffer->operation_stack.size]
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
    case SL_OPERATOR_TYPE_NEQ:
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
            switch (
                buffer
                    ->operation_stack
                    .data[buffer->operation_stack.size + 1]
                    .type) {
            case SL_TOKEN_TYPE_INT_LITERAL:
            case SL_TOKEN_TYPE_INT_VAR:
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
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
    default:
        break;
    }
    return SL_ERR_OK;
}
