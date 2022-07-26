#include "hash.h"

size_t hash_function_string(char **str) {
    size_t len = strlen(*str);
    size_t buf = 0;
    if (len < sizeof(size_t)) {
        memcpy(&buf, *str, len);
    } else {
        memcpy(&buf, *str, sizeof(size_t));
    }
    return buf;
}
