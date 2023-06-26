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
        array_meta->dynamic = true;
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
    return SL_ERR_OK;
}
