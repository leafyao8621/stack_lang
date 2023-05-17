#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_number_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_variable(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_operator_percent(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *early_exit);

SLErrCode handle_operator(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLErrCode err = SL_ERR_OK;
    SLToken token, token_res;
    token.type = SL_TOKEN_TYPE_OPERATOR;
    bool early_exit = false;
    switch (**iter) {
    case '%':
        err =
            handle_operator_percent(parser, buffer, iter, &token, &early_exit);
        if (ret) {
            return err;
        }
        if (early_exit) {
            return SL_ERR_OK;
        }
        break;
    case '&':
        if (
            ((*iter)[1] >= 'A' && (*iter)[1] <= 'Z') ||
            ((*iter)[1] >= 'a' && (*iter)[1] <= 'z')) {
            err = handle_variable(parser, buffer, iter);
            return err;
        }
        switch ((*iter)[1]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token.data.operator = SL_OPERATOR_TYPE_BAND;
            break;
        case '&':
            token.data.operator = SL_OPERATOR_TYPE_LAND;
            ++(*iter);
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
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
    case '-':
        if ((*iter)[1] >= '0' && (*iter)[1] <= '9') {
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            ++(*iter);
            err = handle_number_literal(parser, buffer, iter);
            return err;
        }
        switch ((*iter)[1]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token.data.operator = SL_OPERATOR_TYPE_SUBTRACT;
            break;
        case '-':
            token.data.operator = SL_OPERATOR_TYPE_DEC;
            ++(*iter);
            break;
        case '=':
            token.data.operator = SL_OPERATOR_TYPE_SUBTRACT_ASSIGN;
            ++(*iter);
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        switch (token.data.operator) {
        case SL_OPERATOR_TYPE_SUBTRACT:
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
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
        case SL_OPERATOR_TYPE_DEC:
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
            case SL_TOKEN_TYPE_CHAR_VAR:
                break;
            default:
                return SL_ERR_TYPE_MISMATCH;
            }
            break;
        case SL_OPERATOR_TYPE_SUBTRACT_ASSIGN:
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
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
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
        break;
    case '+':
        switch ((*iter)[1]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token.data.operator = SL_OPERATOR_TYPE_ADD;
            break;
        case '+':
            token.data.operator = SL_OPERATOR_TYPE_INC;
            ++(*iter);
            break;
        case '=':
            token.data.operator = SL_OPERATOR_TYPE_ADD_ASSIGN;
            ++(*iter);
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        switch (token.data.operator) {
        case SL_OPERATOR_TYPE_ADD:
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
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
        case SL_OPERATOR_TYPE_INC:
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
            case SL_TOKEN_TYPE_CHAR_VAR:
                break;
            default:
                return SL_ERR_TYPE_MISMATCH;
            }
            break;
        case SL_OPERATOR_TYPE_ADD_ASSIGN:
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
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
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
        break;
    case '*':
        switch ((*iter)[1]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token.data.operator = SL_OPERATOR_TYPE_MULTIPLY;
            break;
        case '=':
            token.data.operator = SL_OPERATOR_TYPE_MULTIPLY_ASSIGN;
            ++(*iter);
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        switch (token.data.operator) {
        case SL_OPERATOR_TYPE_MULTIPLY:
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
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
        case SL_OPERATOR_TYPE_MULTIPLY_ASSIGN:
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
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
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
        break;
    case '/':
        switch ((*iter)[1]) {
        case ' ':
        case '\t':
        case '\n':
        case '\0':
            token.data.operator = SL_OPERATOR_TYPE_DIVIDE;
            break;
        case '=':
            token.data.operator = SL_OPERATOR_TYPE_DIVIDE_ASSIGN;
            ++(*iter);
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        switch (token.data.operator) {
        case SL_OPERATOR_TYPE_DIVIDE:
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
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
            case SL_TOKEN_TYPE_FLOAT_LITERAL:
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
                    token_res.type = SL_TOKEN_TYPE_FLOAT_LITERAL;
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
        case SL_OPERATOR_TYPE_DIVIDE_ASSIGN:
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
            case SL_TOKEN_TYPE_FLOAT_VAR:
                switch (
                    buffer
                        ->operation_stack
                        .data[buffer->operation_stack.size + 1]
                        .type) {
                case SL_TOKEN_TYPE_INT_LITERAL:
                case SL_TOKEN_TYPE_INT_VAR:
                case SL_TOKEN_TYPE_FLOAT_LITERAL:
                case SL_TOKEN_TYPE_FLOAT_VAR:
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
        break;
    case '<':
        switch ((*iter)[1]) {
        case '<':
            switch ((*iter)[2]) {
            case ' ':
            case '\t':
            case '\n':
            case '\0':
                token.data.operator = SL_OPERATOR_TYPE_LSHIFT;
                ++(*iter);
                break;
            case '=':
                token.data.operator = SL_OPERATOR_TYPE_LSHIFT_ASSIGN;
                *iter += 2;
                break;
            default:
                return SL_ERR_INVALID_OPERATOR;
            }
            break;
        default:
            return SL_ERR_INVALID_OPERATOR;
        }
        switch (token.data.operator) {
        case SL_OPERATOR_TYPE_LSHIFT:
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
        case SL_OPERATOR_TYPE_LSHIFT_ASSIGN:
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
        break;
    }
    ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
    if (ret) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
