#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_operator_eq(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token) {
    int ret = 0;
    SLToken token_res;
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
        case SL_TOKEN_TYPE_STR_LITERAL:
        case SL_TOKEN_TYPE_STR_VAR:
            break;
        default:
            return SL_ERR_TYPE_MISMATCH;
        }
        break;
    default:
        return SL_ERR_TYPE_MISMATCH;
    }
    return SL_ERR_OK;
}
