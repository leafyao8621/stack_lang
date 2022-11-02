#ifndef UTIL_HASH_MAP_H_
#define UTIL_HASH_MAP_H_

#include <string.h>

#include "errcodes.h"

#define DEF_HASHMAP(Key, Value, Capacity)\
typedef struct HashMap##Key##Value##Capacity##Token {\
    unsigned char in_use;\
    Key key;\
    Value value;\
} HashMap##Key##Value##Capacity##Token;\
typedef struct HashMap##Key##Value##Capacity {\
    HashMap##Key##Value##Capacity##Token data[Capacity];\
    size_t (*hash)(Key);\
    unsigned char (*eq)(Key, Key);\
} HashMap##Key##Value##Capacity;\
int HashMap##Key##Value##Capacity##_initialze(\
    HashMap##Key##Value##Capacity *hashset,\
    size_t (*hash)(Key),\
    unsigned char (*eq)(Key, Key));\
int HashMap##Key##Value##Capacity##_fetch(\
    HashMap##Key##Value##Capacity *hashset,\
    Key key,\
    Value **value);\
int HashMap##Key##Value##Capacity##_check(\
    HashMap##Key##Value##Capacity *hashset,\
    Key key,\
    unsigned char *out);\
int HashMap##Key##Value##Capacity##_find(\
    HashMap##Key##Value##Capacity *hashset,\
    Key key,\
    unsigned char *found,\
    size_t *out);

#define DEF_HASHMAP_FUNCTIONS(Key, Value, Capacity)\
int HashMap##Key##Value##Capacity##_initialize(\
    HashMap##Key##Value##Capacity *hashmap,\
    size_t (*hash)(Key),\
    unsigned char (*eq)(Key, Key)) {\
    if (!hashmap || !hash || !eq) {\
        return ERR_NULL_PTR;\
    }\
    memset(\
        hashmap->data,\
        0,\
        sizeof(HashMap##Key##Value##Capacity##Token) * Capacity\
    );\
    hashmap->hash = hash;\
    hashmap->eq = eq;\
    return ERR_OK;\
}\
int HashMap##Key##Value##Capacity##_fetch(\
    HashMap##Key##Value##Capacity *hashmap,\
    Key key,\
    Value **value) {\
    size_t idx, i;\
    HashMap##Key##Value##Capacity##Token *iter;\
    if (!hashmap || !key || !value) {\
        return ERR_NULL_PTR;\
    }\
    idx = hashmap->hash(key) % Capacity;\
    iter = hashmap->data + idx;\
    for (\
        i = 0;\
        i < Capacity &&\
        iter->in_use &&\
        !hashmap->eq(iter->key, key);\
        ++i,\
        idx = (idx + 1) % Capacity,\
        iter = hashmap->data + idx);\
    if (i == Capacity) {\
        return ERR_HASHMAP_FULL;\
    }\
    iter->in_use = 1;\
    iter->key = key;\
    *value = &iter->value;\
    return ERR_OK;\
}\
int HashMap##Key##Value##Capacity##_check(\
    HashMap##Key##Value##Capacity *hashmap,\
    Key key,\
    unsigned char *out) {\
    size_t idx, i;\
    HashMap##Key##Value##Capacity##Token *iter;\
    if (!hashmap || !key) {\
        return ERR_NULL_PTR;\
    }\
    idx = hashmap->hash(key) % Capacity;\
    iter = hashmap->data + idx;\
    for (\
        i = 0;\
        i < Capacity &&\
        !iter->in_use ||\
        (\
            iter->in_use &&\
            !hashmap->eq(iter->key, key)\
        );\
        ++i,\
        idx = (idx + 1) % Capacity,\
        iter = hashmap->data + idx);\
    *out = i != Capacity;\
    return ERR_OK;\
}\
int HashMap##Key##Value##Capacity##_find(\
    HashMap##Key##Value##Capacity *hashmap,\
    Key key,\
    unsigned char *found,\
    size_t *out) {\
    size_t idx, i;\
    HashMap##Key##Value##Capacity##Token *iter;\
    if (!hashmap || !key) {\
        return ERR_NULL_PTR;\
    }\
    idx = hashmap->hash(key) % Capacity;\
    iter = hashmap->data + idx;\
    for (\
        i = 0;\
        i < Capacity &&\
        !iter->in_use ||\
        (\
            iter->in_use &&\
            !hashmap->eq(iter->key, key)\
        );\
        ++i,\
        idx = (idx + 1) % Capacity,\
        iter = hashmap->data + idx);\
    *found = i != Capacity;\
    *out = idx;\
    return ERR_OK;\
}

#endif
