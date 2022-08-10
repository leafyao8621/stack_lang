#ifndef UTIL_ERRCODES_H_
#define UTIL_ERRCODES_H_

#define ERR_NULL_PTR 1
#define ERR_OUT_OF_MEMORY 2
#define ERR_FILE_IO 3
#define ERR_INVALID_VAR_NAME 4
#define ERR_INVALID_STR_LIT 5
#define ERR_INVALID_OPERATOR 6
#define ERR_ARR_NOT_INITIALIZED 7
#define ERR_INVALID_INT_LIT 8
#define ERR_INVALID_ARR_SIZE 9
#define ERR_INVALID_COMMAND 10
#define ERR_EMPTY_DARRAY 11

extern const char *errcode_lookup[12];

#endif
