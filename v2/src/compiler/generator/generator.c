#include "generator.h"
#include "lookup/lookup.h"

DEF_DARRAY_FUNCTIONS(Type)

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
    int ret = DArrayType_initialize(&generator->stack, 1000);
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
    int ret = DArrayType_finalize(&generator->stack);
    if (ret) {
        return ret;
    }
    ret = parser_finalize(&generator->parser);
    if (ret) {
        return ret;
    }
    return 0;
}

static int handle_declarations_x86_64_linux(Generator *generator, FILE *fasm) {
    String *iter_str_lit = generator->parser.str_lit.data;
    for (
        size_t i = 0;
        i < generator->parser.str_lit.size;
        ++i,
        ++iter_str_lit
    ) {
        fprintf(
            fasm,
            "str_lit_%lu:\n    .asciz \"",
            i
        );
        for (char *j = *iter_str_lit; *j; ++j) {
            switch (*j) {
            case '\t':
                fprintf(fasm, "%s", "\\t");
                break;
            case '\n':
                fprintf(fasm, "%s", "\\n");
                break;
            case '"':
                fprintf(fasm, "%s", "\\\"");
                break;
            default:
                fputc(*j, fasm);
            }
        }
        fprintf(fasm, "%c\n", '"');
    }
    HashSetStringNode *iter_int_name = generator->parser.int_name.data;
    for (
        size_t i = 0;
        i < generator->parser.int_name.capacity;
        ++i,
        ++iter_int_name
    ) {
        if (iter_int_name->in_use) {
            fprintf(
                fasm,
                "int_name_%s:\n    .quad 0\n",
                iter_int_name->item);
        }
    }
    HashSetStringNode *iter_str_name = generator->parser.str_name.data;
    for (
        size_t i = 0;
        i < generator->parser.str_name.capacity;
        ++i,
        ++iter_str_name
    ) {
        if (iter_str_name->in_use) {
            fprintf(
                fasm,
                "str_name_%s:\n    .quad 0\n",
                iter_str_name->item);
        }
    }
    HashMapStringSizeNode *iter_arr_name = generator->parser.arr_name.data;
    for (
        size_t i = 0;
        i < generator->parser.arr_name.capacity;
        ++i,
        ++iter_arr_name
    ) {
        if (iter_arr_name->in_use) {
            fprintf(
                fasm,
                "arr_name_%s:\n    .fill %lu\n",
                iter_arr_name->key,
                iter_arr_name->value * 8);
        }
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
