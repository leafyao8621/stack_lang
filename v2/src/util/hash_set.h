#ifndef HASH_SET_H_
#define HASH_SET_H_

#include <stdint.h>

#define DEF_HASHSET(Type)\
typedef struct HashSet##Type {\
    size_t size, capacity;\
    Type *data;\
    uint64_t in_use;\
    size_t (*hash)(Type*);\
} HashSet##Type;

#endif
