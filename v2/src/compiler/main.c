#include "generator/generator.h"

int main(void) {
    Generator generator;
    generator_initialize(&generator);
    generator_finalize(&generator);
    return 0;
}
