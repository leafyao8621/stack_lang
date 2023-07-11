#ifndef CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_INDEX_H_
#define CORE_CORE_RUN_HANDLERS_OPERATOR_HANDLERS_HANDLE_OPERATOR_INDEX_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_operator_index(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_operator_index(SLInterpreter *interpreter) {
    DArraySLToken_pop_back(&interpreter->operation_stack);
    DArraySLToken_pop_back(&interpreter->operation_stack);
    SLToken res;
    String *op_a_str_lit, **op_a_str_var;
    BufferPtr op_a_arr;
    char *op_a_arr_imm;
    int64_t op_b_int;
    Idx offset, dim_size;
    bool found;
    ArrayMeta *meta;
    Idx *iter_size;
    op_a_str_lit = NULL;
    op_a_str_var = NULL;
    op_a_arr = NULL;
    op_b_int = offset = 0;
    dim_size = 1;
    found = false;
    meta = NULL;
    iter_size = NULL;
    int ret = 0;
    switch (
        interpreter
            ->operation_stack
            .data[interpreter->operation_stack.size]
            .type) {
    case SL_TOKEN_TYPE_STR_LITERAL:
        res.type = SL_TOKEN_TYPE_CHAR_VAR;
        op_a_str_lit =
            interpreter
                ->parser
                .str_literals
                .data +
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .str_literal;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_literal;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_literal;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(char*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .char_var
                            .direct;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (interpreter->bound_check) {
            if (op_b_int < 0 || (size_t)op_b_int >= op_a_str_lit->size - 1) {
                return SL_ERR_IDX_OUT_OF_BOUND;
            }
        }
        res.data.char_var.location = SL_VARIABLE_LOCATION_DIRECT;
        res.data.char_var.direct = op_a_str_lit->data + op_b_int;
        break;
    case SL_TOKEN_TYPE_STR_VAR:
        res.type = SL_TOKEN_TYPE_CHAR_VAR;
        offset =
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .str_var
                .idx;
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .str_var
                .location) {
        case SL_VARIABLE_LOCATION_GLOBAL:
            op_a_str_var = (String**)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (!*op_a_str_var) {
            return SL_ERR_STR_NOT_INITIALIZED;
        }
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_literal;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_literal;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(char*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .char_var
                            .direct;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (interpreter->bound_check) {
            if (op_b_int < 0 || (size_t)op_b_int >= (*op_a_str_var)->size - 1) {
                return SL_ERR_IDX_OUT_OF_BOUND;
            }
        }
        res.data.char_var.location = SL_VARIABLE_LOCATION_DIRECT;
        res.data.char_var.direct = (*op_a_str_var)->data + op_b_int;
        break;
    case SL_TOKEN_TYPE_ARR:
        offset =
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
            op_a_arr = (BufferPtr)(interpreter->global.data + offset);
            break;
        default:
            break;
        }
        if (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .arr
                .dim == 1) {
            res.type =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type;
        } else {
            res.type = SL_TOKEN_TYPE_ARR_IMMEDIATE;
            res.data.arr.type =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type;
        }
        if (interpreter->bound_check) {
            HashMapBufferPtrArrayMeta_find(
                &interpreter->global_array,
                &op_a_arr,
                &found
            );
            if (!found) {
                return SL_ERR_ARR_NOT_INITIALIZED;
            }
            HashMapBufferPtrArrayMeta_fetch(
                &interpreter->global_array,
                &op_a_arr,
                &meta
            );
            if (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .dim > 1) {
                iter_size = meta->size.data + 1;
                ret =
                    DArrayIdx_push_back_batch(
                        &interpreter->size_stack,
                        iter_size,
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size]
                            .data
                            .arr
                            .dim - 1
                    );
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
            }
        }
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_literal;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_literal;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(char*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .char_var
                            .direct;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (interpreter->bound_check) {
            if (op_b_int < 0 || (size_t)op_b_int >= meta->size.data[0]) {
                return SL_ERR_IDX_OUT_OF_BOUND;
            }
        }
        if (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .arr
                .dim == 1) {
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type) {
            case SL_TOKEN_TYPE_INT_VAR:
                res.data.int_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.int_var.direct = ((int64_t*)*op_a_arr) + op_b_int;
                break;
            case SL_TOKEN_TYPE_FLOAT_VAR:
                res.data.float_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.float_var.direct = ((double*)*op_a_arr) + op_b_int;
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                res.data.char_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.char_var.direct = *op_a_arr + op_b_int;
                break;
            case SL_TOKEN_TYPE_STR_VAR:
                res.data.str_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.str_var.direct = ((String**)*op_a_arr) + op_b_int;
                break;
            default:
                break;
            }
        } else {
            res.data.arr.dim =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .dim - 1;
            res.data.arr.var_data.location = SL_VARIABLE_LOCATION_DIRECT;
            iter_size = meta->size.data + 1;
            for (
                size_t i = 0;
                i <
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .arr
                        .dim - 1;
                ++i) {
                dim_size *= *iter_size;
            }
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type) {
            case SL_TOKEN_TYPE_INT_VAR:
                res.data.arr.var_data.direct =
                    *op_a_arr + op_b_int * (dim_size << 3);
                break;
            case SL_TOKEN_TYPE_FLOAT_VAR:
                res.data.arr.var_data.direct =
                    *op_a_arr + op_b_int * (dim_size << 3);
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                res.data.arr.var_data.direct =
                    *op_a_arr + op_b_int * dim_size;
                break;
            case SL_TOKEN_TYPE_STR_VAR:
                res.data.arr.var_data.direct =
                    *op_a_arr + op_b_int * (dim_size << 3);
                break;
            default:
                break;
            }
        }
        break;
    case SL_TOKEN_TYPE_ARR_IMMEDIATE:
        op_a_arr_imm =
            (char*)
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .var_data
                    .direct;
        if (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .arr
                .dim == 1) {
            res.type =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type;
        } else {
            res.type = SL_TOKEN_TYPE_ARR_IMMEDIATE;
            res.data.arr.type =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type;
        }
        if (interpreter->bound_check) {
            for (
                size_t i = 0;
                i <
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .arr
                        .dim;
                ++i) {
                DArrayIdx_pop_back(&interpreter->size_stack);
            }
            if (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .dim > 1) {
                iter_size =
                    interpreter->size_stack.data +
                    interpreter->size_stack.size +
                    1;
                ret =
                    DArrayIdx_push_back_batch(
                        &interpreter->size_stack,
                        iter_size,
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size]
                            .data
                            .arr
                            .dim - 1
                    );
                if (ret) {
                    return SL_ERR_OUT_OF_MEMORY;
                }
            }
        }
        switch (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size + 1]
                .type) {
        case SL_TOKEN_TYPE_INT_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_literal;
            break;
        case SL_TOKEN_TYPE_INT_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(int64_t*)(interpreter->global.data + offset);
                break;
            default:
                break;
            }
            break;
        case SL_TOKEN_TYPE_CHAR_LITERAL:
            op_b_int =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_literal;
            break;
        case SL_TOKEN_TYPE_CHAR_VAR:
            offset =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .int_var
                    .idx;
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size + 1]
                    .data
                    .char_var
                    .location) {
            case SL_VARIABLE_LOCATION_GLOBAL:
                op_b_int = *(char*)(interpreter->global.data + offset);
                break;
            case SL_VARIABLE_LOCATION_DIRECT:
                op_b_int =
                    *(char*)
                        interpreter
                            ->operation_stack
                            .data[interpreter->operation_stack.size + 1]
                            .data
                            .char_var
                            .direct;
            default:
                break;
            }
            break;
        default:
            break;
        }
        if (interpreter->bound_check) {
            if (
                op_b_int < 0 ||
                (size_t)op_b_int >=
                interpreter
                    ->size_stack
                    .data[interpreter->size_stack.size]) {
                return SL_ERR_IDX_OUT_OF_BOUND;
            }
        }
        if (
            interpreter
                ->operation_stack
                .data[interpreter->operation_stack.size]
                .data
                .arr
                .dim == 1) {
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type) {
            case SL_TOKEN_TYPE_INT_VAR:
                res.data.int_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.int_var.direct = ((int64_t*)op_a_arr_imm) + op_b_int;
                break;
            case SL_TOKEN_TYPE_FLOAT_VAR:
                res.data.float_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.float_var.direct = ((double*)op_a_arr_imm) + op_b_int;
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                res.data.char_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.char_var.direct = op_a_arr_imm + op_b_int;
                break;
            case SL_TOKEN_TYPE_STR_VAR:
                res.data.str_var.location = SL_VARIABLE_LOCATION_DIRECT;
                res.data.str_var.direct = ((String**)op_a_arr_imm) + op_b_int;
                break;
            default:
                break;
            }
        } else {
            res.data.arr.dim =
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .dim - 1;
            res.data.arr.var_data.location = SL_VARIABLE_LOCATION_DIRECT;
            iter_size =
                interpreter->size_stack.data +
                interpreter->size_stack.size +
                1;
            for (
                size_t i = 0;
                i <
                    interpreter
                        ->operation_stack
                        .data[interpreter->operation_stack.size]
                        .data
                        .arr
                        .dim - 1;
                ++i) {
                dim_size *= *iter_size;
            }
            switch (
                interpreter
                    ->operation_stack
                    .data[interpreter->operation_stack.size]
                    .data
                    .arr
                    .type) {
            case SL_TOKEN_TYPE_INT_VAR:
                res.data.arr.var_data.direct =
                    op_a_arr_imm + op_b_int * (dim_size << 3);
                break;
            case SL_TOKEN_TYPE_FLOAT_VAR:
                res.data.arr.var_data.direct =
                    op_a_arr_imm + op_b_int * (dim_size << 3);
                break;
            case SL_TOKEN_TYPE_CHAR_VAR:
                res.data.arr.var_data.direct =
                    op_a_arr_imm + op_b_int * dim_size;
                break;
            case SL_TOKEN_TYPE_STR_VAR:
                res.data.arr.var_data.direct =
                    op_a_arr_imm + op_b_int * (dim_size << 3);
                break;
            default:
                break;
            }

        }
        break;
    default:
        break;
    }
    ret = DArraySLToken_push_back(&interpreter->operation_stack, &res);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

#endif
