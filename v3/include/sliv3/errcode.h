#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

typedef enum SLErrCode {
    SL_ERR_OK,
    SL_ERR_NULL_PTR
} SLErrCode;

extern const char *sl_errcode_lookup[2];

#endif
