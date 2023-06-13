#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_operator_add(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_subtract(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_multiply(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_divide(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_modulo(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_lshift(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_rshift(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_rlshift(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_bnot(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_band(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_bor(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_bxor(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_assign(SLInterpreter *interpreter);

typedef SLErrCode (*Handler)(SLInterpreter*);

SLErrCode runtime_handle_operator(SLInterpreter *interpreter) {
    Handler handlers[35] = {
        runtime_handle_operator_add,
        runtime_handle_operator_subtract,
        runtime_handle_operator_multiply,
        runtime_handle_operator_divide,
        runtime_handle_operator_modulo,
        runtime_handle_operator_lshift,
        runtime_handle_operator_rshift,
        runtime_handle_operator_rlshift,
        runtime_handle_operator_bnot,
        runtime_handle_operator_band,
        runtime_handle_operator_bor,
        runtime_handle_operator_bxor,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        runtime_handle_operator_assign,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    };
    SLErrCode err =
        handlers[interpreter->current->data.operator_type](interpreter);
    if (err) {
        return err;
    }
    return SL_ERR_OK;
}
