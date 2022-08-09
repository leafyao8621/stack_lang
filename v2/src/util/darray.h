#ifndef UTIL_DARRAY_H_
#define UTIL_DARRAY_H_

#include <stdlib.h>

#include "errcodes.h"

#define DEF_DARRAY(Type)\
typedef struct DArray##Type {\
    size_t size, capacity;\
    Type *data;\
} DArray##Type;

#define DEF_DARRAY_FUNCTIONS(Type)\
int DArray##Type##_initialize(DArray##Type *darray, size_t capacity);\
int DArray##Type##_push(DArray##Type *darray, Type *item);\
int DArray##Type##_finalize(DArray##Type *darray);\
int DArray##Type##_initialize(DArray##Type *darray, size_t capacity) {\
    if (!darray) {\
        return ERR_NULL_PTR;\
    }\
    darray->size = 0;\
    darray->capacity = capacity;\
    darray->data = malloc(sizeof(Type) * capacity);\
    if (!darray->data) {\
        return ERR_OUT_OF_MEMORY;\
    }\
    return 0;\
}\
int DArray##Type##_push(DArray##Type *darray, Type *item) {\
    if (!darray || !item) {\
        return ERR_NULL_PTR;\
    }\
    if (darray->size == darray->capacity) {\
        darray->capacity <<= 1;\
        darray->data = realloc(darray->data, sizeof(Type) * darray->capacity);\
    }\
    darray->data[darray->size++] = *item;\
    return 0;\
}\
int DArray##Type##_finalize(DArray##Type *darray) {\
    if (!darray) {\
        return ERR_NULL_PTR;\
    }\
    free(darray->data);\
    return 0;\
}

#endif
