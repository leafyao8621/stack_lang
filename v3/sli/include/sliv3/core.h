#ifndef INTERPRETER_CORE_CORE_H_
#define INTERPRETER_CORE_CORE_H_

#include <sliv3/parser.h>

typedef SLToken *SLTokenPtr;

DEF_DARRAY(SLTokenPtr)

typedef char **BufferPtr;

typedef struct ArrayMeta {
    bool dynamic;
    DArrayIdx size;
} ArrayMeta;

DEF_HASHMAP(BufferPtr, ArrayMeta)

typedef struct SLInterpreter {
    bool initialized;
    bool bound_check;
    SLParser parser;
    String global;
    HashMapBufferPtrArrayMeta global_array;
    SLToken *current;
    DArraySLToken *cur_token_buf;
    DArraySLToken operation_stack;
    DArraySLTokenPtr control_stack;
    DArrayIdx size_stack;
} SLInterpreter;

SLErrCode SLInterpreter_initialize(SLInterpreter *interpreter);
SLErrCode SLInterpreter_parse(SLInterpreter *interpreter, char *str);
SLErrCode SLInterpreter_run(SLInterpreter *interpreter, bool bound_check);
void SLInterpreter_finalize(SLInterpreter *intepreter);

#endif
