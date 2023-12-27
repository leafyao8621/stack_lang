#ifndef UTIL_ERRCODE_H_
#define UTIL_ERRCODE_H_

typedef enum SLErr {
    SL_ERR_OK,
    SL_ERR_NULL_PTR,
    SL_ERR_FILE_IO,
    SL_ERR_OUT_OF_MEMORY
} SLErr;

extern const char *slerr_lookup[4];

#endif
