#include "core.h"

bool eq_buffer_ptr(BufferPtr *a, BufferPtr *b) {
    return *a == *b;
}

size_t hash_buffer_ptr(BufferPtr *a) {
    return (size_t)*a;
}
