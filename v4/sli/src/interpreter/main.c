#include <stdio.h>

#include <sliv4/parser/parser.h>
#include <sliv4/util/load_file.h>

int main(int argc, char **argv) {
    String buf;
    int ret = DArrayChar_initialize(&buf, 1000);
    if (ret) {
        puts("Out of memory");
        return 1;
    }
    if (argc < 2) {
        puts("Invalid");
        return 1;
    }
    SLErr retcode = load_file(argv[1], &buf);
    if (retcode) {
        puts(slerr_lookup[retcode]);
        return 1;
    }
    char chr = 0;
    DArrayChar_push_back(&buf, &chr);
    puts(buf.data);
    DArrayChar_finalize(&buf);
    return 0;
}
