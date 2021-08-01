#include <stdio.h>
#include "parser/parser.h"
#include "vm/vm.h"

int main(int argc, const char **argv) {
    if (argc != 2) {
        puts("No file");
        return 1;
    }
    if (parser_parse(argv[1])) {
        puts("No file or ill_formated");
        return 2;
    }
    vm_run();
    return 0;
}
