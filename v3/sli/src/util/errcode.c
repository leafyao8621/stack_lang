#include "errcode.h"

const char *sl_errcode_lookup[27] = {
    "SL_ERR_OK",
    "SL_ERR_NULL_PTR",
    "SL_ERR_OUT_OF_MEMORY",
    "SL_ERR_INVALID_INT_LITERAL",
    "SL_ERR_INVALID_FLOAT_LITERAL",
    "SL_ERR_INVALID_VARIABLE_NAME",
    "SL_ERR_INVALID_CHAR_LITERAL",
    "SL_ERR_INVALID_STR_LITERAL",
    "SL_ERR_INVALID_OPERATOR",
    "SL_ERR_MISSING_OPERAND",
    "SL_ERR_TYPE_MISMATCH",
    "SL_ERR_INVALID_COMMAND",
    "SL_ERR_INTERPRETER_NOT_INITIALIZED",
    "SL_ERR_IDX_OUT_OF_BOUND",
    "SL_ERR_STR_NOT_INITIALIZED",
    "SL_ERR_ARR_NOT_INITIALIZED",
    "SL_ERR_DOUBLE_ALLOC",
    "SL_ERR_MULTI_DIM_ARR",
    "SL_ERR_FUNCTION_DOUBLE_DEF",
    "SL_ERR_FUNCTION_DUPLICATE_PAR",
    "SL_ERR_FUNCTION_INVALID_PAR",
    "SL_ERR_FUNCTION_DEF_NO_NAME",
    "SL_ERR_FUNCTION_DEF_DIRTY_STACK",
    "SL_ERR_FUNCTION_RET_MISMATCH",
    "SL_ERR_UNBALANCED",
    "SL_ERR_FUNCTION_CALL_NOT_DEFINED",
    "SL_ERR_FUNCTION_CALL_ARG_MISMATCH"
};
