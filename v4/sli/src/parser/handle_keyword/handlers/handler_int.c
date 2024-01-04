#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_keyword_int8(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT8;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_uint8(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_UINT8;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_int16(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT16;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_uint16(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_UINT16;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_int32(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT32;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_uint32(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_UINT32;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_int64(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT64;
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword_uint64(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_UINT64;
    return SL_ERR_OK;
}


