#include <string.h>

#include "hash.h"

unsigned short hash_function_string(char **str) {
    size_t len = strlen(*str);
    unsigned short buf = 0;
    if (len < 2) {
        memcpy(&buf, *str, 2);
    } else {
        memcpy(&buf, *str, 2);
    }
    return buf;
}
