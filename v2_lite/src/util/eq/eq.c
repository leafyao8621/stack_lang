#include <string.h>

#include "eq.h"

unsigned char eq_function_string(char **a, char **b) {
    return !strcmp(*a, *b);
}
