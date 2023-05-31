#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../parser.h"

SLErrCode handle_command_halt(
    struct SLParserBuffer *buffer,
    char **iter,
    SLToken *token) {
    if (!buffer || !iter || !token) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
