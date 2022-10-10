#include "hash.h"

size_t hash_function_string(char *str) {
    size_t len = strlen(str);
    size_t buf = 0;
    if (len < 2) {
        memcpy(&buf, str, 2);
    } else {
        memcpy(&buf, str, 2);
    }
    return buf;
}
