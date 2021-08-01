#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "parser.h"
#include "../vm/vm.h"

static char buf[320000];

static int process_num(FILE *fin, struct Token **cur, int *n_tok) {
    char *iter = buf + 1;
    int len = 1;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= '0' && in <= '9';
         ++len, ++iter, in = fgetc(fin)) {
        printf("character %d\n", in);
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
    printf("num %s\n", buf);
    return 0;
}

static int process_num_var(FILE *fin, struct Token **cur, int *n_tok) {
    char *iter = buf;
    int len = 0;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 &&
         in >= 'a' && in <= 'z';
         ++len, ++iter, in = fgetc(fin)) {
        
        *iter = in;
    }
    if (len > 2) {
        iter[1] = 0;
        printf("%s variable name too long\n", buf);
        return 1;
    }
    if (++(*n_tok) > 320000) {
        puts("Too many tokens");
        return 1;
    }
    (*cur)->type = TOKEN_NUM_VAR;
    (*cur)->data.num_var = nv + (buf[0] - 'a') * 26 + (buf[1] - 'a');
    ++(*cur);
    printf("num var %s\n", buf);
    return 0;
}

static int process_op(FILE *fin, struct Token **cur, int *n_tok) {
    char *iter = buf + 1;
    int len = 1;
    char cont = 1;
    for (int in = fgetc(fin);
         !feof(fin) && len < 320000 && cont;
         ++len, ++iter, in = fgetc(fin)) {
        printf("character %d\n", in);
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
    } else if (!strcmp(buf, "=")) {
        (*cur)->data.op = TOKEN_OP_ASSIGN;
    } else if (!strcmp(buf, "==")) {
        (*cur)->data.op = TOKEN_OP_EQUAL;
    } else {
        printf("%s is not a valid operator\n", buf);
        return 1;
    }
    ++(*cur);
    printf("op %s\n", buf);
    return 0;
}

static int process_cmd(FILE *fin, struct Token **cur, int *n_tok) {
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
    (*cur)->type = TOKEN_OP;
    if (!strcmp(buf, "print")) {
        (*cur)->data.op = TOKEN_OP_ADD;
    } else {
        printf("%s is not a valid command\n", buf);
        return 1;
    }
    ++(*cur);
    printf("cmd %s\n", buf);
    return 0;
}

int parser_parse(const char *fn) {
    FILE *fin = fopen(fn, "r");
    int n_tok = 0;
    struct Token *iter = code;
    for (int in = fgetc(fin); !feof(fin); in = fgetc(fin)) {
        int ret = 0;
        printf("main %d\n", in);
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
            ret = process_num(fin, &iter, &n_tok);
            break;
        case '#':
            ret = process_num_var(fin, &iter, &n_tok);
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
            *buf = in;
            ret = process_op(fin, &iter, &n_tok);
            break;
        case '!':
            ret = process_cmd(fin, &iter, &n_tok);
            break;
        }
        if (ret) {
            fclose(fin);
            return 1;
        }
    }
    fclose(fin);
    return 0;
}