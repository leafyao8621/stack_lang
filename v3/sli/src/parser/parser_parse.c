#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "parser.h"

DEF_DARRAY_FUNCTIONS(Idx)

DEF_HASHMAP_FUNCTIONS(String, Idx)

inline SLErrCode SLParserBuffer_initialize(struct SLParserBuffer *buffer) {
    int ret = DArrayChar_initialize(&buffer->token_buf, 100);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        HashMapStringIdx_initialize(
            &buffer->str_literal_lookup,
            10,
            containers_eq_dstr,
            containers_hash_dstr
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    buffer->global = true;
    buffer->name = false;
    buffer->par = false;
    buffer->cur_function = 0;
    buffer->global_offset = 0;
    ret = DArrayIdx_initialize(&buffer->parameter_offsets, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayIdx_initialize(&buffer->local_offsets, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArraySLToken_initialize(&buffer->operation_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayIdx_initialize(&buffer->control_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayIdx_initialize(&buffer->control_extra_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

inline void SLParserBuffer_finalize(struct SLParserBuffer *buffer) {
    DArrayChar_finalize(&buffer->token_buf);
    HashMapStringIdx_finalize(&buffer->str_literal_lookup);
    DArrayIdx_finalize(&buffer->parameter_offsets);
    DArrayIdx_finalize(&buffer->local_offsets);
    DArraySLToken_finalize(&buffer->operation_stack);
    DArrayIdx_finalize(&buffer->control_stack);
    DArrayIdx_finalize(&buffer->control_extra_stack);
}

SLErrCode handle_number_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_variable(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_char_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_str_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_operator(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_command(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode handle_function(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter);

SLErrCode SLParser_parse(SLParser *parser, char *str) {
    if (!parser || !str) {
        return SL_ERR_NULL_PTR;
    }
    struct SLParserBuffer buffer;
    SLParserBuffer_initialize(&buffer);
    buffer.cur_token_buf = &parser->code;
    SLErrCode err = SL_ERR_OK;
    char *iter = str;
    for (; *iter; ++iter) {
        switch (*iter) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            err = handle_number_literal(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '#':
        case '$':
        case '@':
            err = handle_variable(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '?':
            err = handle_function(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '\'':
            err = handle_char_literal(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '"':
            err = handle_str_literal(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '<':
        case '>':
        case '!':
        case '&':
        case '|':
        case '^':
        case '~':
        case '=':
        case '[':
            err = handle_operator(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        case '_':
            err = handle_command(parser, &buffer, &iter);
            if (err) {
                SLParserBuffer_finalize(&buffer);
                return err;
            }
            break;
        }
    }
    parser->global_size = buffer.global_offset;
    if (buffer.global) {
        if (buffer.control_stack.size) {
            SLParserBuffer_finalize(&buffer);
            return SL_ERR_UNBALANCED;
        }
        SLToken token;
        token.type = SL_TOKEN_TYPE_COMMAND;
        token.data.command.type = SL_COMMAND_TYPE_HALT;
        int ret = DArraySLToken_push_back(buffer.cur_token_buf, &token);
        if (ret) {
            SLParserBuffer_finalize(&buffer);
            return SL_ERR_OUT_OF_MEMORY;
        }
    } else {
        SLParserBuffer_finalize(&buffer);
        return SL_ERR_UNBALANCED;
    }
    SLParserBuffer_finalize(&buffer);
    return SL_ERR_OK;
}
