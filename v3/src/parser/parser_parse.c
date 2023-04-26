#include <stdio.h>

#include "parser.h"

struct SLParserBuffer {
    String token_buf;
    HashMapSLVariableTypeNameIdx str_literal_lookup;
    DArraySLToken *cur_token_buf;
};

bool eq_sl_variable_type_name(SLVariableTypeName *a, SLVariableTypeName *b);
size_t hash_sl_variable_type_name(SLVariableTypeName *a);

SLErrCode SLParserBuffer_initialize(struct SLParserBuffer *buffer) {
    int ret = DArrayChar_initialize(&buffer->token_buf, 100);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        HashMapSLVariableTypeNameIdx_initialize(
            &buffer->str_literal_lookup,
            10,
            eq_sl_variable_type_name,
            hash_sl_variable_type_name
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

void SLParserBuffer_finalize(struct SLParserBuffer *buffer) {
    DArrayChar_finalize(&buffer->token_buf);
    HashMapSLVariableTypeNameIdx_finalize(&buffer->str_literal_lookup);
}

SLErrCode handle_number_literal(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    bool hex = false;
    bool dot = false;
    bool e = false;
    bool sign = false;
    for (
        size_t idx = 0;
        **iter &&
        **iter != ' ' &&
        **iter != '\t' &&
        **iter != '\n';
        ++idx,
        ++(*iter)) {
        switch (**iter) {
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
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        case 'x':
            if (idx != 1 || (*iter)[-1] != '0') {
                return SL_ERR_INVALID_INT_LITERAL;
            }
            if (hex) {
                return SL_ERR_INVALID_INT_LITERAL;
            }
            hex = true;
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        case 'A':
        case 'B':
        case 'C':
        case 'D':
        case 'F':
        case 'a':
        case 'b':
        case 'c':
        case 'd':
        case 'f':
            if (!hex) {
                return SL_ERR_INVALID_INT_LITERAL;
            }
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        case 'E':
        case 'e':
            if (!idx) {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            if (!hex) {
                if (e) {
                    return SL_ERR_INVALID_FLOAT_LITERAL;
                }
                e = true;
            }
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        case '.':
            if (hex) {
                return SL_ERR_INVALID_INT_LITERAL;
            }
            if (dot) {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            dot = true;
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        case '+':
        case '-':
            if (idx < 2) {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            if (!e) {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            if ((*iter)[-1] != 'E' && (*iter)[-1] != 'e') {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            if (sign) {
                return SL_ERR_INVALID_FLOAT_LITERAL;
            }
            sign = true;
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            break;
        default:
            return SL_ERR_INVALID_INT_LITERAL;
        }
    }
    char chr = 0;
    ret = DArrayChar_push_back(&buffer->token_buf, &chr);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    int64_t val = 0;
    double val_float = 0;
    if (hex) {
        sscanf(buffer->token_buf.data, "%lx", (uint64_t*)&val);
    } else if (e || dot) {
        sscanf(buffer->token_buf.data, "%lf", &val_float);
    } else {
        sscanf(buffer->token_buf.data, "%ld", &val);
    }
    DArrayChar_clear(&buffer->token_buf);
    SLToken token;
    token.type =
        e || dot ?
        SL_TOKEN_TYPE_FLOAT_LITERAL :
        SL_TOKEN_TYPE_INT_LITERAL;
    if (e || dot) {
        token.data.float_literal = val_float;
    } else {
        token.data.int_literal = val;
    }
    ret = DArraySLToken_push_back(buffer->cur_token_buf, &token);
    if (ret) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}

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
        }
    }
    SLParserBuffer_finalize(&buffer);
    return SL_ERR_OK;
}
