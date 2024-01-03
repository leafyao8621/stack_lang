#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_literal_string(
    char **iter, SLModule *module) {
    if (!iter || !module) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
