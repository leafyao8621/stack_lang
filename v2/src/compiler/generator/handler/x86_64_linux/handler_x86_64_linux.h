#ifndef HANDLER_X86_64_LINUX_H_
#define HANDLER_X86_64_LINUX_H_

#include "../../generator.h"

int handle_declarations_x86_64_linux(Generator *generator, FILE *fasm);
int handle_function_definitions_x86_64_linux(Generator *generator, FILE *fasm);
int handle_tokens_x86_64_linux(Generator *generator, FILE *fasm);

#endif
