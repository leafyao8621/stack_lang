#include <stdio.h>

#include "../util/util.h"

typedef char *String;
DEF_STACK(String, 2)
DEF_STACK_FUNCTIONS(String, 2)

int main(void) {
    int ret = ERR_OK;
    unsigned short i = 0;
    StackString2 stack;
    String buf[3] = {
        "abc",
        "def",
        "ghi"
    };
    StackString2_initialize(&stack);
    for (i = 0; i < 3; ++i) {
        ret = StackString2_push(&stack, &buf[i]);
        puts(buf[i]);
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    for (i = 0; i < 3; ++i) {
        ret = StackString2_pop(&stack);
        if (!ret) {
            puts(*stack.tail);
        }
        printf("%d: %s\n", ret, errcode_lookup[ret]);
    }
    return 0;
}
