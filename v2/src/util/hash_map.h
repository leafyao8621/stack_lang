#ifndef HASH_Map_H_
#define HASH_Map_H_

#include <stdint.h>

#define DEF_HASHMAP(Key, Value)\
typedef struct HashMap##Key##Value##Node {\
    Key key;\
    Value value;\
    uint64_t in_use;\
    size_t (*hash)(Key*);\
} HashMap##Key##Value##Node;\
typedef struct HashMap##Key##Value {\
    size_t size, capacity;\
    HashMap##Key##Value##Node *data;\
    size_t (*hash)(Key*);\
} HashMap##Key##Value;

#endif
