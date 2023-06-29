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
SLErrCode runtime_handle_operator_bxor_assign(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator_index(SLInterpreter *interpreter);

SLErrCode runtime_handle_operator(SLInterpreter *interpreter) {
    SLErrCode err = SL_ERR_OK;
    switch (interpreter->current->data.operator_type) {
    case SL_OPERATOR_TYPE_ADD:
        err = runtime_handle_operator_add(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_SUBTRACT:
        err = runtime_handle_operator_subtract(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_MULTIPLY:
        err = runtime_handle_operator_multiply(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_DIVIDE:
        err = runtime_handle_operator_divide(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_MODULO:
        err = runtime_handle_operator_modulo(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LSHIFT:
        err = runtime_handle_operator_lshift(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_RSHIFT:
        err = runtime_handle_operator_rshift(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_RLSHIFT:
        err = runtime_handle_operator_rlshift(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BNOT:
        err = runtime_handle_operator_bnot(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BAND:
        err = runtime_handle_operator_band(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BOR:
        err = runtime_handle_operator_bxor(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BXOR:
        err = runtime_handle_operator_bxor(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LNOT:
        err = runtime_handle_operator_lnot(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LAND:
        err = runtime_handle_operator_land(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LOR:
        err = runtime_handle_operator_lor(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_EQ:
        err = runtime_handle_operator_eq(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_NEQ:
        err = runtime_handle_operator_neq(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LT:
        err = runtime_handle_operator_lt(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_GT:
        err = runtime_handle_operator_gt(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LEQ:
        err = runtime_handle_operator_leq(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_GEQ:
        err = runtime_handle_operator_geq(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_ASSIGN:
        err = runtime_handle_operator_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_INC:
        err = runtime_handle_operator_inc(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_DEC:
        err = runtime_handle_operator_dec(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_ADD_ASSIGN:
        err = runtime_handle_operator_add_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_SUBTRACT_ASSIGN:
        err = runtime_handle_operator_subtract_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_MULTIPLY_ASSIGN:
        err = runtime_handle_operator_multiply_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_DIVIDE_ASSIGN:
        err = runtime_handle_operator_divide_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_LSHIFT_ASSIGN:
        err = runtime_handle_operator_lshift_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_RSHIFT_ASSIGN:
        err = runtime_handle_operator_rshift_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_RLSHIFT_ASSIGN:
        err = runtime_handle_operator_rlshift_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BAND_ASSIGN:
        err = runtime_handle_operator_band_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BOR_ASSIGN:
        err = runtime_handle_operator_bor_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_BXOR_ASSIGN:
        err = runtime_handle_operator_bxor_assign(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_OPERATOR_TYPE_INDEX:
        err = runtime_handle_operator_index(interpreter);
        if (err) {
            return err;
        }
        break;
    default:
        break;
    }
    if (err) {
        return err;
    }
    return SL_ERR_OK;
}
