#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_identifier(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLSymbolTableValue value, value_table;
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
        printf("found offset: %lu\n", value_table.offset);
        break;
    case SL_PARSER_STATE_DEFINITION_INT64:
        switch (parser->state) {
        case SL_PARSER_STATE_DEFINITION_INT64:
            value.type = SL_VALUE_TYPE_INT64;
            break;
        default:
            break;
        }
        value.offset = parser->global_offset;
        switch (parser->prev_state) {
        case SL_PARSER_STATE_GLOBAL:
            value.location = SL_VALUE_VARIABLE_LOCATION_GLOBAL;
            parser->global_offset += 8;
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
    }

    return SL_ERR_OK;
}
