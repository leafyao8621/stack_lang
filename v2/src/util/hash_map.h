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

#define DEF_HASHMAP_FUNCTIONS(Key, Value)\
int HashMap##Key##Value##_initialize(\
    HashMap##Key##Value *hashset,\
    size_t capacity,\
    size_t (*hash)(Key*),\
    bool (*eq)(Key*, Key*)\
);\
int HashMap##Key##Value##_fetch(\
    HashMap##Key##Value *hashmap,\
    Key *key,\
    Value **value\
);\
int HashMap##Key##Value##_check(\
    HashMap##Key##Value *hashmap,\
    Key *key,\
    bool *out\
);\
int HashMap##Key##Value##_finalize(HashMap##Key##Value *hashmap);\
int HashMap##Key##Value##_initialize(\
    HashMap##Key##Value *hashmap,\
    size_t capacity,\
    size_t (*hash)(Key*),\
    bool (*eq)(Key*, Key*)\
) {\
    if (!hashmap || !hash) {\
        return ERR_NULL_PTR;\
    }\
    hashmap->capacity = capacity;\
    hashmap->hash = hash;\
    hashmap->eq = eq;\
    hashmap->data = calloc(capacity, sizeof(HashMap##Key##Value##Node));\
    if (!hashmap->data) {\
        return ERR_OUT_OF_MEMORY;\
    }\
    return 0;\
}\
int HashMap##Key##Value##_fetch(\
    HashMap##Key##Value *hashmap,\
    Key *key,\
    Value **value\
) {\
    if (!hashmap || !key) {\
        return ERR_NULL_PTR;\
    }\
    size_t idx = hashmap->hash(key) % hashmap->capacity;\
    HashMap##Key##Value##Node *iter = hashmap->data + idx;\
    for (\
        ;\
        idx < hashmap->capacity &&\
        iter->in_use &&\
        !hashmap->eq(&iter->key, key);\
        ++idx,\
        ++iter\
    );\
    if (idx < hashmap->capacity && iter->in_use) {\
        *value = &iter->value;\
        return 0;\
    }\
    if (idx == hashmap->capacity) {\
        HashMap##Key##Value##Node *buf = 0;\
        size_t new_capacity = hashmap->capacity << 1;\
        bool cont = false;\
        do {\
            buf = calloc(new_capacity, sizeof(HashMap##Key##Value##Node));\
            if (!buf) {\
                return ERR_OUT_OF_MEMORY;\
            }\
            cont = false;\
            HashMap##Key##Value##Node *iter_old = hashmap->data;\
            for (size_t i = 0; i < hashmap->capacity; ++i, ++iter_old) {\
                if (!iter_old->in_use) {\
                    continue;\
                }\
                size_t idx_new = hashmap->hash(&iter_old->key) % new_capacity;\
                HashMap##Key##Value##Node *iter_new = buf + idx_new;\
                for (\
                    ;\
                    idx < new_capacity &&\
                    iter_new->in_use;\
                    ++idx_new,\
                    ++iter_new\
                );\
                if (idx == new_capacity) {\
                    cont = true;\
                    break;\
                }\
                iter_new->key = iter_old->key;\
                iter_new->value = iter_old->value;\
                iter_new->in_use = true;\
            }\
            idx = hashmap->hash(key) % hashmap->capacity;\
            iter = buf + idx;\
            for (\
                ;\
                idx < new_capacity &&\
                iter->in_use;\
                ++idx,\
                ++iter\
            );\
            if (idx == new_capacity) {\
                cont = true;\
            }\
            if (cont) {\
                new_capacity <<= 1;\
                free(buf);\
            }\
        } while (cont);\
        free(hashmap->data);\
        hashmap->data = buf;\
        hashmap->capacity = new_capacity;\
    }\
    iter->key = *key;\
    iter->in_use = true;\
    *value = &iter->value;\
    return 0;\
}\
int HashMap##Key##Value##_check(\
    HashMap##Key##Value *hashmap,\
    Key *key,\
    bool *out\
) {\
    if (!hashmap || !key || !out) {\
        return ERR_NULL_PTR;\
    }\
    size_t idx = hashmap->hash(key) % hashmap->capacity;\
    HashMap##Key##Value##Node *iter = hashmap->data + idx;\
    for (\
        ;\
        idx < hashmap->capacity &&\
        iter->in_use &&\
        !hashmap->eq(&iter->key, key);\
        ++idx,\
        ++iter\
    );\
    if (idx == hashmap->capacity || !iter->in_use) {\
        *out = false;\
    } else {\
        *out = true;\
    }\
    return 0;\
}\
int HashMap##Key##Value##_finalize(HashMap##Key##Value *hashmap) {\
    if (!hashmap) {\
        return ERR_NULL_PTR;\
    }\
    free(hashmap->data);\
    return 0;\
}

#endif
