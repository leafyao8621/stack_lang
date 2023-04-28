#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

typedef enum SLErrCode {
    SL_ERR_OK,
    SL_ERR_NULL_PTR,
    SL_ERR_OUT_OF_MEMORY,
    SL_ERR_INVALID_INT_LITERAL,
    SL_ERR_INVALID_FLOAT_LITERAL,
    SL_ERR_INVALID_VARIABLE_NAME
} SLErrCode;

extern const char *sl_errcode_lookup[6];

#endif