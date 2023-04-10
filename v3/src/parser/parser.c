#include "parser.h"

DEF_DARRAY_FUNCTIONS(SLToken)

DEF_HASHMAP_FUNCTIONS(String, Idx)

SLErrCode parser_initialize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
