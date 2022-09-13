#ifndef UTIL_HASH_SET_H_
#define UTIL_HASH_SET_H_

#include "errcodes.h"

#define DEF_HASHSET(Type, Capacity)\
typedef struct HashSet##Type##Capacity##Token {\
    unsigned char in_use;\
    Type item;\
} HashSet##Type##Capacity##Token;\
typedef struct HashSet##Type##Capacity {\
    unsigned short size;\
    HashSet##Type##Capacity##Token data[Capacity];\
    unsigned short (*hash)(Type*);\
    unsigned char (*eq)(Type*, Type*);\
} Hashset##Type##Capacity;\
int HashSet##Type##Capacity##_initialze(\
    HashSet##Type##Capacity *hashset,\
    unsigned short (*hash)(Type*),\
    unsigned char (*eq)(Type*, Type*));\
int HashSet##Type##Capacity##_insert(\
    HashSet##Type##Capacity *hashset,\
    Type *item);\
int HashSet##Type##Capacity##_check(\
    HashSet##Type##Capacity *hashset,\
    Type *item);

#define DEF_HASHSET_FUNCTIONS(Type, Capacity)\
int HashSet##Type##Capacity##_initialze(\
    HashSet##Type##Capacity *hashset,\
    size_t (*hash)(Type*),\
    unsigned char (*eq)(Type*, Type*)) {\
    if (!hashset || !hash || !eq) {\
        return ERR_NULL_PTR;\
    }\
    hashset->size = 0;\
    hashset->hash = hash;\
    hashset->eq = eq;\
    return ERR_OK;\
}\

#endif
