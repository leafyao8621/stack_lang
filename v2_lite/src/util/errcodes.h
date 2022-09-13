#ifndef UTIL_ERRCODES_H_
#define UTIL_ERRCODES_H_

#define ERR_OK 0
#define ERR_NULL_PTR 1
#define ERR_STACK_OVERFLOW 2
#define ERR_STACK_EMPTY 3
#define ERR_HASHSET_FULL 4

extern const char *errcode_lookup[5];

#endif
