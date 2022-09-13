#ifndef UTIL_HASH_SET_H_
#define UTIL_HASH_SET_H_

#include <string.h>

#include "errcodes.h"

#define DEF_HASHSET(Type, Capacity)\
typedef struct HashSet##Type##Capacity##Token {\
    unsigned char in_use;\
    Type item;\
} HashSet##Type##Capacity##Token;\
typedef struct HashSet##Type##Capacity {\
    HashSet##Type##Capacity##Token data[Capacity];\
    size_t (*hash)(Type*);\
    unsigned char (*eq)(Type*, Type*);\
} HashSet##Type##Capacity;\
int HashSet##Type##Capacity##_initialze(\
    HashSet##Type##Capacity *hashset,\
    size_t (*hash)(Type*),\
    unsigned char (*eq)(Type*, Type*));\
int HashSet##Type##Capacity##_insert(\
    HashSet##Type##Capacity *hashset,\
    Type *item);\
int HashSet##Type##Capacity##_check(\
    HashSet##Type##Capacity *hashset,\
    Type *item);

#define DEF_HASHSET_FUNCTIONS(Type, Capacity)\
int HashSet##Type##Capacity##_initialize(\
    HashSet##Type##Capacity *hashset,\
    size_t (*hash)(Type*),\
    unsigned char (*eq)(Type*, Type*)) {\
    if (!hashset || !hash || !eq) {\
        return ERR_NULL_PTR;\
    }\
    memset(\
        hashset->data,\
        0,\
        sizeof(HashSet##Type##Capacity##Token) * Capacity\
    );\
    hashset->hash = hash;\
    hashset->eq = eq;\
    return ERR_OK;\
}\
int HashSet##Type##Capacity##_insert(\
    HashSet##Type##Capacity *hashset,\
    Type *item) {\
    size_t idx, i;\
    HashSet##Type##Capacity##Token *iter;\
    if (!hashset || !item) {\
        return ERR_NULL_PTR;\
    }\
    idx = hashset->hash(item) % Capacity;\
    iter = hashset->data + idx;\
    for (\
        i = 0;\
        i < Capacity &&\
        iter->in_use &&\
        !hashset->eq(&iter->item, item);\
        ++i,\
        idx = (idx + 1) % Capacity,\
        iter = hashset->data + idx);\
    if (i == Capacity) {\
        return ERR_HASHSET_FULL;\
    }\
    iter->in_use = 1;\
    iter->item = *item;\
    return ERR_OK;\
}

#endif
