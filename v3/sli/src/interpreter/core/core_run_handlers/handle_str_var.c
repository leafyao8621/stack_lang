#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_str_var(SLInterpreter *interpreter) {
    int ret =
        DArraySLToken_push_back(
            &interpreter->operation_stack,
            interpreter->current
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
