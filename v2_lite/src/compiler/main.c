#include <stdio.h>

#include "../util/util.h"

typedef char *String;
typedef int Int;
DEF_HASHMAP(String, Int, 10)
DEF_HASHMAP_FUNCTIONS(String, Int, 10)

int main(void) {
    int ret = ERR_OK;
    size_t i = 0;
    HashMapStringInt10 hashmap;
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
    HashMapStringInt10_initialize(
        &hashmap,
        hash_function_string,
        eq_function_string
    );
    for (i = 0; i < 12; ++i) {
        int *ptr;
        ret = HashMapStringInt10_fetch(&hashmap, buf + i, &ptr);
        if (!ret) {
            *ptr = i;
        }
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    for (i = 0; i < 10; ++i) {
        if (hashmap.data[i].in_use) {
            printf("%s: %d\n", hashmap.data[i].key, hashmap.data[i].value);
        } else {
            puts("EMPTY");
        }
    }
    for (i = 0; i < 12; ++i) {
        ret = HashMapStringInt10_check(&hashmap, buf1 + i, &check);
        printf("%s: %hhu ", buf1[i], check);
        if (check) {
            int *ptr;
            ret = HashMapStringInt10_fetch(&hashmap, buf1 + i, &ptr);
            printf("%d", *ptr);
        }
        putchar(10);
        printf("%d: %s\n", ret, errcode_lookup[ret]);

    }
    return 0;
}
