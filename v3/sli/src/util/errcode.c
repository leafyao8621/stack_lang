#include "errcode.h"

const char *sl_errcode_lookup[15] = {
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
    "SL_ERR_STR_NOT_INITIALIZED"
};
