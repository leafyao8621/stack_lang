#include <sliv4/parser/parser.h>

typedef enum SLParserKeyword {
    SL_PARSER_KEYWORD_CHAR,
    SL_PARSER_KEYWORD_INT8,
    SL_PARSER_KEYWORD_UINT8,
    SL_PARSER_KEYWORD_INT16,
    SL_PARSER_KEYWORD_UINT16,
    SL_PARSER_KEYWORD_INT32,
    SL_PARSER_KEYWORD_UINT32,
    SL_PARSER_KEYWORD_INT64,
    SL_PARSER_KEYWORD_UINT64,
    SL_PARSER_KEYWORD_INVALID
} SLParserKeyword;

SLErr SLParser_parse_module_text_handle_keyword_int64(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_keyword(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    SLParserKeyword kwd = 0;
    static const char *keywords[9] = {
        "char",
        "int8",
        "uint8",
        "int16",
        "uint16",
        "int32",
        "uint32",
        "int64",
        "uint64"
    };
    const char **iter = keywords;
    for (size_t i = 0; i < 9; ++i, ++kwd, ++iter) {
        if (!strcmp(parser->buf.data, *iter)) {
            break;
        }
    }
    SLErr reterr = SL_ERR_OK;
    switch (kwd) {
    case SL_PARSER_KEYWORD_INVALID:
        return SL_ERR_INVALID_KEYWORD;
    case SL_PARSER_KEYWORD_INT64:
        reterr = SLParser_parse_module_text_handle_keyword_int64(parser, module);
        if (reterr) {
            return reterr;
        }
        break;
    default:
        break;
    }
    return SL_ERR_OK;
}
