#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_keyword_int64(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT64;
    return SL_ERR_OK;
}
