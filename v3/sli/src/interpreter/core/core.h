#ifndef INTERPRETER_CORE_CORE_H_
#define INTERPRETER_CORE_CORE_H_

#include <sliv3/parser.h>

typedef SLToken *SLTokenPtr;

DEF_DARRAY(SLTokenPtr)

typedef char **BufferPtr;

DEF_HASHMAP(BufferPtr, DArrayIdx)

typedef struct SLInterpreter {
    bool initialized;
    SLParser parser;
    String global;
    HashMapBufferPtrDArrayIdx global_array;
    SLToken *current;
    DArraySLToken *cur_token_buf;
    DArraySLToken operation_stack;
    DArraySLTokenPtr control_stack;
} SLInterpreter;

SLErrCode SLInterpreter_initialize(SLInterpreter *interpreter);
SLErrCode SLInterpreter_parse(SLInterpreter *interpreter, char *str);
SLErrCode SLInterpreter_run(SLInterpreter *interpreter);
void SLInterpreter_finalize(SLInterpreter *intepreter);

#endif
