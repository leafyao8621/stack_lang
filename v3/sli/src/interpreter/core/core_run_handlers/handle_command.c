#include <stdio.h>

#include <containers/eq.h>
#include <containers/hash.h>

#include "../core.h"

#include "command_handlers/handle_command_print.h"
#include "command_handlers/handle_command_println.h"
#include "command_handlers/handle_command_alloc.h"
#include "command_handlers/handle_command_printhex.h"
#include "command_handlers/handle_command_printe.h"
#include "command_handlers/handle_command_if.h"
#include "command_handlers/handle_command_else.h"
#include "command_handlers/handle_command_do_while.h"
#include "command_handlers/handle_command_end_while.h"
#include "command_handlers/handle_command_do_for.h"
#include "command_handlers/handle_command_end_for.h"
#include "command_handlers/handle_command_break.h"
#include "command_handlers/handle_command_continue.h"

SLErrCode runtime_handle_command(SLInterpreter *interpreter) {
    SLErrCode err = SL_ERR_OK;
    switch (interpreter->current->data.command.type) {
    case SL_COMMAND_TYPE_PRINT:
        err = runtime_handle_command_print(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_PRINTLN:
        err = runtime_handle_command_println(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_PRINTHEX:
        err = runtime_handle_command_printhex(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_PRINTE:
        err = runtime_handle_command_printe(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_ALLOC:
        err = runtime_handle_command_alloc(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_IF:
        err = runtime_handle_command_if(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_ELSE:
        err = runtime_handle_command_else(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_DO_WHILE:
        err = runtime_handle_command_do_while(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_END_WHILE:
        err = runtime_handle_command_end_while(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_DO_FOR:
        err = runtime_handle_command_do_for(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_END_FOR:
        err = runtime_handle_command_end_for(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_BREAK:
        err = runtime_handle_command_break(interpreter);
        if (err) {
            return err;
        }
        break;
    case SL_COMMAND_TYPE_CONTINUE:
        err = runtime_handle_command_continue(interpreter);
        if (err) {
            return err;
        }
        break;
    default:
        break;
    }
    if (err) {
        return err;
    }
    return SL_ERR_OK;
}
