#ifndef UTIL_H_
#define UTIL_H_

#include <stdlib.h>

#include "errcodes.h"

#define DEF_DARRAY(Type)\
typedef struct DArray##Type {\
    unsigned size, capacity;\
    Type *data;\
} DArray##Type;

#define DEF_DRRAY_FUNCTIONS(Type)\
int DArray##Type##_initialize(DArray##Type *darray);\
int DArray##Type##_push(DArray##Type *darray, Type *item);\
int DArray##Type##_finalize(DArray##Type *darray);\
int DArray##Type##_initialize(DArray##Type *darray) {\
    if (!darray) {\
        return ERR_NULL_PTR;\
    }\
    darray->size = 0;\
    darray->capacity = 1000;\
    darray->data = malloc(sizeof(Type) * 1000);\
    if (!darray->data) {\
        return ERR_OUT_OF_MEMORY;\
    }\
    return 0;\
}\
int DArray##Type##_##push(DArray##Type *darray, Type *item) {\
    if (!darray || !item) {\
        return ERR_NULL_PTR;\
    }\
    \
    return 0;\
}\
int DArray##Type##_##finalize(DArray##Type *darray) {\
    if (!darray) {\
        return ERR_NULL_PTR;\
    }\
    free(darray->data);\
    return 0;\
}

#endif
