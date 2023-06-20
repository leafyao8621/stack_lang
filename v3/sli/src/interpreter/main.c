#include <sliv3/core.h>

#include <containers/dstring.h>

void read_file(char *fn, String *buf) {
    char in_buf[1000];
    FILE *fin = fopen(fn, "rb");
    size_t sz = 0;
    for (; (sz = fread(in_buf, 1, 1000, fin)) == 1000;) {
        DArrayChar_push_back_batch(buf, in_buf, sz);
    }
    DArrayChar_push_back_batch(buf, in_buf, sz);
    char zero = 0;
    DArrayChar_push_back(buf, &zero);
    fclose(fin);
}

int main(void) {
    String buf;
    DArrayChar_initialize(&buf, 1000);
    read_file("../../programs/v3/b.sl", &buf);
    SLInterpreter interpreter;
    SLInterpreter_initialize(&interpreter);
    SLErrCode ret = SLInterpreter_parse(&interpreter, buf.data);
    printf("RET: %d\nMSG: %s\n", ret, sl_errcode_lookup[ret]);
    SLParser_log(&interpreter.parser, stdout);
    // SLInterpreter_run(&interpreter);
    SLInterpreter_finalize(&interpreter);
    DArrayChar_finalize(&buf);
    return 0;
}
