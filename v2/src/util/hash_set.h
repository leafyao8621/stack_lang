#ifndef UTIL_HASH_SET_H_
#define UTIL_HASH_SET_H_

#include <stdint.h>
#include <stdbool.h>

#define DEF_HASHSET(Type)\
typedef struct HashSet##Type##Node {\
    Type item;\
    bool in_use;\
} HashSet##Type##Node;\
typedef struct HashSet##Type {\
    size_t capacity;\
    HashSet##Type##Node *data;\
    size_t (*hash)(Type*);\
    bool (*eq)(Type*, Type*);\
} HashSet##Type;\
int HashSet##Type##_initialize(\
    HashSet##Type *hashset,\
    size_t capacity,\
    size_t (*hash)(Type*),\
    bool (*eq)(Type*, Type*)\
);\
int HashSet##Type##_insert(HashSet##Type *hashset, Type *item);\
int HashSet##Type##_check(HashSet##Type *hashset, Type *item, bool *out);\
int HashSet##Type##_finalize(HashSet##Type *hashset);\

#define DEF_HASHSET_FUNCTIONS(Type)\
int HashSet##Type##_initialize(\
    HashSet##Type *hashset,\
    size_t capacity,\
    size_t (*hash)(Type*),\
    bool (*eq)(Type*, Type*)\
) {\
    if (!hashset || !hash) {\
        return ERR_NULL_PTR;\
    }\
    hashset->capacity = capacity;\
    hashset->hash = hash;\
    hashset->eq = eq;\
    hashset->data = calloc(capacity, sizeof(HashSet##Type##Node));\
    if (!hashset->data) {\
        return ERR_OUT_OF_MEMORY;\
    }\
    return 0;\
}\
int HashSet##Type##_insert(HashSet##Type *hashset, Type *item) {\
    if (!hashset || !item) {\
        return ERR_NULL_PTR;\
    }\
    size_t idx = hashset->hash(item) % hashset->capacity;\
    HashSet##Type##Node *iter = hashset->data + idx;\
    for (\
        ;\
        idx < hashset->capacity &&\
        iter->in_use &&\
        !hashset->eq(&iter->item, item);\
        ++idx,\
        ++iter\
    );\
    if (idx == hashset->capacity) {\
        HashSet##Type##Node *buf = 0;\
        size_t new_capacity = hashset->capacity << 1;\
        bool cont = false;\
        do {\
            buf = calloc(new_capacity, sizeof(HashSet##Type##Node));\
            if (!buf) {\
                return ERR_OUT_OF_MEMORY;\
            }\
            cont = false;\
            HashSet##Type##Node *iter_old = hashset->data;\
            for (size_t i = 0; i < hashset->capacity; ++i, ++iter_old) {\
                if (!iter_old->in_use) {\
                    continue;\
                }\
                size_t idx_new = hashset->hash(&iter_old->item) % new_capacity;\
                HashSet##Type##Node *iter_new = buf + idx_new;\
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
                iter_new->item = iter_old->item;\
                iter_new->in_use = true;\
            }\
            idx = hashset->hash(item) % new_capacity;\
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
        free(hashset->data);\
        hashset->data = buf;\
        hashset->capacity = new_capacity;\
    }\
    iter->item = *item;\
    iter->in_use = true;\
    return 0;\
}\
int HashSet##Type##_check(HashSet##Type *hashset, Type *item, bool *out) {\
    if (!hashset || !item || !out) {\
        return ERR_NULL_PTR;\
    }\
    size_t idx = hashset->hash(item) % hashset->capacity;\
    HashSet##Type##Node *iter = hashset->data + idx;\
    for (\
        ;\
        idx < hashset->capacity &&\
        iter->in_use &&\
        !hashset->eq(&iter->item, item);\
        ++idx,\
        ++iter\
    );\
    if (idx == hashset->capacity || !iter->in_use) {\
        *out = false;\
    } else {\
        *out = true;\
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
