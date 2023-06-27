#include <sliv3/core.h>

#include <containers/dstring.h>

int read_file(char *fn, String *buf) {
    char in_buf[1000];
    FILE *fin = fopen(fn, "rb");
    if (!fin) {
        return 1;
    }
    int ret;
    size_t sz = 0;
    for (; (sz = fread(in_buf, 1, 1000, fin)) == 1000;) {
        ret = DArrayChar_push_back_batch(buf, in_buf, sz);
        if (ret) {
            return 1;
        }
    }
    ret = DArrayChar_push_back_batch(buf, in_buf, sz);
    if (ret) {
        return 1;
    }
    char zero = 0;
    ret = DArrayChar_push_back(buf, &zero);
    if (ret) {
        return 1;
    }
    fclose(fin);
    return 0;
}

int main(int argc, const char **argv) {
    String buf;
    int ret_int = DArrayChar_initialize(&buf, 1000);
    if (ret_int) {
        puts("Out of Memory");
        return 0;
    }
    ret_int = read_file((char*)argv[1], &buf);
    if (ret_int) {
        puts("File IO");
        return 0;
    }
    SLInterpreter interpreter;
    SLInterpreter_initialize(&interpreter);
    SLErrCode ret = SLInterpreter_parse(&interpreter, buf.data);
    printf("RET: %d\nMSG: %s\n", ret, sl_errcode_lookup[ret]);
    SLParser_log(&interpreter.parser, stdout);
    ret = SLInterpreter_run(&interpreter);
    printf("RET: %d\nMSG: %s\n", ret, sl_errcode_lookup[ret]);
    ret = SLInterpreter_run(&interpreter);
    printf("RET: %d\nMSG: %s\n", ret, sl_errcode_lookup[ret]);
    SLInterpreter_finalize(&interpreter);
    DArrayChar_finalize(&buf);
    return 0;
}
