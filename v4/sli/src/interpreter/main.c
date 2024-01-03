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
    puts(slerr_lookup[retcode]);
    if (retcode) {
        return 1;
    }
    char chr = 0;
    DArrayChar_push_back(&buf, &chr);
    puts(buf.data);
    SLParser parser;
    retcode = SLParser_initialize(&parser);
    puts(slerr_lookup[retcode]);
    if (retcode) {
        return 0;
    }
    SLModule module;
    retcode = SLModule_initialize(&module);
    puts(slerr_lookup[retcode]);
    if (retcode) {
        return 0;
    }
    retcode = SLParser_parse_module_text(&buf, &parser, &module);
    puts(slerr_lookup[retcode]);
    if (retcode) {
        DArrayChar_finalize(&buf);
        SLParser_finalize(&parser);
        SLModule_finalize(&module);
        return 0;
    }
    SLModule_log(&module, stdout);
    DArrayChar_finalize(&buf);
    SLParser_finalize(&parser);
    SLModule_finalize(&module);
    return 0;
}
