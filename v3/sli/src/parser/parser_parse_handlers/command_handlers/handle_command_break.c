#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_break(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    *push_control = false;
    *push_control_extra = true;
    return SL_ERR_OK;
}
