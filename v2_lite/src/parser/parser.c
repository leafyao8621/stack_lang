#include <stdlib.h>

#include "parser.h"

DEF_STACK_FUNCTIONS(Token, 10)
DEF_STACK_FUNCTIONS(Token, 2000)
DEF_HASHSET_FUNCTIONS(String, 10)
DEF_STACK_FUNCTIONS(Idx, 10)
DEF_STACK_FUNCTIONS(Character, 5000)
DEF_HASHSET_FUNCTIONS(String, 200)
DEF_HASHMAP_FUNCTIONS(String, Size, 10)
DEF_HASHMAP_FUNCTIONS(String, Function, 10)
DEF_HASHMAP_FUNCTIONS(String, Idx, 14)

int parser_initialize(Parser *parser, String fn) {
    if (!parser || !fn) {
        return ERR_NULL_PTR;
    }
    parser->idx_if = 0;
    parser->idx_else = 0;
    parser->idx_while = 0;
    parser->idx_do = 0;
    parser->idx_end_if = 0;
    parser->idx_end_loop = 0;

    StackToken2000_initialize(&parser->tokens);
    parser->cur_token_buf = &parser->tokens;

    parser->function_definition = 0;
    parser->cur_function = 0;

    StackCharacter5000_initialize(&parser->str_buf);
    parser->fin = fopen(fn, "r");
    if (!parser->fin) {
        return ERR_FILE_IO;
    }
    return ERR_OK;
}

int parser_finalize(Parser *parser) {
    if (!parser) {
        return ERR_NULL_PTR;
    }
    if (parser->fin) {
        fclose(parser->fin);
    }
    return ERR_OK;
}

int parser_log(Parser *parser, FILE *fout) {
    size_t i;
    Token *iter;
    if (!parser || !fout) {
        return ERR_NULL_PTR;
    }
    fputs("tokens:\n", fout);
    for (
        i = 0, iter = parser->tokens.data;
        i < parser->tokens.size;
        ++i, ++iter) {
        fprintf(fout, "position: %lu\n", i);
        switch (iter->type) {
        case TOKEN_INT_LIT:
            fprintf(
                fout,
                "INT_LIT\n%d\n",
                iter->data.int_lit
            );
            break;
        }
    }
    return ERR_OK;
}

int handle_int_lit(Parser *parser) {
    int in, ret, val;
    Character chr;
    Token token;
    for (
        in = fgetc(parser->fin);
        !feof(parser->fin) &&
        in != ' ' &&
        in != '\t' &&
        in != '\n';
        in = fgetc(parser->fin)) {
        chr = in;
        if (chr < '0' || chr > '9') {
            return ERR_INVALID_TOKEN;
        }
        ret = StackCharacter5000_push(&parser->str_buf, &chr);
        if (ret) {
            return ret;
        }
    }
    chr = 0;
    ret = StackCharacter5000_push(&parser->str_buf, &chr);
    if (ret) {
        return ret;
    }
    val = atoi(parser->str_buf.data);
    StackCharacter5000_clear(&parser->str_buf);
    token.type = TOKEN_INT_LIT;
    token.data.int_lit = val;
    ret = StackToken2000_push(parser->cur_token_buf, &token);
    if (ret) {
        return ret;
    }
    return ERR_OK;
}

int parser_parse(Parser *parser) {
    int in, ret;
    Character chr;
    if (!parser) {
        return ERR_NULL_PTR;
    }
    for (
        in = fgetc(parser->fin);
        !feof(parser->fin);
        in = fgetc(parser->fin)) {
        chr = in;
        switch (chr) {
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
            ret = StackCharacter5000_push(&parser->str_buf, &chr);
            if (ret) {
                return ret;
            }
            ret = handle_int_lit(parser);
            if (ret) {
                return ret;
            }
            break;
        default:
            return ERR_INVALID_TOKEN;
        }
    }

    return ERR_OK;
}
