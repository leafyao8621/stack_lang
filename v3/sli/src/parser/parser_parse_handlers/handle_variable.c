#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../parser.h"

SLErrCode handle_variable(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter) {
    SLVariableTypeName vtn;
    Idx dim;
    SLTokenType array_type;
    if (!parser || !buffer || !iter) {
        return SL_ERR_NULL_PTR;
    }
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
    case '$':
        vtn.type = SL_TOKEN_TYPE_STR_VAR;
        ++(*iter);
        break;
    case '@':
        vtn.type = SL_TOKEN_TYPE_ARR;
        for (dim = 0; **iter && **iter == '@'; ++dim, ++(*iter));
        switch (**iter) {
        case '%':
            array_type = SL_TOKEN_TYPE_INT_VAR;
            break;
        case '#':
            array_type = SL_TOKEN_TYPE_FLOAT_VAR;
            break;
        case '&':
            array_type = SL_TOKEN_TYPE_CHAR_VAR;
            break;
        case '$':
            array_type = SL_TOKEN_TYPE_STR_VAR;
            break;
        default:
            return SL_ERR_INVALID_VARIABLE_NAME;
        }
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
            *offset = buffer->global_offset;
            switch (token.type) {
            case SL_TOKEN_TYPE_INT_VAR:
                buffer->global_offset += 8;
                break;
            case SL_TOKEN_TYPE_FLOAT_VAR:
                buffer->global_offset += 8;
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                buffer->global_offset++;
                break;
            case SL_TOKEN_TYPE_STR_VAR:
                buffer->global_offset += 8;
                break;
            case SL_TOKEN_TYPE_ARR:
                buffer->global_offset += 8;
                break;
            default:
                break;
            }
        }
        switch (token.type) {
        case SL_TOKEN_TYPE_INT_VAR:
            token.data.int_var.location = SL_VARIABLE_LOCATION_GLOBAL;
            token.data.int_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            token.data.float_var.location = SL_VARIABLE_LOCATION_GLOBAL;
            token.data.float_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            token.data.char_var.location = SL_VARIABLE_LOCATION_GLOBAL;
            token.data.char_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_STR_VAR:
            token.data.str_var.location = SL_VARIABLE_LOCATION_GLOBAL;
            token.data.str_var.idx = *offset;
            break;
        case SL_TOKEN_TYPE_ARR:
            token.data.arr.var_data.location = SL_VARIABLE_LOCATION_GLOBAL;
            token.data.arr.var_data.idx = *offset;
            token.data.arr.dim = dim;
            token.data.arr.type = array_type;
            break;
        default:
            break;
        }
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
