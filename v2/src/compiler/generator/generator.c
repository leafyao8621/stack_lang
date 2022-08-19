#include <time.h>

#include "generator.h"
#include "lookup/lookup.h"
#include "handler/handler.h"

DEF_HASHMAP_FUNCTIONS(String, DArrayToken)

int generator_initialize(
    Generator *generator,
    Architecture architecture,
    String ifn,
    String ofn
) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    generator->architecture = architecture;
    int ret = DArrayToken_initialize(&generator->stack, 100);
    if (ret) {
        return ret;
    }
    ret =
        HashMapStringDArrayToken_initialize(
            &generator->ret_vals,
            10, hash_function_string,
            eq_function_string
        );
    if (ret) {
        return ret;
    }
    ret = parser_initialize(&generator->parser, ifn);
    if (ret) {
        return ret;
    }
    generator->ofn = ofn;
    return 0;
}

int generator_finalize(Generator *generator) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    int ret = DArrayToken_finalize(&generator->stack);
    if (ret) {
        return ret;
    }
    HashMapStringDArrayTokenNode *iter_ret_vals =
        generator->ret_vals.data;
    for (
        size_t i = 0;
        i < generator->ret_vals.capacity;
        ++i, ++iter_ret_vals) {
        if (iter_ret_vals->in_use) {
            ret = DArrayToken_finalize(&iter_ret_vals->value);
            if (ret) {
                return ret;
            }
        }
    }
    ret = HashMapStringDArrayToken_finalize(&generator->ret_vals);
    if (ret) {
        return ret;
    }
    ret = parser_finalize(&generator->parser);
    if (ret) {
        return ret;
    }
    return 0;
}

int generator_generate(Generator *generator) {
    if (!generator) {
        return ERR_NULL_PTR;
    }
    int ret = parser_parse(&generator->parser);
    if (ret) {
        return ret;
    }
    // parser_log(&generator->parser, stdout);
    size_t ofn_len = strlen(generator->ofn);
    size_t buf_len = ofn_len + 100;
    String buf = malloc(buf_len);
    if (!buf) {
        return ERR_OUT_OF_MEMORY;
    }
    time_t timestamp = time(NULL);
    sprintf(buf, "/tmp/temp_%lu.s", timestamp);
    FILE *fasm = fopen(buf, "w");
    if (!fasm) {
        return ERR_FILE_IO;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        fprintf(fasm, "%s\n", data_section_x86_64_linux);
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        ret = handle_declarations_x86_64_linux(generator, fasm);
        if (ret) {
            return ret;
        }
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        fprintf(fasm, "%s\n", text_start_x86_64_linux_0);
        fprintf(fasm, "%s\n", text_start_x86_64_linux_1);
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        ret = handle_function_definitions_x86_64_linux(generator, fasm);
        if (ret) {
            fclose(fasm);
            parser_log(&generator->parser, stdout);
            return ret;
        }
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        fprintf(fasm, "%s\n", program_start_x86_64_linux);
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        ret = handle_tokens_x86_64_linux(generator, fasm);
        if (ret) {
            fclose(fasm);
            parser_log(&generator->parser, stdout);
            return ret;
        }
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        fprintf(fasm, "%s\n", text_end_x86_64_linux);
        break;
    }
    fclose(fasm);
    sprintf(buf, "as /tmp/temp_%lu.s -o /tmp/temp_%lu.o", timestamp, timestamp);
    system(buf);
    sprintf(buf, "ld /tmp/temp_%lu.o -o %s", timestamp,generator->ofn);
    system(buf);
    sprintf(buf, "/tmp/temp_%lu.s", timestamp);
    remove(buf);
    sprintf(buf, "/tmp/temp_%lu.o", timestamp);
    remove(buf);
    free(buf);
    return 0;
}
