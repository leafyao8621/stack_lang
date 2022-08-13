#include "handler_x86_64_linux.h"

int handle_declarations_x86_64_linux(Generator *generator, FILE *fasm) {
    String *iter_str_lit = generator->parser.str_lit.data;
    for (
        size_t i = 0;
        i < generator->parser.str_lit.size;
        ++i,
        ++iter_str_lit
    ) {
        fprintf(
            fasm,
            "str_lit_%lu:\n    .asciz \"",
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
                "int_name_%s:\n    .quad 0\n",
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
                "str_name_%s:\n    .quad 0\n",
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
                "arr_name_%s:\n    .fill %lu\n",
                iter_arr_name->key,
                iter_arr_name->value * 8);
        }
    }
    return 0;
}

static int handle_token_int_name(
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
        "    movq stack_ptr, %%rax\n"
        "    movabsq $int_name_%s, %%rbx\n"
        "    movq %%rbx, (%%rax)\n"
        "    addq $8, %%rax\n"
        "    movq %%rax, stack_ptr\n",
        token->data.int_name
    );
    return 0;
}

static int handle_token_str_name(
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
        "    movq stack_ptr, %%rax\n"
        "    movabsq $str_name_%s, %%rbx\n"
        "    movq %%rbx, (%%rax)\n"
        "    addq $8, %%rax\n"
        "    movq %%rax, stack_ptr\n",
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
        "    movq stack_ptr, %%rax\n"
        "    movabsq $%ld, %%rbx\n"
        "    movq %%rbx, (%%rax)\n"
        "    addq $8, %%rax\n"
        "    movq %%rax, stack_ptr\n",
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
        "    movq stack_ptr, %%rax\n"
        "    movabsq $str_lit_%lu, %%rbx\n"
        "    movq %%rbx, (%%rax)\n"
        "    addq $8, %%rax\n"
        "    movq %%rax, stack_ptr\n",
        token->data.str_lit
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
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    addq %rcx, %rbx\n"
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
                    "    addq %rcx, %rbx\n"
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
                    "    addq %rcx, %rbx\n"
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
                    "    addq %rcx, %rbx\n"
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
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    subq %rbx, %rcx\n"
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
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    subq %rbx, %rcx\n"
                    "    movq %rcx, (%rax)\n"
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
                    "    subq %rbx, %rcx\n"
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
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq (%rcx), %rcx\n"
                    "    subq %rbx, %rcx\n"
                    "    movq %rcx, (%rax)\n"
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    imulq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    imulq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    imulq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    imulq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rax, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rdx, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rdx, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rdx, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                fputs(
                    "    movq stack_ptr, %r10\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rbx\n"
                    "    movq (%rbx), %rbx\n"
                    "    subq $8, %r10\n"
                    "    movq (%r10), %rax\n"
                    "    movq (%rax), %rax\n"
                    "    cqo\n"
                    "    idivq %rbx\n"
                    "    movq %rdx, (%r10)\n"
                    "    addq $8, %r10\n"
                    "    movq %r10, stack_ptr\n",
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
                    "    movq stack_ptr, %rax\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rbx\n"
                    "    subq $8, %rax\n"
                    "    movq (%rax), %rcx\n"
                    "    movq %rbx, (%rcx)\n"
                    "    movq %rax, stack_ptr\n",
                    fasm
                );
                break;
            case TOKEN_INT_NAME:
                break;
            }
            break;
        case TOKEN_STR_NAME:
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
                    "    sarq %rbx\n"
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
                    "    sarq %rbx\n"
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
                    "    sarq %rbx\n"
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
                    "    sarq %rbx\n"
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
                    "    shlq %rbx\n"
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
                    "    shlq %rbx\n"
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
                    "    shlq %rbx\n"
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
                    "    shlq %rbx\n"
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
    }
    return 0;
}

static int handle_token_command(
    Generator *generator,
    Token *token,
    FILE *fasm
) {
    Token op;
    Token *back = generator->stack.data + generator->stack.size;
    int ret = 0;
    switch (token->data.command.type) {
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
                "call print_int\n",
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
                "    call print_int\n",
                fasm
            );
            break;
        case TOKEN_STR_LIT:
            break;
        case TOKEN_STR_NAME:
            break;
        case TOKEN_STR_CHAR:
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
                "call println_int\n",
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
                "    call println_int\n",
                fasm
            );
            break;
        case TOKEN_STR_LIT:
            break;
        case TOKEN_STR_NAME:
            break;
        case TOKEN_STR_CHAR:
            break;
        default:
            return ERR_INVALID_OPERAND;
        }
        break;
    }
    return 0;
}

int handle_tokens_x86_64_linux(Generator *generator, FILE *fasm) {
    Token *iter_token = generator->parser.tokens.data;
    int ret = 0;
    for (size_t i = 0; i < generator->parser.tokens.size; ++i, ++iter_token) {
        switch (iter_token->type) {
        case TOKEN_INT_NAME:
            ret =
                handle_token_int_name(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        case TOKEN_STR_NAME:
            ret =
                handle_token_str_name(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        case TOKEN_INT_LIT:
            ret =
                handle_token_int_lit(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        case TOKEN_STR_LIT:
            ret =
                handle_token_str_lit(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        case TOKEN_OPERATOR:
            ret =
                handle_token_operator(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        case TOKEN_COMMAND:
            ret =
                handle_token_command(generator, iter_token, fasm);
            if (ret) {
                return ret;
            }
            break;
        }
    }
    return 0;
}
