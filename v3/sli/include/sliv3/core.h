#ifndef INTERPRETER_CORE_CORE_H_
#define INTERPRETER_CORE_CORE_H_

#include <sliv3/parser.h>

typedef struct SLInterpreter {
    bool initialized;
    SLParser parser;
    String global;
    SLToken *current;
} SLInterpreter;

SLErrCode SLInterpreter_initialize(SLInterpreter *interpreter);
SLErrCode SLInterpreter_parse(SLInterpreter *interpreter, char *str);
SLErrCode SLInterpreter_run(SLInterpreter *interpreter);
void SLInterpreter_finalize(SLInterpreter *intepreter);

#endif
