#include "handler_arm64_linux.h"

int handle_declarations_arm64_linux(Generator *generator, FILE *fasm) {
    String *iter_str_lit = generator->parser.str_lit.data;
    for (
        size_t i = 0;
        i < generator->parser.str_lit.size;
        ++i,
        ++iter_str_lit
    ) {
        fprintf(
            fasm,
            "str_lit_%lu: .asciz \"",
            i
        );
        for (char *j = *iter_str_lit; *j; ++j) {
            switch (*j) {
            case '\t':
                fprintf(fasm, "%s", "\\t");
                break;
            case '\n':
                fprintf(fasm, "%s", "\\n");
                break;
            case '"':
                fprintf(fasm, "%s", "\\\"");
                break;
            default:
                fputc(*j, fasm);
            }
        }
        fprintf(fasm, "%c\n", '"');
    }
    HashSetStringNode *iter_int_name = generator->parser.int_name.data;
    for (
        size_t i = 0;
        i < generator->parser.int_name.capacity;
        ++i,
        ++iter_int_name
    ) {
        if (iter_int_name->in_use) {
            fprintf(
                fasm,
                "int_name_%s: .quad 0\n",
                iter_int_name->item);
        }
    }
    HashSetStringNode *iter_str_name = generator->parser.str_name.data;
    for (
        size_t i = 0;
        i < generator->parser.str_name.capacity;
        ++i,
        ++iter_str_name
    ) {
        if (iter_str_name->in_use) {
            fprintf(
                fasm,
                "str_name_%s: .quad 0\n",
                iter_str_name->item);
        }
    }
    HashMapStringSizeNode *iter_arr_name = generator->parser.arr_name.data;
    for (
        size_t i = 0;
        i < generator->parser.arr_name.capacity;
        ++i,
        ++iter_arr_name
    ) {
        if (iter_arr_name->in_use) {
            fprintf(
                fasm,
                "arr_name_%s: .fill %lu, 8, 0\n",
                iter_arr_name->key,
                iter_arr_name->value
            );
        }
    }
    HashMapStringFunctionNode *iter_function_name =
        generator->parser.function_name.data;
    for (
        size_t i = 0;
        i < generator->parser.function_name.capacity;
        ++i,
        ++iter_function_name
    ) {
        if (iter_function_name->in_use) {
            Token *iter_args = iter_function_name->value.args.data;
            for (
                size_t i = 0;
                i < iter_function_name->value.args.size;
                ++i, ++iter_args
            ) {
                if (iter_args->type == TOKEN_ARR_NAME) {
                    fprintf(
                        fasm,
                        "function_%s_arr_name_%s: .quad 0\n",
                        iter_function_name->key,
                        iter_args->data.arr_name
                    );
                }
            }
            HashSetStringNode *iter_function_name_int_name =
                iter_function_name->value.int_name.data;
            for (
                size_t i = 0;
                i < iter_function_name->value.int_name.capacity;
                ++i,
                ++iter_function_name_int_name
            ) {
                if (iter_function_name_int_name->in_use) {
                    fprintf(
                        fasm,
                        "function_%s_int_name_%s: .quad 0\n",
                        iter_function_name->key,
                        iter_function_name_int_name->item);
                }
            }
            HashSetStringNode *iter_function_name_str_name =
                iter_function_name->value.str_name.data;
            for (
                size_t i = 0;
                i < iter_function_name->value.str_name.capacity;
                ++i,
                ++iter_function_name_str_name
            ) {
                if (iter_function_name_str_name->in_use) {
                    fprintf(
                        fasm,
                        "function_%s_str_name_%s: .quad 0\n",
                        iter_function_name->key,
                        iter_function_name_str_name->item);
                }
            }
        }
    }
    return 0;
}

static int handle_token_int_name(
    Generator *generator,
    Token *token,
    String function,
    FILE *fasm
) {
    if (generator->stack.size == 100) {
        return ERR_STACK_OVERFLOW;
    }
    int ret = DArrayToken_push(&generator->stack, token);
    if (ret) {
        return ret;
    }
    fprintf(
        fasm,
        "    ldr x9, =stack_ptr\n"
        "    ldr x10, [x9]\n"
        "    ldr x11, =%s%s%sint_name_%s\n"
        "    str x11, [x10]\n"
        "    add x10, x10, #8\n"
        "    str x10, [x9]\n",
        function ? "function_" : "",
        function ? function : "",
        function ? "_" : "",
        token->data.int_name
    );
    return 0;
}

static int handle_token_str_name(
    Generator *generator,
    Token *token,
    String function,
    FILE *fasm
) {
    if (generator->stack.size == 100) {
        return ERR_STACK_OVERFLOW;
    }
    int ret = DArrayToken_push(&generator->stack, token);
    if (ret) {
        return ret;
    }
    fprintf(
        fasm,
        "    ldr x9, =stack_ptr\n"
        "    ldr x10, [x9]\n"
        "    ldr x11, =%s%s%sstr_name_%s\n"
        "    str x11, [x10]\n"
        "    add x10, x10, #8\n"
        "    str x10, [x9]\n",
        function ? "function_" : "",
        function ? function : "",
        function ? "_" : "",
        token->data.str_name
    );
    return 0;
}

static int handle_token_int_lit(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    if (generator->stack.size == 100) {
        return ERR_STACK_OVERFLOW;
    }
    int ret = DArrayToken_push(&generator->stack, token);
    if (ret) {
        return ret;
    }
    fprintf(
        fasm,
        "    ldr x9, =stack_ptr\n"
        "    ldr x10, [x9]\n"
        "    mov x11, #%ld\n"
        "    str x11, [x10]\n"
        "    add x10, x10, #8\n"
        "    str x10, [x9]\n",
        token->data.int_lit
    );
    return 0;
}

static int handle_token_str_lit(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    if (generator->stack.size == 100) {
        return ERR_STACK_OVERFLOW;
    }
    int ret = DArrayToken_push(&generator->stack, token);
    if (ret) {
        return ret;
    }
    fprintf(
        fasm,
        "    ldr x9, =stack_ptr\n"
        "    ldr x10, [x9]\n"
        "    ldr x11, =str_lit_%lu\n"
        "    str x11, [x10]\n"
        "    add x10, x10, #8\n"
        "    str x10, [x9]\n",
        token->data.str_lit
    );
    return 0;
}

static int handle_token_arr_name(
    Generator *generator,
    Token *token,
    String function,
    FILE *fasm
) {
    if (generator->stack.size == 100) {
        return ERR_STACK_OVERFLOW;
    }
    int ret = DArrayToken_push(&generator->stack, token);
    if (ret) {
        return ret;
    }
    fprintf(
        fasm,
        "    ldr x9, =stack_ptr"
        "    ldr x10, [x9]"
        "    ldr x11, =%s%s%sarr_name_%s\n"
        "%s"
        "    str x11, [x10]\n"
        "    add x10, x10, #8\n"
        "    str x10, [x9]\n",
        function ? "function_" : "",
        function ? function : "",
        function ? "_" : "",
        token->data.arr_name,
        function ? "    ldr x11, [x11]\n" : ""
    );
    return 0;
}

static int handle_token_operator(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    Token op1, op2, res;
    Token *back = generator->stack.data + generator->stack.size;
    int ret = 0;
    switch (token->data.operater) {
    case TOKEN_OPERATOR_ADD:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    add x11, x11, x12\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    add x11, x11, x12\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                   "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    add x11, x11, x12\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    add x11, x11, x12\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_SUBTRACT:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sub x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sub x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sub x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sub x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_MULTIPLY:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    mul x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    mul x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    mul x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    mul x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_DIVIDE:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sdiv x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sdiv x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sdiv x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sdiv x11, x12, x11\n"
                    "    str x11, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_MODULO:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sdiv x13, x12, x11\n"
                    "    msub x13, x13, x11, x12\n"
                    "    str x13, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    sdiv x13, x12, x11\n"
                    "    msub x13, x13, x11, x12\n"
                    "    str x13, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sdiv x13, x12, x11\n"
                    "    msub x13, x13, x11, x12\n"
                    "    str x13, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    ldr x12, [x12]\n"
                    "    sdiv x13, x12, x11\n"
                    "    msub x13, x13, x11, x12\n"
                    "    str x13, [x10]\n"
                    "    add x10, x10, 8\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_ASSIGN:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    str x11, [x12]\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    str x11, [x12]\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_STR_NAME:
            switch (op2.type) {
            case TOKEN_STR_LIT:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    str x11, [x12]\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            case TOKEN_STR_NAME:
                fputs(
                    "    ldr x9, =stack_ptr\n"
                    "    ldr x10, [x9]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x11, [x10]\n"
                    "    ldr x11, [x11]\n"
                    "    sub x10, x10, #8\n"
                    "    ldr x12, [x10]\n"
                    "    str x11, [x12]\n"
                    "    str x10, [x9]\n",
                    fasm
                );
                break;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_OPERATOR_GT:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LT:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovlq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovlq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovlq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovlq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_GTE:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgeq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgeq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgeq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovgeq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LTE:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovleq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovleq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovleq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovleq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_EQ:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmoveq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmoveq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmoveq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmoveq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_NEQ:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovneq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovneq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovneq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %rdx\n"
                    "    movabsq $1, %r10\n"
                    "    cmpq %rbx, %rcx\n"
                    "    cmovneq %r10, %rdx\n"
                    "    movq %rdx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_RSHIFT:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movb (%rax), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    sarq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    sarq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movb (%rax), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    sarq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    sarq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LSHIFT:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movb (%rax), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    shlq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    shlq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movb (%rax), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    shlq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    shlq %cl, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_BNOT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    notq %rbx\n"
                "    movq %rbx, (%rax)\n"
                "    addq $8, %rax\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        case TOKEN_INT_NAME:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movq (%rbx), %rbx\n"
                "    notq %rbx\n"
                "    movq %rbx, (%rax)\n"
                "    addq $8, %rax\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LNOT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movabsq $0, %r10\n"
                "    movabsq $-2, %r11\n"
                "    movabsq $-1, %r12\n"
                "    cmpq %r10, %rbx\n"
                "    cmovneq %r12, %r11\n"
                "    movq %r11, %rbx\n"
                "    notq %rbx\n"
                "    movq %rbx, (%rax)\n"
                "    addq $8, %rax\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        case TOKEN_INT_NAME:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movq (%rbx), %rbx\n"
                "    movabsq $0, %r10\n"
                "    movabsq $-2, %r11\n"
                "    movabsq $-1, %r12\n"
                "    cmpq %r10, %rbx\n"
                "    cmovneq %r12, %r11\n"
                "    movq %r11, %rbx\n"
                "    notq %rbx\n"
                "    movq %rbx, (%rax)\n"
                "    addq $8, %rax\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_BAND:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LAND:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    andq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_BOR:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_LOR:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rbx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    movabsq $0, %r10\n"
                    "    movabsq $0, %r11\n"
                    "    movabsq $1, %r12\n"
                    "    cmpq %r10, %rcx\n"
                    "    cmovneq %r12, %r11\n"
                    "    movq %r11, %rcx\n"
                    "    orq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_XOR:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    xorq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    xorq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    xorq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    xorq %rcx, %rbx\n"
                    "    movq %rbx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        res.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    case TOKEN_OPERATOR_INCREMENT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_NAME:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movq (%rbx), %rcx\n"
                "    addq $1, %rcx\n"
                "    movq %rcx, (%rbx)\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_OPERATOR_INCREMENT_BY:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rdx\n"
                    "    addq %rbx, %rdx\n"
                    "    movq %rdx, (%rcx)\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rdx\n"
                    "    addq %rbx, %rdx\n"
                    "    movq %rdx, (%rcx)\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_OPERATOR_DECREMENT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_NAME:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movq (%rbx), %rcx\n"
                "    subq $1, %rcx\n"
                "    movq %rcx, (%rbx)\n"
                "    movq %rax, stack_ptr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_OPERATOR_DECREMENT_BY:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_INT_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rdx\n"
                    "    subq %rbx, %rdx\n"
                    "    movq %rdx, (%rcx)\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rdx\n"
                    "    subq %rbx, %rdx\n"
                    "    movq %rdx, (%rcx)\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_OPERATOR_IDX:
        if (generator->stack.size < 2) {
            return ERR_INVALID_OPERAND;
        }
        op1 = back[-2];
        op2 = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op1.type) {
        case TOKEN_ARR_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    shlq $3, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movq %rcx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    shlq $3, %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movq %rcx, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            res.type = TOKEN_INT_NAME;
            break;
        case TOKEN_STR_NAME:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    movb %cl, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    movb %cl, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            res.type = TOKEN_STR_CHAR;
            break;
        case TOKEN_STR_LIT:
            switch (op2.type) {
            case TOKEN_INT_LIT:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    movb %cl, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    addq %rbx, %rcx\n"
                    "    movb (%rcx), %cl\n"
                    "    movb %cl, (%rax)\n"
                    "    addq $8, %rax\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            res.type = TOKEN_STR_CHAR;
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        ret = DArrayToken_push(&generator->stack, &res);
        if (ret) {
            return ret;
        }
        break;
    }
    return 0;
}

static int handle_token_command(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    Token op, tgt;
    Token *back = generator->stack.data + generator->stack.size;
    int ret = 0;
    switch (token->data.command.type) {
    case TOKEN_COMMAND_INPUT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op.type) {
        case TOKEN_INT_NAME:
            fputs(
                "    call input\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_PRINT:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op.type) {
        case TOKEN_INT_LIT:
            fputs(
                "    bl print_int\n",
                fasm
            );
            break;
        case TOKEN_INT_NAME:
            fputs(
                "    ldr x9, =stack_ptr\n"
                "    ldr x10, [x9]\n"
                "    sub x10, x10, #8\n"
                "    ldr x11, [x10]\n"
                "    ldr x11, [x11]\n"
                "    str x11, [x10]\n"
                "    bl print_int\n",
                fasm
            );
            break;
        case TOKEN_STR_LIT:
            fputs(
                "    bl calc_strlen\n"
                "    bl print_str\n",
                fasm
            );
            break;
        case TOKEN_STR_NAME:
            fputs(
                "    ldr x9, =stack_ptr\n"
                "    ldr x10, [x9]\n"
                "    sub x10, x10, #8\n"
                "    ldr x11, [x10]\n"
                "    ldr x11, [x11]\n"
                "    str x11, [x10]\n"
                "    bl calc_strlen\n"
                "    bl print_str\n",
                fasm
            );
            break;
        case TOKEN_STR_CHAR:
            fputs(
                "    call print_chr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_PRINTLN:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op.type) {
        case TOKEN_INT_LIT:
            fputs(
                "    bl println_int\n",
                fasm
            );
            break;
        case TOKEN_INT_NAME:
            fputs(
                "    ldr x9, =stack_ptr\n"
                "    ldr x10, [x9]\n"
                "    sub x10, x10, #8\n"
                "    ldr x11, [x10]\n"
                "    ldr x11, [x11]\n"
                "    str x11, [x10]\n"
                "    bl println_int\n",
                fasm
            );
            break;
        case TOKEN_STR_LIT:
            fputs(
                "    bl calc_strlen\n"
                "    bl println_str\n",
                fasm
            );
            break;
        case TOKEN_STR_NAME:
            fputs(
                "    ldr x9, =stack_ptr\n"
                "    ldr x10, [x9]\n"
                "    sub x10, x10, #8\n"
                "    ldr x11, [x10]\n"
                "    ldr x11, [x11]\n"
                "    str x11, [x10]\n"
                "    bl calc_strlen\n"
                "    bl println_str\n",
                fasm
            );
            break;
        case TOKEN_STR_CHAR:
            fputs(
                "    call println_chr\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_IF:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        tgt =
            generator
                ->parser
                .cur_token_buf
                ->data[token->data.command.data.command_if.offset];
        switch (op.type) {
        case TOKEN_INT_LIT:
            fprintf(
                fasm,
                "    movq stack_ptr, %%rax\n"
                "    subq $8, %%rax\n"
                "    movq (%%rax), %%rbx\n"
                "    movq %%rax, stack_ptr\n"
                "    movabsq $0, %%r10\n"
                "    cmpq %%rbx, %%r10\n"
                "    je %s_%lu\n",
                tgt.data.command.type == TOKEN_COMMAND_ELSE ?
                "else" :
                "eif",
                tgt.data.command.type == TOKEN_COMMAND_ELSE ?
                tgt.data.command.data.command_else.idx :
                tgt.data.command.data.command_end_if
            );
            break;
        case TOKEN_INT_NAME:
            fprintf(
                fasm,
                "    movq stack_ptr, %%rax\n"
                "    subq $8, %%rax\n"
                "    movq (%%rax), %%rbx\n"
                "    movq (%%rbx), %%rbx\n"
                "    movq %%rax, stack_ptr\n"
                "    movabsq $0, %%r10\n"
                "    cmpq %%rbx, %%r10\n"
                "    je %s_%lu\n",
                tgt.data.command.type == TOKEN_COMMAND_ELSE ?
                "else" :
                "eif",
                tgt.data.command.type == TOKEN_COMMAND_ELSE ?
                tgt.data.command.data.command_else.idx :
                tgt.data.command.data.command_end_if
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_ELSE:
        tgt =
            generator
                ->parser
                .cur_token_buf
                ->data[token->data.command.data.command_else.offset];
        fprintf(
            fasm,
            "    jmp eif_%lu\n"
            "else_%lu:\n",
            tgt.data.command.data.command_end_if,
            token->data.command.data.command_else.idx
        );
        break;
    case TOKEN_COMMAND_END_IF:
        fprintf(
            fasm,
            "eif_%lu:\n",
            token->data.command.data.command_end_if
        );
        break;
    case TOKEN_COMMAND_WHILE:
        fprintf(
            fasm,
            "loop_%lu:\n",
            token->data.command.data.command_while
        );
        break;
    case TOKEN_COMMAND_DO:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        tgt =
            generator
                ->parser
                .cur_token_buf
                ->data[token->data.command.data.command_do.offset];
        switch (op.type) {
        case TOKEN_INT_LIT:
            fprintf(
                fasm,
                "    movq stack_ptr, %%rax\n"
                "    subq $8, %%rax\n"
                "    movq (%%rax), %%rbx\n"
                "    movq %%rax, stack_ptr\n"
                "    movabsq $0, %%r10\n"
                "    cmpq %%rbx, %%r10\n"
                "    je eloop_%lu\n",
                tgt.data.command.data.command_end_loop.idx
            );
            break;
        case TOKEN_INT_NAME:
            fprintf(
                fasm,
                "    movq stack_ptr, %%rax\n"
                "    subq $8, %%rax\n"
                "    movq (%%rax), %%rbx\n"
                "    movq (%%rbx), %%rbx\n"
                "    movq %%rax, stack_ptr\n"
                "    movabsq $0, %%r10\n"
                "    cmpq %%rbx, %%r10\n"
                "    je eloop_%lu\n",
                tgt.data.command.data.command_end_loop.idx
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_END_LOOP:
        tgt =
            generator
                ->parser
                .cur_token_buf
                ->data[token->data.command.data.command_else.offset];
        fprintf(
            fasm,
            "    jmp loop_%lu\n"
            "eloop_%lu:\n",
            tgt.data.command.data.command_while,
            token->data.command.data.command_end_loop.idx
        );
        break;
    case TOKEN_COMMAND_SRAND:
        if (generator->stack.size < 1) {
            return ERR_INVALID_OPERAND;
        }
        op = back[-1];
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (op.type) {
        case TOKEN_INT_LIT:
            fputs(
                "    call srand\n",
                fasm
            );
            break;
        case TOKEN_INT_NAME:
            fputs(
                "    movq stack_ptr, %rax\n"
                "    subq $8, %rax\n"
                "    movq (%rax), %rbx\n"
                "    movq (%rbx), %rbx\n"
                "    movq %rbx, (%rax)\n"
                "    addq $8, %rax\n"
                "    movq %rax, stack_ptr\n"
                "    call srand\n",
                fasm
            );
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    case TOKEN_COMMAND_RAND:
        fputs(
            "    call rand\n",
            fasm
        );
        tgt.type = TOKEN_INT_LIT;
        ret = DArrayToken_push(&generator->stack, &tgt);
        if (ret) {
            return ret;
        }
        break;
    }
    return 0;
}

static int handle_token_function_call(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    bool found;
    int ret =
        HashMapStringFunction_check(
            &generator->parser.function_name,
            &token->data.function_call,
            &found
        );
    if (ret) {
        return ret;
    }
    if (!found) {
        return ERR_FUNCTION_NOT_DEFINED;
    }
    Function *function;
    ret =
        HashMapStringFunction_fetch(
            &generator->parser.function_name,
            &token->data.function_call,
            &function
        );
    if (ret) {
        return ret;
    }
    size_t size = function->args.size;
    if (generator->stack.size < size) {
        return ERR_INVALID_OPERAND;
    }
    Token *iter_args = function->args.data + size - 1;
    Token *iter_stack = generator->stack.data + generator->stack.size - 1;
    for (size_t i = 0; i < size; ++i, --iter_args, --iter_stack) {
        ret = DArrayToken_pop(&generator->stack);
        if (ret) {
            return ret;
        }
        switch (iter_args->type) {
        case TOKEN_INT_NAME:
            switch (iter_stack->type) {
            case TOKEN_INT_LIT:
                fprintf(
                    fasm,
                    "    movq stack_ptr, %%rax\n"
                    "    subq $8, %%rax\n"
                    "    movq (%%rax), %%rbx\n"
                    "    movq %%rbx, function_%s_int_name_%s\n"
                    "    movq %%rax, stack_ptr\n",
                    token->data.function_call,
                    iter_args->data.int_name
                );
                break;
            case TOKEN_INT_NAME:
                fprintf(
                    fasm,
                    "    movq stack_ptr, %%rax\n"
                    "    subq $8, %%rax\n"
                    "    movq (%%rax), %%rbx\n"
                    "    movq (%%rbx), %%rbx\n"
                    "    movq %%rbx, function_%s_int_name_%s\n"
                    "    movq %%rax, stack_ptr\n",
                    token->data.function_call,
                    iter_args->data.int_name
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_STR_NAME:
            switch (iter_stack->type) {
            case TOKEN_STR_LIT:
                fprintf(
                    fasm,
                    "    movq stack_ptr, %%rax\n"
                    "    subq $8, %%rax\n"
                    "    movq (%%rax), %%rbx\n"
                    "    movq %%rbx, function_%s_str_name_%s\n"
                    "    movq %%rax, stack_ptr\n",
                    token->data.function_call,
                    iter_args->data.str_name
                );
                break;
            case TOKEN_STR_NAME:
                fprintf(
                    fasm,
                    "    movq stack_ptr, %%rax\n"
                    "    subq $8, %%rax\n"
                    "    movq (%%rax), %%rbx\n"
                    "    movq (%%rbx), %%rbx\n"
                    "    movq %%rbx, function_%s_str_name_%s\n"
                    "    movq %%rax, stack_ptr\n",
                    token->data.function_call,
                    iter_args->data.str_name
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        case TOKEN_ARR_NAME:
            switch (iter_stack->type) {
            case TOKEN_ARR_NAME:
                fprintf(
                    fasm,
                    "    movq stack_ptr, %%rax\n"
                    "    subq $8, %%rax\n"
                    "    movq (%%rax), %%rbx\n"
                    "    movq %%rbx, function_%s_arr_name_%s\n"
                    "    movq %%rax, stack_ptr\n",
                    token->data.function_call,
                    iter_args->data.arr_name
                );
                break;
            default:
                return ERR_INVALID_OPERAND;
            }
            break;
        }
    }
    fprintf(
        fasm,
        "    call function_%s\n",
        token->data.function_call
    );
    Function *cur_function;
    ret =
        HashMapStringFunction_fetch(
            &generator->parser.function_name,
            &token->data.function_call,
            &cur_function
        );
    if (ret) {
        return ret;
    }
    Token *iter_ret_vals = cur_function->ret_vals.data;
    for (
        size_t i = 0;
        i < function->ret_vals.size;
        ++i, ++iter_ret_vals) {
        ret = DArrayToken_push(&generator->stack, iter_ret_vals);
        if (ret) {
            return ret;
        }
        if (generator->stack.size > 100) {
            return ERR_STACK_OVERFLOW;
        }
    }
    return 0;
}

int handle_function_definitions_arm64_linux(Generator *generator, FILE *fasm) {
    HashMapStringFunctionNode *iter_function_name =
        generator->parser.function_name.data;
    for (
        size_t i = 0;
        i < generator->parser.function_name.capacity;
        ++i, ++iter_function_name
    ) {
        if (iter_function_name->in_use) {
            fprintf(
                fasm,
                "function_%s:\n",
                iter_function_name->key
            );
            generator->parser.cur_token_buf =
                &iter_function_name->value.tokens;
            Token *iter_token = iter_function_name->value.tokens.data;
            int ret = 0;
            for (
                size_t i = 0;
                i < iter_function_name->value.tokens.size;
                ++i, ++iter_token
            ) {
                switch (iter_token->type) {
                case TOKEN_INT_NAME:
                    ret =
                        handle_token_int_name(
                            generator,
                            iter_token,
                            iter_function_name->key,
                            fasm
                        );
                    break;
                case TOKEN_STR_NAME:
                    ret =
                        handle_token_str_name(
                            generator,
                            iter_token,
                            iter_function_name->key,
                            fasm
                        );
                    break;
                case TOKEN_INT_LIT:
                    ret =
                        handle_token_int_lit(generator, iter_token, fasm);
                    break;
                case TOKEN_STR_LIT:
                    ret =
                        handle_token_str_lit(generator, iter_token, fasm);
                    break;
                case TOKEN_ARR_NAME:
                    ret =
                        handle_token_arr_name(
                            generator,
                            iter_token,
                            iter_function_name->key,
                            fasm);
                    break;
                case TOKEN_OPERATOR:
                    ret =
                        handle_token_operator(generator, iter_token, fasm);
                    break;
                case TOKEN_COMMAND:
                    ret =
                        handle_token_command(generator, iter_token, fasm);
                    break;
                case TOKEN_FUNCTION_CALL:
                    ret =
                        handle_token_function_call(generator, iter_token, fasm);
                    break;
                }
                if (ret) {
                    printf(
                        "error in function %s at position %lu\n",
                        iter_function_name->key,
                        i
                    );
                    return ret;
                }
            }
            fputs("    ret\n", fasm);
        }
    }
    return 0;
}

int handle_tokens_arm64_linux(Generator *generator, FILE *fasm) {
    generator->parser.cur_token_buf =
        &generator->parser.tokens;
    Token *iter_token = generator->parser.tokens.data;
    int ret = 0;
    for (size_t i = 0; i < generator->parser.tokens.size; ++i, ++iter_token) {
        switch (iter_token->type) {
        case TOKEN_INT_NAME:
            ret =
                handle_token_int_name(generator, iter_token, NULL, fasm);
            break;
        case TOKEN_STR_NAME:
            ret =
                handle_token_str_name(generator, iter_token, NULL, fasm);
            break;
        case TOKEN_INT_LIT:
            ret =
                handle_token_int_lit(generator, iter_token, fasm);
            break;
        case TOKEN_STR_LIT:
            ret =
                handle_token_str_lit(generator, iter_token, fasm);
            break;
        case TOKEN_ARR_NAME:
            ret =
                handle_token_arr_name(generator, iter_token, NULL, fasm);
            break;
        case TOKEN_OPERATOR:
            ret =
                handle_token_operator(generator, iter_token, fasm);
            break;
        case TOKEN_COMMAND:
            ret =
                handle_token_command(generator, iter_token, fasm);
            break;
        case TOKEN_FUNCTION_CALL:
            ret =
                handle_token_function_call(generator, iter_token, fasm);
            break;
        }
        if (ret) {
            printf("error at position %lu\n", i);
            return ret;
        }
    }
    return 0;
}
