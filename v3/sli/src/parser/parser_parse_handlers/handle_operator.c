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

SLErrCode handle_operator_ampersand(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *early_exit);

SLErrCode handle_operator_dash(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *early_exit);

SLErrCode handle_operator_plus(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_asterisk(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_slash(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_lt(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_gt(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_tilde(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_bang(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *early_exit);

SLErrCode handle_operator_eq(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator_bracket(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_operator(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLErrCode err = SL_ERR_OK;
    SLToken token;
    token.type = SL_TOKEN_TYPE_OPERATOR;
    bool early_exit = false;
    switch (**iter) {
    case '%':
        err =
            handle_operator_percent(parser, buffer, iter, &token, &early_exit);
        if (err) {
            return err;
        }
        if (early_exit) {
            return SL_ERR_OK;
        }
        break;
    case '&':
        err =
            handle_operator_ampersand(
                parser,
                buffer,
                iter,
                &token,
                &early_exit
            );
        if (err) {
            return err;
        }
        if (early_exit) {
            return SL_ERR_OK;
        }
        break;
    case '-':
        err =
            handle_operator_dash(
                parser,
                buffer,
                iter,
                &token,
                &early_exit
            );
        if (err) {
            return err;
        }
        if (early_exit) {
            return SL_ERR_OK;
        }
        break;
    case '+':
        err =
            handle_operator_plus(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '*':
        err =
            handle_operator_asterisk(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '/':
        err =
            handle_operator_slash(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '<':
        err =
            handle_operator_lt(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '>':
        err =
            handle_operator_gt(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '~':
        err =
            handle_operator_tilde(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '!':
        err =
            handle_operator_bang(
                parser,
                buffer,
                iter,
                &token,
                &early_exit
            );
        if (err) {
            return err;
        }
        if (early_exit) {
            return SL_ERR_OK;
        }
        break;
    case '=':
        err =
            handle_operator_eq(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
        break;
    case '[':
        err =
            handle_operator_bracket(
                buffer,
                iter,
                &token
            );
        if (err) {
            return err;
        }
    }
    ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
    if (ret) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
