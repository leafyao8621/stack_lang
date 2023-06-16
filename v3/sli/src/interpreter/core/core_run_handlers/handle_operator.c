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
SLErrCode runtime_handle_operator_lnot(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_land(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_lor(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_eq(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_neq(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_lt(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_gt(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_leq(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_geq(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_inc(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_dec(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_add_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_subtract_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_multiply_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_divide_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_lshift_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_rshift_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_rlshift_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_band_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_bor_assign(SLInterpreter *interpreter);

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
        runtime_handle_operator_lnot,
        runtime_handle_operator_land,
        runtime_handle_operator_lor,
        runtime_handle_operator_eq,
        runtime_handle_operator_neq,
        runtime_handle_operator_lt,
        runtime_handle_operator_gt,
        runtime_handle_operator_leq,
        runtime_handle_operator_geq,
        runtime_handle_operator_assign,
        runtime_handle_operator_inc,
        runtime_handle_operator_dec,
        runtime_handle_operator_add_assign,
        runtime_handle_operator_subtract_assign,
        runtime_handle_operator_multiply_assign,
        runtime_handle_operator_divide_assign,
        runtime_handle_operator_lshift_assign,
        runtime_handle_operator_rshift_assign,
        runtime_handle_operator_rlshift_assign,
        runtime_handle_operator_band_assign,
        runtime_handle_operator_bor_assign,
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
