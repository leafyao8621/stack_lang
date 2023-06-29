#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_alloc(SLInterpreter *interpreter) {
    bool found = false;
    for (; !found && interpreter->operation_stack.size;) {
        DArraySLToken_pop_back(&interpreter->operation_stack);
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
        case SL_TOKEN_TYPE_INT_VAR:
        case SL_TOKEN_TYPE_CHAR_LITERAL:
        case SL_TOKEN_TYPE_CHAR_VAR:
            break;
        case SL_TOKEN_TYPE_ARR:
            found = true;
            break;
        default:
            return SL_ERR_INVALID_COMMAND;
        }
    }
    BufferPtr arr = NULL;
    Idx offset =
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .data
            .arr
            .var_data
            .idx;
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .data
            .arr
            .var_data
            .location) {
    case SL_VARIABLE_LOCATION_GLOBAL:
        arr = (BufferPtr)(interpreter->global.data + offset);
        break;
    case SL_VARIABLE_LOCATION_DIRECT:
        arr =
            (BufferPtr)
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .var_data
                    .direct;
        break;
    default:
        break;
    }
    HashMapBufferPtrArrayMeta_find(
        &interpreter->global_array,
        &arr,
        &found
    );
    ArrayMeta *array_meta;
    int ret = 0;
    if (found) {
        HashMapBufferPtrArrayMeta_fetch(
            &interpreter->global_array,
            &arr,
            &array_meta
        );
        if (array_meta->dynamic) {
            return SL_ERR_DOUBLE_ALLOC;
        }
    } else {
        ret =
            HashMapBufferPtrArrayMeta_fetch(
                &interpreter->global_array,
                &arr,
                &array_meta
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    SLToken *iter_stack =
        interpreter->operation_stack.data +
        interpreter->operation_stack.size + 1;
    size_t dim =
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .data
            .arr
            .dim;
    size_t nmemb = 1;
    int64_t op_int = 0;
    ret = DArrayIdx_initialize(&array_meta->size, dim);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    for (size_t i = 0; i < dim; ++i, ++iter_stack) {
        switch (iter_stack->type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_int = iter_stack->data.int_literal;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset = iter_stack->data.int_var.idx;
            switch (iter_stack->data.int_var.location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_int = *(int64_t*)iter_stack->data.int_var.direct;
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_int = iter_stack->data.char_literal;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset = iter_stack->data.char_var.idx;
            switch (iter_stack->data.char_var.location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_int = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_int = *(char*)iter_stack->data.char_var.direct;
                break;
            default:
                break;
            }
            break;
        default:
            break;
        }
        nmemb *= op_int;
        ret = DArrayIdx_push_back(&array_meta->size, (Idx*)&op_int);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .data
            .arr
            .type) {
    case SL_TOKEN_TYPE_INT_VAR:
        *arr = calloc(nmemb, 8);
        break;
    case SL_TOKEN_TYPE_FLOAT_VAR:
        *arr = calloc(nmemb, 8);
        break;
    case SL_TOKEN_TYPE_CHAR_VAR:
        *arr = calloc(nmemb, 1);
        break;
    case SL_TOKEN_TYPE_STR_VAR:
        *arr = calloc(nmemb, 8);
        break;
    default:
        break;
    }
    if (!*arr) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    array_meta->dynamic = true;
    return SL_ERR_OK;
}
