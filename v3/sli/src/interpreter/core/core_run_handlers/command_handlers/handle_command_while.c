#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../../core.h"

SLErrCode runtime_handle_command_while(SLInterpreter *interpreter) {
    if (!interpreter) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
