#include <stdio.h>

#include <sliv4/parser/parser.h>


SLErr SLParser_parse_module_text(
    String *code, SLParser *parser, SLModule *module) {
    if (!code || !parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
