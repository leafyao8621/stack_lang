#ifndef CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_BREAK_H_
#define CORE_CORE_RUN_HANDLERS_COMMAND_HANDLERS_HANDLE_COMMAND_BREAK_H_

#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

extern inline SLErrCode runtime_handle_command_break(SLInterpreter *interpreter);

inline SLErrCode runtime_handle_command_break(SLInterpreter *interpreter) {
    interpreter->current =
        interpreter
            ->cur_token_buf
            ->data +
        interpreter
            ->current
            ->data
            .command
            .tgt;
    return SL_ERR_OK;
}

#endif
