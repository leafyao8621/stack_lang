#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_str_literal(SLInterpreter *interpreter) {
    SLToken token;
    token.type = SL_TOKEN_TYPE_STR_LITERAL;
    token.data.str_literal = interpreter->current->data.str_literal;
    int ret = DArraySLToken_push_back(&interpreter->operation_stack, &token);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
