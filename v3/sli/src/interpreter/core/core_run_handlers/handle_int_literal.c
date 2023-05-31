#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_int_literal(SLInterpreter *interpreter) {
    SLToken token;
    token.type = SL_TOKEN_TYPE_INT_LITERAL;
    token.data.int_literal = interpreter->current->data.int_literal;
    int ret = DArraySLToken_push_back(&interpreter->operation_stack, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ++interpreter->current;
    return SL_ERR_OK;
}
