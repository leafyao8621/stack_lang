#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_end_for(SLInterpreter *interpreter) {
    if (
        !(
            interpreter->control_stack.size &&
            (
                interpreter
                    ->control_stack
                    .data[interpreter->control_stack.size - 1] ==
                interpreter
                    ->cur_token_buf
                    ->data +
                interpreter
                    ->current
                    ->data
                    .command
                    .tgt
            )
        )) {
        int ret =
            DArraySLTokenPtr_push_back(
                &interpreter->control_stack,
                &interpreter->current
            );
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    }
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
