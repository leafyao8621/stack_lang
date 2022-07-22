#include "generator/generator.h"

int main(int argc, char **argv) {
    Generator generator;
    if (argc != 3) {
        puts("Usage: in_file out_file");
        return 1;
    }
    generator_initialize(&generator, argv[1], argv[2]);
    generator_generate(&generator);
    generator_finalize(&generator);
    return 0;
}
