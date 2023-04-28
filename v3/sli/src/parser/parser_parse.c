#include <stdio.h>

#include "parser.h"

DEF_DARRAY(Idx)

DEF_DARRAY_FUNCTIONS(Idx)

struct SLParserBuffer {
    String token_buf;
    HashMapSLVariableTypeNameIdx str_literal_lookup;
    DArraySLToken *cur_token_buf;
    bool global;
    Idx cur_function, global_offset;
    DArrayIdx parameter_offsets, local_offsets;
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
    buffer->global = true;
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
    return SL_ERR_OK;
}

void SLParserBuffer_finalize(struct SLParserBuffer *buffer) {
    DArrayChar_finalize(&buffer->token_buf);
    HashMapSLVariableTypeNameIdx_finalize(&buffer->str_literal_lookup);
    DArrayIdx_finalize(&buffer->parameter_offsets);
    DArrayIdx_finalize(&buffer->local_offsets);
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

SLErrCode handle_variable(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    SLVariableTypeName vtn;
    switch (**iter) {
    case '%':
        vtn.type = SL_TOKEN_TYPE_INT_VAR;
        ++(*iter);
        break;
    case '#':
        vtn.type = SL_TOKEN_TYPE_FLOAT_VAR;
        ++(*iter);
        break;
    case '&':
        vtn.type = SL_TOKEN_TYPE_CHAR_VAR;
        ++(*iter);
        break;
    }
    int ret = 0;
    for (
        size_t idx = 0;
        **iter &&
        **iter != ' ' &&
        **iter != '\t' &&
        **iter != '\n';
        ++idx,
        ++(*iter)) {
        if (
            (**iter >= 'A' && **iter <= 'Z') ||
            (**iter >= 'a' && **iter <= 'z')) {
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
        } else if (**iter == '_') {
            if (!idx) {
                return SL_ERR_INVALID_VARIABLE_NAME;
            }
            ret = DArrayChar_push_back(&buffer->token_buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
        } else {
            return SL_ERR_INVALID_VARIABLE_NAME;
        }
    }
    char chr = 0;
    ret = DArrayChar_push_back(&buffer->token_buf, &chr);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&vtn.name, buffer->token_buf.size);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        DArrayChar_push_back_batch(
            &vtn.name,
            buffer->token_buf.data,
            buffer->token_buf.size
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    DArrayChar_clear(&buffer->token_buf);
    SLToken token;
    token.type = vtn.type;
    bool found = false;
    Idx *offset = 0;
    if (buffer->global) {
        token.data.int_var.location = SL_VARIABLE_LOCATION_GLOBAL;
        HashMapSLVariableTypeNameIdx_find(&parser->global_lookup, &vtn, &found);
        if (found) {
            HashMapSLVariableTypeNameIdx_fetch(
                &parser->global_lookup,
                &vtn,
                &offset
            );
            DArrayChar_finalize(&vtn.name);
        } else {
            ret = HashMapSLVariableTypeNameIdx_fetch(
                &parser->global_lookup,
                &vtn,
                &offset
            );
            if (ret) {
                DArrayChar_finalize(&vtn.name);
                return SL_ERR_OUT_OF_MEMORY;
            }
            *offset = buffer->global_offset++;
        }
        switch (token.type) {
        case SL_TOKEN_TYPE_INT_VAR:
            token.data.int_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            token.data.float_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            token.data.char_var.idx = *offset;
            break;
        default:
            break;
        }
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
        case '%':
        case '#':
        case '&':
            err = handle_variable(parser, &buffer, &iter);
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
