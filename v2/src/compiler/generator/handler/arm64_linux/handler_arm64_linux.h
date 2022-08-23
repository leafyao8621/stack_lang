#ifndef HANDLER_ARM64_LINUX_H_
#define HANDLER_ARM64_LINUX_H_

#include "../../generator.h"

int handle_declarations_arm64_linux(Generator *generator, FILE *fasm);
int handle_function_definitions_arm64_linux(Generator *generator, FILE *fasm);
int handle_tokens_arm64_linux(Generator *generator, FILE *fasm);

#endif
