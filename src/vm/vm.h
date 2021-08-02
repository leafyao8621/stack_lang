#ifndef _VM_H_
#define _VM_H_

#define TOKEN_HALT 0

#define TOKEN_NUM 1

#define TOKEN_NUM_VAR 2

#define TOKEN_OP 3
#define TOKEN_OP_ADD 0
#define TOKEN_OP_SUBTRACT 1
#define TOKEN_OP_MULTIPLY 2
#define TOKEN_OP_DIVIDE 3
#define TOKEN_OP_ASSIGN 4
#define TOKEN_OP_EQUAL 5
#define TOKEN_OP_UNEQUAL 6
#define TOKEN_OP_LESS 7
#define TOKEN_OP_GREATER 8
#define TOKEN_OP_LEQ 9
#define TOKEN_OP_GEQ 10
#define TOKEN_OP_LSHIFT 11
#define TOKEN_OP_RSHIFT 12
#define TOKEN_OP_BAND 13
#define TOKEN_OP_LAND 14
#define TOKEN_OP_BOR 15
#define TOKEN_OP_LOR 16

#define TOKEN_CMD 4
#define TOKEN_CMD_PRINT 0
#define TOKEN_CMD_IF 1
#define TOKEN_CMD_ELSE 2
#define TOKEN_CMD_END 3
#define TOKEN_CMD_WHILE 4
#define TOKEN_CMD_DO 5

extern short nv[702];
extern char memory[320000];

struct Token {
    char type;
    union {
        short num;
        short *num_var;
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
