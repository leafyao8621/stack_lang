#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_char_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    char chr = 0;
    SLToken token;
    token.type = SL_TOKEN_TYPE_CHAR_LITERAL;
    if ((*iter)[1] == '\\') {
        switch ((*iter)[2]) {
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
            if ((*iter)[3] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            token.data.char_literal = (*iter)[2] - '0';
            *iter += 3;
            break;
        case 't':
            if ((*iter)[3] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            token.data.char_literal = '\t';
            *iter += 3;
            break;
        case 'n':
            if ((*iter)[3] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            token.data.char_literal = '\n';
            *iter += 3;
            break;
        case '\'':
            if ((*iter)[3] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            token.data.char_literal = '\'';
            *iter += 3;
            break;
        case '\\':
            if ((*iter)[3] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            token.data.char_literal = '\\';
            *iter += 3;
            break;
        case 'x':
            ret = DArrayChar_push_back_batch(&buffer->token_buf, "0x", 2);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            ret = DArrayChar_push_back_batch(&buffer->token_buf, *iter + 3, 2);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            ret = DArrayChar_push_back(&buffer->token_buf, &chr);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            if ((*iter)[5] != '\'') {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            ret = sscanf(
                buffer->token_buf.data,
                "%hhx",
                (uint8_t*)&token.data.char_literal
            );
            if (ret == -1) {
                return SL_ERR_INVALID_CHAR_LITERAL;
            }
            DArrayChar_clear(&buffer->token_buf);
            *iter += 5;
            break;
        }
    } else {
        if ((*iter)[2] != '\'') {
            return SL_ERR_INVALID_CHAR_LITERAL;
        }
        if ((*iter)[1] == '\'') {
            return SL_ERR_INVALID_CHAR_LITERAL;
        }
        token.data.char_literal = (*iter)[1];
        *iter += 2;
    }
    ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArraySLToken_push_back(&buffer->operation_stack, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
