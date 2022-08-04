#include "generator/generator.h"

int main(int argc, char **argv) {
    Generator generator;
    if (argc != 3) {
        puts("Usage: in_file out_file");
        return 1;
    }
    int ret = generator_initialize(&generator, argv[1], argv[2]);
    if (ret) {
        printf("errcode: %d\n", ret);
        return 1;
    }
    ret = generator_generate(&generator);
    if (ret) {
        printf("errcode: %d\ndescription: %s\n", ret, errcode_lookup[ret]);
        generator_finalize(&generator);
        return 1;
    }
    ret = generator_finalize(&generator);
    if (ret) {
        printf("errcode: %d\n", ret);
        return 1;
    }
    return 0;
}
