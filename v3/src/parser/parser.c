#include "parser.h"

SLErrCode parser_initialize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    return SL_ERR_OK;
}
