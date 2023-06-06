#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_command_print(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_command_printe(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

SLErrCode handle_command_halt(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token);

typedef SLErrCode (*Handler)(struct SLParserBuffer*, char**, SLToken*);

SLErrCode handle_command(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    static const char *command_lookup[22] = {
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
        "for",
        "step",
        "break",
        "continue",
        "struct",
        "def",
        "return",
        "halt"
    };
    Handler handlers[22] = {
        handle_command_print,
        handle_command_print,
        handle_command_print,
        handle_command_printe,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
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
    for (const char **i = command_lookup; idx < 20; ++idx, ++i) {
        if (!strcmp(*i, buffer->token_buf.data)) {
            break;
        }
    }
    if (idx == 20) {
        return SL_ERR_INVALID_COMMAND;
    }
    SLToken token;
    token.type = SL_TOKEN_TYPE_COMMAND;
    token.data.command.type = idx;
    SLErrCode err = handlers[idx](buffer, iter, &token);
    if (err) {
        return err;
    }
    DArrayChar_clear(&buffer->token_buf);
    ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
