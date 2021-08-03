#include <stdio.h>
#include "vm.h"

short nv[702];
char *sv[26];
struct Array av[26];
char memory[320000];
char *mp;
static struct Token stack[100];
struct Token code[320000];

int process_op(struct Token *tok, struct Token **sp) {
    short op1 = 0;
    short op2 = 0;
    short *op1_num_var = 0;
    short *op2_num_var = 0;
    switch (tok->data.op) {
    case TOKEN_OP_ADD:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 + op2;
        ++(*sp);
        break;
    case TOKEN_OP_SUBTRACT:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 - op2;
        ++(*sp);
        break;
    case TOKEN_OP_MULTIPLY:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 * op2;
        ++(*sp);
        break;
    case TOKEN_OP_DIVIDE:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 / op2;
        ++(*sp);
        break;
    case TOKEN_OP_MOD:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 % op2;
        ++(*sp);
        break;
    case TOKEN_OP_ASSIGN:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM_VAR:
            op2_num_var = (*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        *op2_num_var = op1;
        break;
    case TOKEN_OP_GREATER:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 > op2;
        ++(*sp);
        break;
    case TOKEN_OP_LESS:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 < op2;
        ++(*sp);
        break;
    case TOKEN_OP_GEQ:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 >= op2;
        ++(*sp);
        break;
    case TOKEN_OP_LEQ:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 <= op2;
        ++(*sp);
        break;
    case TOKEN_OP_EQUAL:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 == op2;
        ++(*sp);
        break;
    case TOKEN_OP_UNEQUAL:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 != op2;
        ++(*sp);
        break;
    case TOKEN_OP_LSHIFT:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 << op2;
        ++(*sp);
        break;
    case TOKEN_OP_RSHIFT:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 >> op2;
        ++(*sp);
        break;
    case TOKEN_OP_BAND:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 & op2;
        ++(*sp);
        break;
    case TOKEN_OP_LAND:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 && op2;
        ++(*sp);
        break;
    case TOKEN_OP_BOR:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 | op2;
        ++(*sp);
        break;
    case TOKEN_OP_LOR:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = op1 || op2;
        ++(*sp);
        break;
    case TOKEN_OP_BNOT:
        if (*sp - stack < 1) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 1;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = ~op1;
        ++(*sp);
        break;
    case TOKEN_OP_LNOT:
        if (*sp - stack < 1) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 1;
        switch ((*sp)[0].type) {
        case TOKEN_NUM:
            op1 = (*sp)[0].data.num;
            break;
        case TOKEN_NUM_VAR:
            op1 = *(*sp)[0].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        (**sp).type = TOKEN_NUM;
        (**sp).data.num = !op1;
        ++(*sp);
        break;
    case TOKEN_OP_IDX:
        if (*sp - stack < 2) {
            puts("Invalid number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)[0].type) {
        case TOKEN_ARR_VAR:
            op1_num_var = (*sp)[0].data.arr_var->head;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            op2 = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            op2 = *(*sp)[1].data.num_var;
            break;
        default:
            puts("Invalid operand");
            return 1;
        }
        if (op2 < 0 || op2 >= (*sp)[0].data.arr_var->offset) {
            puts("Index out of bound");
            return 1;
        }
        (**sp).type = TOKEN_NUM_VAR;
        (**sp).data.num_var = op1_num_var + op2;
        ++(*sp);
        break;
    }
    return 0;
}

int process_cmd(struct Token **tok, struct Token **sp) {
    short op1 = 0;
    short op2 = 0;
    short *op1_num_var = 0;
    short *op2_num_var = 0;
    switch ((*tok)->data.cmd.type) {
    case TOKEN_CMD_PRINT:
        if (*sp - stack < 1) {
            puts("Invalid number of operands");
            return 1;
        }
        --(*sp);
        switch ((*sp)->type) {
        case TOKEN_NUM:
            printf("%hd\n", (*sp)->data.num);
            break;
        case TOKEN_NUM_VAR:
            printf("%hd\n", *(*sp)->data.num_var);
            break;
        }
        break;
    case TOKEN_CMD_IF:
        if (*sp - stack < 1) {
            puts("_if no condition");
            return 1;
        }
        --(*sp);
        switch ((*sp)->type) {
        case TOKEN_NUM:
            if (!(*sp)->data.num) {
                *tok = code + (*tok)->data.cmd.data;
            }
            break;
        case TOKEN_NUM_VAR:
            if (!*(*sp)->data.num_var) {
                *tok = code + (*tok)->data.cmd.data;
            }
            break;
        }
        break;
    case TOKEN_CMD_ELSE:
        *tok = code + (*tok)->data.cmd.data;
        break;
    case TOKEN_CMD_END:
        if ((**tok).data.cmd.data) {
            *tok = code + (*tok)->data.cmd.data;
        }
        break;
    case TOKEN_CMD_WHILE:
        break;
    case TOKEN_CMD_DO:
        if (*sp - stack < 1) {
            puts("_do no condition");
            return 1;
        }
        --(*sp);
        switch ((*sp)->type) {
        case TOKEN_NUM:
            if (!(*sp)->data.num) {
                *tok = code + (*tok)->data.cmd.data;
            }
            break;
        case TOKEN_NUM_VAR:
            if (!*(*sp)->data.num_var) {
                *tok = code + (*tok)->data.cmd.data;
            }
            break;
        }
        break;
    case TOKEN_CMD_ALLOC:
        if (*sp - stack < 2) {
            puts("_alloc insufficient number of operands");
            return 1;
        }
        *sp -= 2;
        switch ((*sp)->type) {
        case TOKEN_ARR_VAR:
            if((*sp)->data.arr_var->head) {
                puts("Double _alloc");
                return 1;
            }
            (*sp)->data.arr_var->head = (short*)mp;
            break;
        default:
            puts("_alloc first operand not arr");
            return 1;
        }
        short offset = 0;
        switch ((*sp)[1].type) {
        case TOKEN_NUM:
            offset = (*sp)[1].data.num;
            break;
        case TOKEN_NUM_VAR:
            offset = *(*sp)[1].data.num_var;
            break;
        default:
            puts("_alloc second operand not numeric");
            return 1;
        }
        if (mp + (offset << 1) - memory > 320000) {
            puts("Out of memory");
            return 1;
        }
        (*sp)->data.arr_var->offset = offset;
        break;
    default:
        puts("Invalid cmd");
        return 1;
    }
    return 0;
}

int vm_run(char verbose) {
    struct Token *sp = stack;
    for (struct Token *i = code; i->type; ++i) {
        int ret = 0;

        switch (i->type) {
        case TOKEN_NUM:
            if (verbose) {
                printf("num %d\n", i->data.num);
            }
            *(sp++) = *i;
            break;
        case TOKEN_NUM_VAR:
            if (verbose) {
                printf("num var offset %d val %d \n",
                       i->data.num_var - nv,
                       *i->data.num_var);
            }
            *(sp++) = *i;
            break;
        case TOKEN_ARR_VAR:
            if (verbose) {
                printf("num var offset %d\n",
                       i->data.arr_var - av);
            }
            *(sp++) = *i;
            break;
        case TOKEN_OP:
            if (verbose) {
                printf("op %d\n", i->data.op);
            }
            ret = process_op(i, &sp);
            break;
        case TOKEN_CMD:
            if (verbose) {
                printf("cmd %d\n", i->data.cmd.type);
            }
            ret = process_cmd(&i, &sp);
            break;
        }
        if (ret) {
            return 1;
        }
    }
    return 0;
}
