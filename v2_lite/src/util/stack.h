#ifndef UTIL_STACK_H_
#define UTIL_STACK_H_

#include <stdint.h>

#include "errcodes.h"

#define DEF_STACK(Type, Capacity)\
typedef struct Stack##Type##Capacity {\
    uint16_t size;\
    Type data[Capacity], *tail;\
} Stack##Type##Capacity;\
int Stack##Type##Capacity##_initialize(Stack##Type##Capacity *stack);\
int Stack##Type##Capacity##_push(Stack##Type##Capacity *stack, Type *item);\
int Stack##Type##Capacity##_pop(Stack##Type##Capacity *stack);

#define DEF_STACK_FUNCTIONS(Type, Capacity)\
int Stack##Type##Capacity##_initialize(Stack##Type##Capacity *stack) {\
    if (!stack) {\
        return ERR_NULL_PTR;\
    }\
    stack->size = 0;\
    stack->tail = stack->data;\
    return ERR_OK;\
}\
int Stack##Type##Capacity##_push(Stack##Type##Capacity *stack, Type *item) {\
    if (!stack || !item) {\
        return ERR_NULL_PTR;\
    }\
    if (stack->size == Capacity) {\
        return ERR_STACK_OVERFLOW;\
    }\
    ++stack->size;\
    *(stack->tail++) = *item;\
    return ERR_OK;\
}\
int Stack##Type##Capacity##_pop(Stack##Type##Capacity *stack) {\
    if (!stack) {\
        return ERR_NULL_PTR;\
    }\
    if (!stack->size) {\
        return ERR_STACK_EMPTY;\
    }\
    --stack->size;\
    --stack->tail;\
    return ERR_OK;\
}

#endif
