#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

SLErrCode runtime_handle_operator_add(SLInterpreter *interpreter);

typedef SLErrCode (*Handler)(SLInterpreter*);

SLErrCode runtime_handle_operator(SLInterpreter *interpreter) {
    Handler handlers[35] = {
        runtime_handle_operator_add,
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
    return SL_ERR_OK;
}
