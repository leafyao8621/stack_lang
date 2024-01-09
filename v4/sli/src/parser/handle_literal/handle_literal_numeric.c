#include <sliv4/parser/parser.h>

typedef enum SLParserNumericType {
    SL_PARSER_NUMERIC_TYPE_INTEGER,
    SL_PARSER_NUMERIC_TYPE_INTEGER_HEX,
    SL_PARSER_NUMERIC_TYPE_FLOAT
} SLParserNumericType;

SLErr SLParser_parse_module_text_handle_literal_numeric(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    switch (parser->state) {
    case SL_PARSER_STATE_GLOBAL:
        break;
    default:
        return SL_ERR_RUNAWAY_VARIABLE_DEF;
    }
    SLParserNumericType type = SL_PARSER_NUMERIC_TYPE_INTEGER;
    bool dot = false;
    bool e = false;
    bool x = false;
    char *iter = parser->buf.data;
    for (size_t i = 0; i < parser->buf.size - 1; ++i, ++iter) {
        switch (*iter) {
        case '0':
        case '1':
        case '2':
        case '3':
        case '4':
        case '5':
        case '6':
        case '7':
        case '8':
        case '9':
            break;
        case '-':
            if (!(!i || (i && (iter[-1] != 'E' && iter[-1] != 'e'))) || x) {
                return SL_ERR_INVALID_LITERAL_NUMERIC;
            }
            break;
        case '.':
            if (dot || e || x) {
                return SL_ERR_INVALID_LITERAL_NUMERIC;
            }
            dot = true;
            type = SL_PARSER_NUMERIC_TYPE_FLOAT;
            break;
        case 'E':
        case 'e':
            if (x && (!i || e)) {
                return SL_ERR_INVALID_LITERAL_NUMERIC;
            }
            if (!x) {
                e = true;
                type = SL_PARSER_NUMERIC_TYPE_FLOAT;
            }
            break;
        case 'x':
            if (i == 1 && iter[-1] == '0') {
                x = true;
                type = SL_PARSER_NUMERIC_TYPE_INTEGER_HEX;
            } else {
                return SL_ERR_INVALID_LITERAL_NUMERIC;
            }
            break;
        case 'A':
        case 'a':
        case 'B':
        case 'b':
        case 'C':
        case 'c':
        case 'D':
        case 'd':
        case 'F':
        case 'f':
            puts("hex digit");
            if (!x) {
                return SL_ERR_INVALID_LITERAL_NUMERIC;
            }
            break;
        default:
            return SL_ERR_INVALID_LITERAL_NUMERIC;
        }
    }
    SLValue value;
    value.is_literal = true;
    switch (type) {
    case SL_PARSER_NUMERIC_TYPE_INTEGER:
        value.data.literal.type = SL_VALUE_TYPE_INT64;
        #if INTPTR_MAX == INT64_MAX
            sscanf(parser->buf.data, "%ld", &value.data.literal.data.int64);
        #else
            sscanf(parser->buf.data, "%lld", &value.data.literal.int64);
        #endif
        break;
    case SL_PARSER_NUMERIC_TYPE_INTEGER_HEX:
        value.data.literal.type = SL_VALUE_TYPE_UINT64;
        #if INTPTR_MAX == INT64_MAX
            sscanf(parser->buf.data, "%lx", &value.data.literal.data.uint64);
        #else
            sscanf(parser->buf.data, "%llx", &value.data.literal.uint64);
        #endif
        break;
    case SL_PARSER_NUMERIC_TYPE_FLOAT:
        value.data.literal.type = SL_VALUE_TYPE_FLOAT64;
        sscanf(parser->buf.data, "%lf", &value.data.literal.data.float64);
        break;
    }
    int ret =
        DArraySLValue_push_back(&parser->value_stack, &value);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}
