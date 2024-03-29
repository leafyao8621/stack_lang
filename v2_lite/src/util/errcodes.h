#ifndef UTIL_ERRCODES_H_
#define UTIL_ERRCODES_H_

#define ERR_OK 0
#define ERR_NULL_PTR 1
#define ERR_STACK_OVERFLOW 2
#define ERR_STACK_EMPTY 3
#define ERR_HASHSET_FULL 4
#define ERR_HASHMAP_FULL 5
#define ERR_FILE_IO 6
#define ERR_INVALID_TOKEN 7

extern const char *errcode_lookup[8];

#endif
