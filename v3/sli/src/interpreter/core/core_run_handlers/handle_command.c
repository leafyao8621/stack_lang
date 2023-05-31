#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_command_print(SLInterpreter *interpreter);
SLErrCode runtime_handle_command_println(SLInterpreter *interpreter);

typedef SLErrCode (*Handler)(SLInterpreter*);

SLErrCode runtime_handle_command(SLInterpreter *interpreter) {
    Handler handlers[20] = {
        runtime_handle_command_print,
        runtime_handle_command_println,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
        NULL,
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
    ++interpreter->current;
    return SL_ERR_OK;
}
