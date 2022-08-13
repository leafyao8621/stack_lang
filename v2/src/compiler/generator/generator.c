#include "generator.h"
#include "lookup/lookup.h"
#include "handler/handler.h"

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
    parser_log(&generator->parser, stdout);
    FILE *fasm = fopen("temp.s", "w");
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
        fprintf(fasm, "%s\n", text_start_x86_64_linux);
        break;
    }
    switch (generator->architecture) {
    case ARCHITECTURE_X86_64_LINUX:
        ret = handle_tokens_x86_64_linux(generator, fasm);
        if (ret) {
            fclose(fasm);
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
    system("as temp.s -o temp.o");
    size_t ofn_len = strlen(generator->ofn);
    size_t buf_len = ofn_len + 20;
    String buf = malloc(buf_len);
    if (!buf) {
        return ERR_OUT_OF_MEMORY;
    }
    sprintf(buf, "ld temp.o -o %s", generator->ofn);
    system(buf);
    free(buf);
    return 0;
}
