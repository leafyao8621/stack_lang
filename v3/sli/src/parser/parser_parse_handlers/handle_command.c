#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_command_print(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_printe(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_alloc(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_if(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_else(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_end(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_while(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_do(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_for(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_break(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_continue(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_def(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_begin(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

SLErrCode handle_command_halt(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra);

typedef SLErrCode (*Handler)(
    SLParser *,
    struct SLParserBuffer*,
    char**,
    SLToken*,
    bool*,
    bool*,
    bool*);

SLErrCode handle_command(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    static const char *command_lookup[28] = {
        "print",
        "println",
        "printhex",
        "printe",
        "input",
        "srand",
        "rand",
        "alloc",
        "realloc",
        "if",
        "else",
        "end",
        "while",
        "do",
        "end",
        "for",
        "do",
        "step",
        "end",
        "break",
        "continue",
        "struct",
        "end",
        "def",
        "begin",
        "end",
        "return",
        "halt"
    };
    Handler handlers[28] = {
        handle_command_print,
        handle_command_print,
        handle_command_print,
        handle_command_printe,
        NULL,
        NULL,
        NULL,
        handle_command_alloc,
        NULL,
        handle_command_if,
        handle_command_else,
        handle_command_end,
        handle_command_while,
        handle_command_do,
        handle_command_end,
        handle_command_for,
        handle_command_do,
        NULL,
        handle_command_end,
        handle_command_break,
        handle_command_continue,
        NULL,
        NULL,
        handle_command_def,
        handle_command_begin,
        NULL,
        NULL,
        handle_command_halt
    };
    int ret = 0;
    for (++(*iter); **iter >= 'a' && **iter <= 'z'; ++(*iter)) {
        ret = DArrayChar_push_back(&buffer->token_buf, *iter);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    char chr = 0;
    ret = DArrayChar_push_back(&buffer->token_buf, &chr);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    size_t idx = 0;
    for (const char **i = command_lookup; idx < 27; ++idx, ++i) {
        if (!strcmp(*i, buffer->token_buf.data)) {
            break;
        }
    }
    if (idx == 27) {
        return SL_ERR_INVALID_COMMAND;
    }
    SLToken token;
    token.type = SL_TOKEN_TYPE_COMMAND;
    token.data.command.type = idx;
    bool push_token = false;
    bool push_control = false;
    bool push_control_extra = false;
    SLErrCode err =
        handlers[idx](
            parser,
            buffer,
            iter,
            &token,
            &push_token,
            &push_control,
            &push_control_extra
        );
    if (err) {
        return err;
    }
    DArrayChar_clear(&buffer->token_buf);
    if (push_control) {
        ret =
            DArrayIdx_push_back(
                &buffer->control_stack,
                &buffer->cur_token_buf->size
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    if (push_control_extra) {
        ret =
            DArrayIdx_push_back(
                &buffer->control_extra_stack,
                &buffer->cur_token_buf->size
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    if (push_token) {
        ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    return SL_ERR_OK;
}
