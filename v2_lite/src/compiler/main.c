#include <stdio.h>

#include "../util/util.h"

typedef char *String;
DEF_HASHSET(String, 10)
DEF_HASHSET_FUNCTIONS(String, 10)

int main(void) {
    int ret = ERR_OK;
    size_t i = 0;
    HashSetString10 hashset;
    unsigned char check = 0;
    String buf[12] = {
        "def",
        "abc",
        "ghi",
        "abc",
        "jkl",
        "mno",
        "pqr",
        "stu",
        "vwx",
        "wza",
        "123",
        "456"
    };
    String buf1[12] = {
        "defx",
        "abc",
        "ghiq",
        "abc",
        "jklf",
        "mno",
        "pqr",
        "stu",
        "vwx",
        "wzab",
        "123",
        "456"
    };
    HashSetString10_initialize(
        &hashset,
        hash_function_string,
        eq_function_string
    );
    for (i = 0; i < 12; ++i) {
        ret = HashSetString10_insert(&hashset, buf + i);
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    for (i = 0; i < 10; ++i) {
        if (hashset.data[i].in_use) {
            puts(hashset.data[i].item);
        } else {
            puts("EMPTY");
        }
    }
    for (i = 0; i < 12; ++i) {
        ret = HashSetString10_check(&hashset, buf1 + i, &check);
        printf("%s: %hhu\n", buf1[i], check);
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    return 0;
}
