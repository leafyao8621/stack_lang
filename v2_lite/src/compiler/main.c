#include <stdio.h>

#include "../util/util.h"

typedef char *String;
DEF_HASHSET(String, 10)
DEF_HASHSET_FUNCTIONS(String, 10)

int main(void) {
    int ret = ERR_OK;
    size_t i = 0;
    HashSetString10 hashset;
    String buf[3] = {
        "abc",
        "def",
        "ghi"
    };
    HashSetString10_initialize(
        &hashset,
        hash_function_string,
        eq_function_string
    );
    for (i = 0; i < 3; ++i) {
        ret = HashSetString10_insert(&hashset, buf + i);
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    for (i = 0; i < 10; ++i) {
        if (hashset.data[i].in_use) {
            puts(hashset.data[i].item);
        }
    }
    return 0;
}
