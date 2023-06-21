#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_for(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token,
    bool *push_control,
    bool *push_control_extra) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    *push_control = true;
    *push_control_extra = false;
    return SL_ERR_OK;
}
