#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_str_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    char chr = 0;
    for (++(*iter); **iter && **iter != '"'; ++(*iter)) {
        char hex_buf[5] = {0};
        switch (**iter) {
        case '\n':
            return SL_ERR_INVALID_STR_LITERAL;
        case '\\':
            switch (*(++(*iter))) {
            case '"':
                ret = DArrayChar_push_back(&buffer->token_buf, *iter);
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
            case 't':
                chr = '\t';
                ret = DArrayChar_push_back(&buffer->token_buf, &chr);
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
            case 'n':
                chr = '\n';
                ret = DArrayChar_push_back(&buffer->token_buf, &chr);
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
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
                chr = **iter - '0';
                ret = DArrayChar_push_back(&buffer->token_buf, &chr);
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                break;
            case 'x':
                memcpy(hex_buf, "0x", 2);
                memcpy(hex_buf + 2, *iter + 1, 2);
                ret = sscanf(hex_buf, "%hhx", (uint8_t*)&chr);
                if (ret == -1) {
                    return SL_ERR_INVALID_STR_LITERAL;
                }
                ret = DArrayChar_push_back(&buffer->token_buf, &chr);
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
                (*iter) += 2;
                break;
            default:
                return SL_ERR_INVALID_STR_LITERAL;
            }
            break;
        default:
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        }
    }
    if (!**iter) {
        return SL_ERR_INVALID_STR_LITERAL;
    }
    chr = 0;
    ret = DArrayChar_push_back(&buffer->token_buf, &chr);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    SLToken token;
    token.type = SL_TOKEN_TYPE_STR_LITERAL;
    bool found = false;
    HashMapStringIdx_find(
        &buffer->str_literal_lookup,
        &buffer->token_buf,
        &found
    );
    if (!found) {
        String tmp;
        ret = DArrayChar_initialize(&tmp, buffer->token_buf.size);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
        DArrayChar_push_back_batch(
            &tmp,
            buffer->token_buf.data,
            buffer->token_buf.size
        );
        Idx *tgt;
        ret = HashMapStringIdx_fetch(&buffer->str_literal_lookup, &tmp, &tgt);
        if (ret) {
            DArrayChar_finalize(&tmp);
            return SL_ERR_OUT_OF_MEMORY;
        }
        *tgt = buffer->str_literal_lookup.size - 1;
        ret = DArrayString_push_back(&parser->str_literals, &tmp);
        if (ret) {
            DArrayChar_finalize(&tmp);
            return SL_ERR_OUT_OF_MEMORY;
        }
        token.data.str_literal = *tgt;
    } else {
        Idx *tgt;
        HashMapStringIdx_fetch(
            &buffer->str_literal_lookup, &buffer->token_buf,
            &tgt
        );
        token.data.str_literal = *tgt;
    }
    DArrayChar_clear(&buffer->token_buf);
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
