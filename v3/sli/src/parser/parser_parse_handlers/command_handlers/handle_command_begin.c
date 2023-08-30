#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_begin(
    SLParser *parser,
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token || !parser) {
        return SL_ERR_NULL_PTR;
    }
    buffer->par = false;
    *push_token = false;
    *push_control = false;
    *push_control_extra = false;
    return SL_ERR_OK;
}
