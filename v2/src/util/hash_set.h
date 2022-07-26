#ifndef UTIL_HASH_SET_H_
#define UTIL_HASH_SET_H_

#include <stdint.h>
#include <stdbool.h>

#define DEF_HASHSET(Type)\
typedef struct HashSet##Type {\
    size_t size, capacity;\
    Type *data;\
    uint64_t in_use;\
    size_t (*hash)(Type*);\
} HashSet##Type;

#define DEF_HASHSET_FUNCTIONS(Type)\
int HashSet##Type##_initialize(\
    HashSet##Type *hashset,\
    size_t capacity,\
    size_t (*hash)(Type*)\
);\
int HashSet##Type##_insert(HashSet##Type *hashset, Type *item);\
int HashSet##Type##_check(HashSet##Type *hashset, Type *item, bool *out);\
int HashSet##Type##_finalize(HashSet##Type *hashset);\
int HashSet##Type##_initialize(\
    HashSet##Type *hashset,\
    size_t capacity,\
    size_t (*hash)(Type*)\
) {\
    if (!hashset || !hash) {\
        return ERR_NULL_PTR;\
    }\
    hashset->size = 0;\
    hashset->capacity = capacity;\
    hashset->hash = hash;\
    hashset->data = malloc(sizeof(Type) * capacity);\
    if (!hashset->data) {\
        return ERR_OUT_OF_MEMORY;\
    }\
    return 0;\
}\
int HashSet##Type##_insert(HashSet##Type *hashset, Type *item) {\
    if (!hashset || !item) {\
        return ERR_NULL_PTR;\
    }\
    return 0;\
}\
int HashSet##Type##_check(HashSet##Type *hashset, Type *item, bool *out) {\
    if (!hashset || !item || !out) {\
        return ERR_NULL_PTR;\
    }\
    return 0;\
}\
int HashSet##Type##_finalize(HashSet##Type *hashset) {\
    if (!hashset) {\
        return ERR_NULL_PTR;\
    }\
    free(hashset->data);\
    return 0;\
}

#endif
