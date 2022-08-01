#ifndef UTIL_HASH_Map_H_
#define UTIL_HASH_Map_H_

#include <stdint.h>

#define DEF_HASHMAP(Key, Value)\
typedef struct HashMap##Key##Value##Node {\
    Key key;\
    Value value;\
    uint64_t in_use;\
} HashMap##Key##Value##Node;\
typedef struct HashMap##Key##Value {\
    size_t size, capacity;\
    HashMap##Key##Value##Node *data;\
    size_t (*hash)(Key*);\
    bool (*eq)(Key*, Key*);\
} HashMap##Key##Value;

#endif
