#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

#include "operator_handlers/handle_operator_add.h"
#include "operator_handlers/handle_operator_subtract.h"
#include "operator_handlers/handle_operator_multiply.h"
#include "operator_handlers/handle_operator_divide.h"
#include "operator_handlers/handle_operator_modulo.h"
#include "operator_handlers/handle_operator_lshift.h"
#include "operator_handlers/handle_operator_rshift.h"
#include "operator_handlers/handle_operator_rlshift.h"
#include "operator_handlers/handle_operator_bnot.h"
#include "operator_handlers/handle_operator_band.h"
#include "operator_handlers/handle_operator_bor.h"
#include "operator_handlers/handle_operator_bxor.h"
#include "operator_handlers/handle_operator_lnot.h"
#include "operator_handlers/handle_operator_land.h"
#include "operator_handlers/handle_operator_lor.h"
#include "operator_handlers/handle_operator_eq.h"
#include "operator_handlers/handle_operator_neq.h"
#include "operator_handlers/handle_operator_lt.h"
#include "operator_handlers/handle_operator_gt.h"
#include "operator_handlers/handle_operator_leq.h"
#include "operator_handlers/handle_operator_geq.h"
#include "operator_handlers/handle_operator_assign.h"
#include "operator_handlers/handle_operator_inc.h"
#include "operator_handlers/handle_operator_dec.h"
#include "operator_handlers/handle_operator_add_assign.h"
#include "operator_handlers/handle_operator_subtract_assign.h"
#include "operator_handlers/handle_operator_multiply_assign.h"
#include "operator_handlers/handle_operator_divide_assign.h"
#include "operator_handlers/handle_operator_lshift_assign.h"
#include "operator_handlers/handle_operator_rshift_assign.h"
#include "operator_handlers/handle_operator_rlshift_assign.h"
#include "operator_handlers/handle_operator_band_assign.h"
#include "operator_handlers/handle_operator_bor_assign.h"
#include "operator_handlers/handle_operator_bxor_assign.h"
#include "operator_handlers/handle_operator_index.h"

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
