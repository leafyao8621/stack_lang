#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_operator_plus(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_operator_minus(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_operator_equal(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_operator(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case SL_PARSER_STATE_GLOBAL:
        break;
    default:
        return SL_ERR_RUNAWAY_VARIABLE_DEF;
    }
    SLErr reterr = SL_ERR_OK;
    switch (*parser->buf.data) {
    case '+':
        reterr =
            SLParser_parse_module_text_handle_operator_plus(parser, module);
        if (reterr) {
            return reterr;
        }
        break;
    case '-':
        break;
    case '=':
        reterr =
            SLParser_parse_module_text_handle_operator_equal(parser, module);
        if (reterr) {
            return reterr;
        }
        break;
    default:
        return SL_ERR_INVALID_OPERATOR;
    }
    return SL_ERR_OK;
}
