#ifndef _VM_H_
#define _VM_H_

#define TOKEN_HALT 0

#define TOKEN_NUM 1

#define TOKEN_NUM_VAR 2

#define TOKEN_ARR_VAR 3

#define TOKEN_OP 4
#define TOKEN_OP_ADD 0
#define TOKEN_OP_SUBTRACT 1
#define TOKEN_OP_MULTIPLY 2
#define TOKEN_OP_DIVIDE 3
#define TOKEN_OP_MOD 4
#define TOKEN_OP_ASSIGN 5
#define TOKEN_OP_EQUAL 6
#define TOKEN_OP_UNEQUAL 7
#define TOKEN_OP_LESS 8
#define TOKEN_OP_GREATER 9
#define TOKEN_OP_LEQ 10
#define TOKEN_OP_GEQ 11
#define TOKEN_OP_LSHIFT 12
#define TOKEN_OP_RSHIFT 13
#define TOKEN_OP_BAND 14
#define TOKEN_OP_LAND 15
#define TOKEN_OP_BOR 16
#define TOKEN_OP_LOR 17
#define TOKEN_OP_BNOT 18
#define TOKEN_OP_LNOT 19
#define TOKEN_OP_IDX 20

#define TOKEN_CMD 5
#define TOKEN_CMD_PRINT 0
#define TOKEN_CMD_IF 1
#define TOKEN_CMD_ELSE 2
#define TOKEN_CMD_END 3
#define TOKEN_CMD_WHILE 4
#define TOKEN_CMD_DO 5
#define TOKEN_CMD_ALLOC 6

extern short nv[702];
extern char *sv[26];
extern char memory[320000];
extern char *mp;

struct Array {
    short *head;
    short offset;
};

extern struct Array av[26];

struct Token {
    char type;
    union {
        short num;
        short *num_var;
        char *str;
        char **str_var;
        struct Array *arr_var;
        char op;
        struct {
            char type;
            short data;
        } cmd;
    } data;
};

extern struct Token code[320000];

int vm_run(char verbose);

#endif
