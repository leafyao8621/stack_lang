#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_operator_tilde(
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
        token->data.operator_type = SL_OPERATOR_TYPE_BNOT;
        ++(*iter);
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    switch (token->data.operator_type) {
    case SL_OPERATOR_TYPE_BNOT:
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
    default:
        break;
    }
    return SL_ERR_OK;
}
