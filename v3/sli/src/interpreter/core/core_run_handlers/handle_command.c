#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_command_print(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_println(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_printhex(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_printe(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_if(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_else(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_end_if(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_while(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_do_while(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_end_while(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_break(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_continue(SLInterpreter *interpreter);

typedef SLErrCode (*Handler)(SLInterpreter*);

SLErrCode runtime_handle_command(SLInterpreter *interpreter) {
    Handler handlers[27] = {
        runtime_handle_command_print,
        runtime_handle_command_println,
        runtime_handle_command_printhex,
        runtime_handle_command_printe,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        runtime_handle_command_if,
        runtime_handle_command_else,
        runtime_handle_command_end_if,
        runtime_handle_command_while,
        runtime_handle_command_do_while,
        runtime_handle_command_end_while,
        NULL,
        NULL,
        NULL,
        NULL,
        runtime_handle_command_break,
        runtime_handle_command_continue,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL
    };
    SLErrCode err =
        handlers[interpreter->current->data.command.type](interpreter);
    if (err) {
        return err;
    }
    return SL_ERR_OK;
}
