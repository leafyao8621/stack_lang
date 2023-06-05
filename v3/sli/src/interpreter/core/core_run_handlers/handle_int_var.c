#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_int_var(SLInterpreter *interpreter) {
    SLToken token;
    token.type = SL_TOKEN_TYPE_INT_VAR;
    token.data.int_var = interpreter->current->data.int_var;
    int ret = DArraySLToken_push_back(&interpreter->operation_stack, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
