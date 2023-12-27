#include <stdio.h>

#include <sliv4/util/load_file.h>

SLErr load_file(char *fn, String *out) {
    if (!fn) {
        return SL_ERR_NULL_PTR;
    }
    FILE *fin = fopen(fn, "rb");
    if (!fin) {
        return SL_ERR_FILE_IO;
    }
    char buf[1000];
    size_t sz = 0;
    int ret = 0;
    for (; (sz = fread(buf, 1, 1000, fin)) == 1000;) {
        ret =
            DArrayChar_push_back_batch(
                out,
                buf,
                sz
            );
        if (ret) {
            fclose(fin);
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    if (sz) {
        ret =
            DArrayChar_push_back_batch(
                out,
                buf,
                sz
            );
        if (ret) {
            fclose(fin);
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    fclose(fin);
    return SL_ERR_OK;
}
