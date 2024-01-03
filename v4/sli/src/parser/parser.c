#include <stdio.h>

#include <sliv4/parser/parser.h>

SLErr SLParser_initialize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    parser->state = SL_PARSER_STATE_GLOBAL;
    parser->global_offset = 0;
    parser->temp_offset = 0;
    parser->temp_offset_max = 0;
    int ret = DArrayChar_initialize(&parser->buf, 20);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret = DArraySLValue_initialize(&parser->value_stack, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

SLErr SLParser_finalize(SLParser *parser) {
    if (!parser) {
        return SL_ERR_NULL_PTR;
    }
    DArrayChar_finalize(&parser->buf);
    DArraySLValue_finalize(&parser->value_stack);
    return SL_ERR_OK;
}

SLErr SLParser_collect_token(char **iter, SLParser *parser) {
    if (!iter || !parser) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    for (
        ;
        **iter && **iter != ' ' && **iter != '\n' && **iter != '\t';
        ++(*iter)) {
        if (
            (
                **iter >= 'A' &&
                **iter <= 'Z'
            ) ||
            (
                **iter >= 'a' &&
                **iter <= 'z'
            ) ||
            (
                **iter >= '0' &&
                **iter <= '9'
            ) ||
            **iter == '_' ||
            **iter == '.' ||
            **iter == '-' ||
            **iter == '+' ||
            **iter == '-' ||
            **iter == '*' ||
            **iter == '/' ||
            **iter == '&' ||
            **iter == '|' ||
            **iter == '^' ||
            **iter == '>' ||
            **iter == '<' ||
            **iter == '=' ||
            **iter == '[' ||
            **iter == ']') {
            ret = DArrayChar_push_back(&parser->buf, *iter);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
        } else {
            return SL_ERR_INVALID_TOKEN;
        }
    }
    char chr = 0;
    ret = DArrayChar_push_back(&parser->buf, &chr);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    printf("token: %s\n", parser->buf.data);
    return SL_ERR_OK;
}

SLErr SLParser_parse_module_text_handle_keyword(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_identifier(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_literal_numeric(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_literal_string(
    char **iter, SLModule *module);

SLErr SLParser_parse_module_text_handle_operator(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text(
    String *code, SLParser *parser, SLModule *module) {
    if (!code || !parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    SLErr reterr = SL_ERR_OK;
    for (char *code_iter = code->data; *code_iter; ++code_iter) {
        if (
            (
                *code_iter >= 'A' &&
                *code_iter <= 'Z'
            ) ||
            (
                *code_iter >= 'a' &&
                *code_iter <= 'z'
            )) {
            reterr = SLParser_collect_token(&code_iter, parser);
            if (reterr) {
                return reterr;
            }
            reterr = SLParser_parse_module_text_handle_keyword(parser, module);
            if (reterr) {
                if (reterr == SL_ERR_INVALID_KEYWORD) {
                    reterr =
                        SLParser_parse_module_text_handle_identifier(
                            parser, module);
                } else {
                    return reterr;
                }
            }
            DArrayChar_clear(&parser->buf);
        } else if (
            *code_iter >= '0' &&
            *code_iter <= '9') {
            reterr = SLParser_collect_token(&code_iter, parser);
            if (reterr) {
                return reterr;
            }
            reterr =
                SLParser_parse_module_text_handle_literal_numeric(
                    parser,
                    module
                );
            if (reterr) {
                return reterr;
            }
            DArrayChar_clear(&parser->buf);
        } else if (*code_iter == '"') {
            reterr =
                SLParser_parse_module_text_handle_literal_string(
                    &code_iter,
                    module
                );
            if (reterr) {
                return reterr;
            }
        } else {
            reterr = SLParser_collect_token(&code_iter, parser);
            if (reterr) {
                return reterr;
            }
            switch (*parser->buf.data) {
            case '+':
            case '-':
            case '*':
            case '/':
            case '&':
            case '|':
            case '^':
            case '>':
            case '<':
            case '=':
            case '[':
                reterr =
                    SLParser_parse_module_text_handle_operator(
                        parser,
                        module
                    );
                if (reterr) {
                    return reterr;
                }
                break;
            }
            DArrayChar_clear(&parser->buf);
        }
    }
    return SL_ERR_OK;
}


