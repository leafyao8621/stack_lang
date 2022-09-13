#ifndef UTIL_HASH_SET_H_
#define UTIL_HASH_SET_H_

#include "errcodes.h"

#define DEF_HASH_SET(Type, Capacity)\
typedef struct HashSet##Type##Capacity##Token {\
    unsigned char in_use;\
    Type item;\
} HashSet##Type##Capacity##Token;\
typedef struct HashSet##Type##Capacity {\
    unsigned short size;\
    HashSet##Type##Capacity##Token data[Capacity];\
    size_t (*hash)(Type*);\
    unsigned char (*eq)(Type*, Type*);\
} Hashset##Type##Capacity;\
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

#endif
