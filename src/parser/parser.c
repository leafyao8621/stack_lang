#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../vm/vm.h"

static char buf[320000];
static struct Token *stack[1000];

static int process_num(FILE *fin,
                       struct Token **cur,
                       int *n_tok,
                       char verbose) {
    char *iter = buf + 1;
    int len = 1;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= '0' && in <= '9';
         ++len, ++iter, in = fgetc(fin)) {
        *iter = in;
    }
    if (len == 320000) {
        puts("Number literal too long");
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    *iter = 0;
    (*cur)->type = TOKEN_NUM;
    (*cur)->data.num = atoi(buf);
    ++(*cur);
    if (verbose) {
        printf("num %s\n", buf);
    }
    return 0;
}

static int process_str(FILE *fin,
                       struct Token **cur,
                       int *n_tok,
                       char verbose) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 && in != '"';
         ++len, ++iter, in = fgetc(fin)) {
        *iter = in;
    }
    if (len == 320000) {
        puts("String too long");
        return 1;
    }
    ++len;
    if (mp + len - memory > 320000) {
        puts("Out of memory");
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    *iter = 0;
    (*cur)->type = TOKEN_STR;
    memcpy(mp, buf, len);
    (*cur)->data.str = mp;
    mp += len;
    ++(*cur);
    if (verbose) {
        printf("str %s\n", buf);
    }
    return 0;
}

static int process_num_var(FILE *fin,
                           struct Token **cur,
                           int *n_tok,
                           char verbose) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= 'a' && in <= 'z';
         ++len, ++iter, in = fgetc(fin)) {

        *iter = in;
    }
    *iter = 0;
    if (len > 2) {
        printf("%s variable name too long\n", buf);
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    (*cur)->type = TOKEN_NUM_VAR;
    (*cur)->data.num_var = nv + (buf[0] - 'a') +
                           (buf[1] ? (buf[1] + 1 - 'a') * 26 : 0);
    ++(*cur);
    if (verbose) {
        printf("num var %s\n", buf);
    }
    return 0;
}

static int process_arr_var(FILE *fin,
                           struct Token **cur,
                           int *n_tok,
                           char verbose) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= 'a' && in <= 'z';
         ++len, ++iter, in = fgetc(fin)) {

        *iter = in;
    }
    *iter = 0;
    if (len > 1) {
        printf("%s variable name too long\n", buf);
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    (*cur)->type = TOKEN_ARR_VAR;
    (*cur)->data.arr_var = av + (buf[0] - 'a');
    ++(*cur);
    if (verbose) {
        printf("arr var %s\n", buf);
    }
    return 0;
}

static int process_str_var(FILE *fin,
                           struct Token **cur,
                           int *n_tok,
                           char verbose) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= 'a' && in <= 'z';
         ++len, ++iter, in = fgetc(fin)) {

        *iter = in;
    }
    *iter = 0;
    if (len > 1) {
        printf("%s variable name too long\n", buf);
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    (*cur)->type = TOKEN_STR_VAR;
    (*cur)->data.str_var = sv + (buf[0] - 'a');
    ++(*cur);
    if (verbose) {
        printf("str var %s\n", buf);
    }
    return 0;
}

static int process_op(FILE *fin, struct Token **cur, int *n_tok, char verbose) {
    char *iter = buf + 1;
    int len = 1;
    char cont = 1;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 && cont;
         ++len, ++iter, in = fgetc(fin)) {
        cont = 0;
        switch (in) {
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '<':
        case '>':
        case '&':
        case '|':
        case '[':
        case ']':
        case '%':
        case '!':
        case '~':
            cont = 1;
            break;
        }
        if (cont) {
            *iter = in;
        } else {
            break;
        }
    }
    if (len > 2) {
        printf("%s operator too long\n", buf);
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    *iter = 0;
    (*cur)->type = TOKEN_OP;
    if (!strcmp(buf, "+")) {
        (*cur)->data.op = TOKEN_OP_ADD;
    } else if (!strcmp(buf, "-")) {
        (*cur)->data.op = TOKEN_OP_SUBTRACT;
    } else if (!strcmp(buf, "*")) {
        (*cur)->data.op = TOKEN_OP_MULTIPLY;
    } else if (!strcmp(buf, "/")) {
        (*cur)->data.op = TOKEN_OP_DIVIDE;
    } else if (!strcmp(buf, "%")) {
        (*cur)->data.op = TOKEN_OP_MOD;
    } else if (!strcmp(buf, "=")) {
        (*cur)->data.op = TOKEN_OP_ASSIGN;
    } else if (!strcmp(buf, "==")) {
        (*cur)->data.op = TOKEN_OP_EQUAL;
    } else if (!strcmp(buf, "!=")) {
        (*cur)->data.op = TOKEN_OP_UNEQUAL;
    } else if (!strcmp(buf, ">")) {
        (*cur)->data.op = TOKEN_OP_GREATER;
    } else if (!strcmp(buf, "<")) {
        (*cur)->data.op = TOKEN_OP_LESS;
    } else if (!strcmp(buf, ">=")) {
        (*cur)->data.op = TOKEN_OP_GEQ;
    } else if (!strcmp(buf, "<=")) {
        (*cur)->data.op = TOKEN_OP_LEQ;
    } else if (!strcmp(buf, "<<")) {
        (*cur)->data.op = TOKEN_OP_LSHIFT;
    } else if (!strcmp(buf, ">>")) {
        (*cur)->data.op = TOKEN_OP_RSHIFT;
    } else if (!strcmp(buf, "&")) {
        (*cur)->data.op = TOKEN_OP_BAND;
    } else if (!strcmp(buf, "&&")) {
        (*cur)->data.op = TOKEN_OP_LAND;
    } else if (!strcmp(buf, "|")) {
        (*cur)->data.op = TOKEN_OP_BOR;
    } else if (!strcmp(buf, "||")) {
        (*cur)->data.op = TOKEN_OP_LOR;
    } else if (!strcmp(buf, "~")) {
        (*cur)->data.op = TOKEN_OP_BNOT;
    } else if (!strcmp(buf, "!")) {
        (*cur)->data.op = TOKEN_OP_LNOT;
    } else if (!strcmp(buf, "[]")) {
        (*cur)->data.op = TOKEN_OP_IDX;
    } else {
        printf("%s is not a valid operator\n", buf);
        return 1;
    }
    ++(*cur);
    if (verbose) {
        printf("op %s\n", buf);
    }
    return 0;
}

static int process_cmd(FILE *fin,
                       struct Token **cur,
                       int *n_tok,
                       struct Token ***sp,
                       char verbose) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= 'a' && in <= 'z';
         ++len, ++iter, in = fgetc(fin)) {
        *iter = in;
    }
    if (len == 320000) {
        puts("Command too long");
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    *iter = 0;
    (*cur)->type = TOKEN_CMD;
    if (!strcmp(buf, "print")) {
        (*cur)->data.cmd.type = TOKEN_CMD_PRINT;
    } else if (!strcmp(buf, "println")) {
        (*cur)->data.cmd.type = TOKEN_CMD_PRINTLN;
    } else if (!strcmp(buf, "if")) {
        (*cur)->data.cmd.type = TOKEN_CMD_IF;
        **sp = *cur;
        ++(*sp);
    } else if (!strcmp(buf, "else")) {
        (*cur)->data.cmd.type = TOKEN_CMD_ELSE;
        if (*sp <= stack) {
            puts("_else unbalanced");
            return 1;
        }
        --(*sp);
        if ((***sp).data.cmd.type != TOKEN_CMD_IF) {
            puts("_else without _if");
            return 1;
        }
        (***sp).data.cmd.data = (*cur) - code;
        **sp = *cur;
        ++(*sp);
    } else if (!strcmp(buf, "end")) {
        (*cur)->data.cmd.type = TOKEN_CMD_END;
        if (*sp <= stack) {
            puts("_end unbalanced");
            return 1;
        }
        --(*sp);
        switch ((***sp).data.cmd.type) {
        case TOKEN_CMD_IF:
        case TOKEN_CMD_ELSE:
            (***sp).data.cmd.data = (*cur) - code;
            break;
        case TOKEN_CMD_DO:
            if (*sp <= stack) {
                puts("_end unbalanced, missing _while");
                return 1;
            }
            --(*sp);
            if ((***sp).data.cmd.type != TOKEN_CMD_WHILE) {
                puts("_end unbalanced, not _while");
                return 1;
            }
            (*cur)->data.cmd.data = (***sp).data.cmd.data;
            (*((*sp)[1])).data.cmd.data = (*cur) - code;
            break;
        default:
            puts("_end unbalanced");
            return 1;
        }
    } else if (!strcmp(buf, "while")) {
        (*cur)->data.cmd.type = TOKEN_CMD_WHILE;
        (*cur)->data.cmd.data = (*cur) - code;
        **sp = *cur;
        ++(*sp);
    } else if (!strcmp(buf, "do")) {
        (*cur)->data.cmd.type = TOKEN_CMD_DO;
        **sp = *cur;
        ++(*sp);
    } else if (!strcmp(buf, "alloc")) {
        (*cur)->data.cmd.type = TOKEN_CMD_ALLOC;
        **sp = *cur;
        ++(*sp);
    } else {
        printf("%s is not a valid command\n", buf);
        return 1;
    }
    ++(*cur);
    if (verbose) {
        printf("cmd %s\n", buf);
    }
    return 0;
}

int parser_parse(const char *fn, char verbose) {
    FILE *fin = fopen(fn, "r");
    int n_tok = 0;
    struct Token *iter = code;
    struct Token **sp = stack;
    mp = memory;
    for (int in = fgetc(fin); !feof(fin); in = fgetc(fin)) {
        int ret = 0;
        switch (in) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            *buf = in;
            ret = process_num(fin, &iter, &n_tok, verbose);
            break;
        case '"':
            ret = process_str(fin, &iter, &n_tok, verbose);
            break;
        case '#':
            ret = process_num_var(fin, &iter, &n_tok, verbose);
            break;
        case '@':
            ret = process_arr_var(fin, &iter, &n_tok, verbose);
            break;
        case '$':
            ret = process_str_var(fin, &iter, &n_tok, verbose);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '=':
        case '<':
        case '>':
        case '&':
        case '|':
        case '[':
        case ']':
        case '%':
        case '!':
        case '~':
            *buf = in;
            ret = process_op(fin, &iter, &n_tok, verbose);
            break;
        case '_':
            ret = process_cmd(fin, &iter, &n_tok, &sp, verbose);
            break;
        }
        if (ret) {
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);
    iter->type = TOKEN_HALT;
    return 0;
}
