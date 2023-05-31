#include "core.h"

SLErrCode SLInterpreter_initialize(SLInterpreter *interpreter) {
    if (!interpreter) {
        return SL_ERR_NULL_PTR;
    }
    interpreter->initialized = false;
    return SLParser_initialize(&interpreter->parser);
}

SLErrCode SLInterpreter_parse(SLInterpreter *interpreter, char *str) {
    if (!interpreter || !str) {
        return SL_ERR_NULL_PTR;
    }
    SLErrCode err = SLParser_parse(&interpreter->parser, str);
    if (err) {
        return err;
    }
    // int ret = DArrayChar_initialize(interpreter->global, interpreter->parser.global_loo)
    interpreter->initialized = true;
    return SL_ERR_OK;
}

void SLInterpreter_finalize(SLInterpreter *interpreter) {
    SLParser_finalize(&interpreter->parser);
}
