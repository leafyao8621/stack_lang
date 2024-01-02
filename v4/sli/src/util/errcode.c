#include <sliv4/util/errcode.h>

const char *slerr_lookup[9] =
    {
        "SL_ERR_OK",
        "SL_ERR_NULL_PTR",
        "SL_ERR_FILE_IO",
        "SL_ERR_OUT_OF_MEMORY",
        "SL_ERR_INVALID_TOKEN",
        "SL_ERR_INVALID_KEYWORD",
        "SL_ERR_INVALID_IDENTIFIER",
        "SL_ERR_IDENTIFIER_NOT_FOUND",
        "SL_ERR_DUPLICATE_IDENTIFIER"
    };
