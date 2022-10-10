#include <stdio.h>

#include "../parser/parser.h"

int main(void) {
    int ret;
    Parser parser;
    ret = parser_initialize(&parser, "../programs/v2_lite/test.sl");
    printf("retcode: %d msg: %s\n", ret, errcode_lookup[ret]);
    ret = parser_parse(&parser);
    printf("retcode: %d msg: %s\n", ret, errcode_lookup[ret]);
    ret = parser_log(&parser, stdout);
    printf("retcode: %d msg: %s\n", ret, errcode_lookup[ret]);
    parser_finalize(&parser);
    return 0;
}
