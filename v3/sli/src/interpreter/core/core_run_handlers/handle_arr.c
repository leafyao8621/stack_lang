#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_arr(SLInterpreter *interpreter) {
    int ret =
        DArraySLToken_push_back(
            &interpreter->operation_stack,
            interpreter->current
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    if (!interpreter->bound_check && interpreter->current->data.arr.dim > 1) {
        return SL_ERR_MULTI_DIM_ARR;
    }
    return SL_ERR_OK;
}
