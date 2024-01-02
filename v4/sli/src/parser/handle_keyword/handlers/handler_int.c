#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_keyword_int64(
    SLParser *parser, SLModule *module, char *token) {
    if (!parser || !module || !token) {
        return SL_ERR_NULL_PTR;
    }
    puts("Handling Keyword int64");
    parser->prev_state = parser->state;
    parser->state = SL_PARSER_STATE_DEFINITION_INT64;
    return SL_ERR_OK;
}
