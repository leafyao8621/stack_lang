#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_identifier(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLSymbolTableValue value, value_table;
    SLValue value_stack;
    SLErr reterr = SL_ERR_OK;
    switch (parser->state) {
    case SL_PARSER_STATE_GLOBAL:
        for (
            char *iter = parser->buf.data;
            *iter && *iter != ' ' && *iter != '\n' && *iter != '\t';
            ++iter) {
            if (
                !(
                    (
                        *iter >= 'A' &&
                        *iter <= 'Z'
                    ) ||
                    (
                        *iter >= 'a' &&
                        *iter <= 'z'
                    ) ||
                    (
                        *iter >= '0' &&
                        *iter <= '9'
                    ) ||
                    *iter == '_'
                )) {
                return SL_ERR_INVALID_IDENTIFIER;
            }
        }
        reterr =
            SLModule_find_identifier(
                module,
                parser->buf.data,
                &value_table
            );
        if (reterr) {
            return reterr;
        }
        value_stack.is_literal = false;
        value_stack.data.identifier.type = value_table.type;
        value_stack.data.identifier.data.variable.is_reference = false;
        value_stack.data.identifier.data.variable.location =
            value_table.location;
        value_stack.data.identifier.data.variable.offset =
            value_table.offset;
        ret =
            DArraySLValue_push_back(&parser->value_stack, &value_stack);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
        break;
    case SL_PARSER_STATE_DEFINITION_CHAR:
    case SL_PARSER_STATE_DEFINITION_INT8:
    case SL_PARSER_STATE_DEFINITION_UINT8:
    case SL_PARSER_STATE_DEFINITION_INT16:
    case SL_PARSER_STATE_DEFINITION_UINT16:
    case SL_PARSER_STATE_DEFINITION_INT32:
    case SL_PARSER_STATE_DEFINITION_UINT32:
    case SL_PARSER_STATE_DEFINITION_INT64:
    case SL_PARSER_STATE_DEFINITION_UINT64:
    case SL_PARSER_STATE_DEFINITION_FLOAT32:
    case SL_PARSER_STATE_DEFINITION_FLOAT64:
        switch (parser->state) {
        case SL_PARSER_STATE_DEFINITION_CHAR:
            value.type = SL_VALUE_TYPE_CHAR;
            break;
        case SL_PARSER_STATE_DEFINITION_INT8:
            value.type = SL_VALUE_TYPE_INT8;
            break;
        case SL_PARSER_STATE_DEFINITION_UINT8:
            value.type = SL_VALUE_TYPE_UINT8;
            break;
        case SL_PARSER_STATE_DEFINITION_INT16:
            value.type = SL_VALUE_TYPE_INT16;
            break;
        case SL_PARSER_STATE_DEFINITION_UINT16:
            value.type = SL_VALUE_TYPE_UINT16;
            break;
        case SL_PARSER_STATE_DEFINITION_INT32:
            value.type = SL_VALUE_TYPE_INT32;
            break;
        case SL_PARSER_STATE_DEFINITION_UINT32:
            value.type = SL_VALUE_TYPE_UINT32;
            break;
        case SL_PARSER_STATE_DEFINITION_INT64:
            value.type = SL_VALUE_TYPE_INT64;
            break;
        case SL_PARSER_STATE_DEFINITION_UINT64:
            value.type = SL_VALUE_TYPE_UINT64;
            break;
        case SL_PARSER_STATE_DEFINITION_FLOAT32:
            value.type = SL_VALUE_TYPE_FLOAT32;
            break;
        case SL_PARSER_STATE_DEFINITION_FLOAT64:
            value.type = SL_VALUE_TYPE_FLOAT64;
            break;
        default:
            break;
        }
        value.offset = parser->global_offset;
        switch (parser->prev_state) {
        case SL_PARSER_STATE_GLOBAL:
            value.location = SL_VALUE_VARIABLE_LOCATION_GLOBAL;
            switch (value.type) {
            case SL_VALUE_TYPE_CHAR:
            case SL_VALUE_TYPE_INT8:
            case SL_VALUE_TYPE_UINT8:
                ++parser->global_offset;
                break;
            case SL_VALUE_TYPE_INT16:
            case SL_VALUE_TYPE_UINT16:
                parser->global_offset += 2;
                break;
            case SL_VALUE_TYPE_INT32:
            case SL_VALUE_TYPE_UINT32:
            case SL_VALUE_TYPE_FLOAT32:
                parser->global_offset += 4;
                break;
            case SL_VALUE_TYPE_INT64:
            case SL_VALUE_TYPE_UINT64:
            case SL_VALUE_TYPE_FLOAT64:
                parser->global_offset += 8;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        for (
            char *iter = parser->buf.data;
            *iter && *iter != ' ' && *iter != '\n' && *iter != '\t';
            ++iter) {
            if (
                !(
                    (
                        *iter >= 'A' &&
                        *iter <= 'Z'
                    ) ||
                    (
                        *iter >= 'a' &&
                        *iter <= 'z'
                    ) ||
                    (
                        *iter >= '0' &&
                        *iter <= '9'
                    ) ||
                    *iter == '_'
                )) {
                return SL_ERR_INVALID_IDENTIFIER;
            }
        }
        reterr =
            SLModule_find_identifier(
                module,
                parser->buf.data,
                &value_table
            );
        if (!reterr) {
            return SL_ERR_DUPLICATE_IDENTIFIER;
        }
        if (reterr != SL_ERR_IDENTIFIER_NOT_FOUND) {
            return reterr;
        }
        ret =
            DArrayChar_push_back_batch(
                &module->symbol_table.symbol_buf,
                parser->buf.data,
                parser->buf.size
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
        ret =
            DArraySLSymbolTableValue_push_back(
                &module->symbol_table.table,
                &value
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
        parser->state = parser->prev_state;
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
