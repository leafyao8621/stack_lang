#include "core.h"

DEF_DARRAY_FUNCTIONS(SLTokenPtr)

DEF_HASHMAP_FUNCTIONS(BufferPtr, ArrayMeta)

bool eq_buffer_ptr(BufferPtr *a, BufferPtr *b);
size_t hash_buffer_ptr(BufferPtr *a);

SLErrCode SLInterpreter_initialize(SLInterpreter *interpreter) {
    if (!interpreter) {
        return SL_ERR_NULL_PTR;
    }
    interpreter->initialized = false;
    int ret = DArraySLToken_initialize(&interpreter->operation_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArraySLTokenPtr_initialize(&interpreter->control_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        HashMapBufferPtrArrayMeta_initialize(
            &interpreter->global_array,
            10,
            eq_buffer_ptr,
            hash_buffer_ptr
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayIdx_initialize(&interpreter->size_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArrayChar_initialize(&interpreter->call_stack, 80);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SLParser_initialize(&interpreter->parser);
}

SLErrCode SLInterpreter_parse(SLInterpreter *interpreter, char *str) {
    if (!interpreter || !str) {
        return SL_ERR_NULL_PTR;
    }
    SLErrCode err = SLParser_parse(&interpreter->parser, str);
    if (err) {
        return err;
    }
    int ret =
        DArrayChar_initialize(
            &interpreter->global,
            interpreter->parser.global_size
        );
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    interpreter->initialized = true;
    return SL_ERR_OK;
}

void SLInterpreter_finalize(SLInterpreter *interpreter) {
    DArraySLToken_finalize(&interpreter->operation_stack);
    DArraySLTokenPtr_finalize(&interpreter->control_stack);
    HashMapBufferPtrArrayMeta_finalize(&interpreter->global_array);
    DArrayIdx_finalize(&interpreter->size_stack);
    DArrayChar_finalize(&interpreter->call_stack);
    SLParser_finalize(&interpreter->parser);
    if (interpreter->initialized) {
        DArrayChar_finalize(&interpreter->global);
    }
}

SLErrCode runtime_handle_int_literal(SLInterpreter *interpreter);
SLErrCode runtime_handle_int_var(SLInterpreter *interpreter);
SLErrCode runtime_handle_float_literal(SLInterpreter *interpreter);
SLErrCode runtime_handle_float_var(SLInterpreter *interpreter);
SLErrCode runtime_handle_char_literal(SLInterpreter *interpreter);
SLErrCode runtime_handle_char_var(SLInterpreter *interpreter);
SLErrCode runtime_handle_str_literal(SLInterpreter *interpreter);
SLErrCode runtime_handle_str_var(SLInterpreter *interpreter);
SLErrCode runtime_handle_arr(SLInterpreter *interpreter);
SLErrCode runtime_handle_operator(SLInterpreter *interpreter);
SLErrCode runtime_handle_command(SLInterpreter *interpreter);

void runtime_cleanup(SLInterpreter *interpreter) {
    DArraySLToken_clear(&interpreter->operation_stack);
    DArraySLTokenPtr_clear(&interpreter->control_stack);
    HashMapBufferPtrArrayMetaNode *iter_global_array =
        interpreter->global_array.data;
    for (
        size_t i = 0;
        i < interpreter->global_array.capacity;
        ++i, ++iter_global_array) {
        if (iter_global_array->in_use) {
            iter_global_array->in_use = false;
            DArrayIdx_finalize(&iter_global_array->value.size);
            if (iter_global_array->value.dynamic) {
                free(*iter_global_array->key);
            }
            iter_global_array->value.dynamic = false;
        }
    }
    DArrayIdx_clear(&interpreter->size_stack);
}

SLErrCode SLInterpreter_run(SLInterpreter *interpreter, bool bound_check) {
    if (!interpreter) {
        return SL_ERR_NULL_PTR;
    }
    if (!interpreter->initialized) {
        return SL_ERR_INTERPRETER_NOT_INITIALIZED;
    }
    interpreter->bound_check = bound_check;
    interpreter->current = interpreter->parser.code.data;
    interpreter->cur_token_buf = &interpreter->parser.code;
    memset(interpreter->global.data, 0, interpreter->parser.global_size);
    SLErrCode err;
    for (
        ;
        interpreter->current->type != SL_TOKEN_TYPE_COMMAND ||
        interpreter->current->data.command.type != SL_COMMAND_TYPE_HALT;
        ++interpreter->current) {
        switch (interpreter->current->type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            err = runtime_handle_int_literal(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            err = runtime_handle_int_var(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_FLOAT_LITERAL:
            err = runtime_handle_float_literal(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_FLOAT_VAR:
            err = runtime_handle_float_var(interpreter);
            if (err) {
                return err;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            err = runtime_handle_char_literal(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            err = runtime_handle_char_var(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_STR_LITERAL:
            err = runtime_handle_str_literal(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_STR_VAR:
            err = runtime_handle_str_var(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_ARR:
            err = runtime_handle_arr(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_OPERATOR:
            err = runtime_handle_operator(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        case SL_TOKEN_TYPE_COMMAND:
            err = runtime_handle_command(interpreter);
            if (err) {
                runtime_cleanup(interpreter);
                return err;
            }
            break;
        default:
            break;
        }
    }
    runtime_cleanup(interpreter);
    return SL_ERR_OK;
}
