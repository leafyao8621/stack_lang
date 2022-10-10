#include <stdlib.h>

#include "parser.h"

DEF_STACK_FUNCTIONS(Token, 10)
DEF_STACK_FUNCTIONS(Token, 2000)
DEF_HASHSET_FUNCTIONS(String, 10)
DEF_STACK_FUNCTIONS(Idx, 10)
DEF_STACK_FUNCTIONS(Character, 500)
DEF_STACK_FUNCTIONS(Character, 5000)
DEF_HASHSET_FUNCTIONS(String, 200)
DEF_HASHMAP_FUNCTIONS(String, Size, 10)
DEF_HASHMAP_FUNCTIONS(String, Function, 10)
DEF_HASHMAP_FUNCTIONS(String, Idx, 14)

int parser_initialize(Parser *parser, String fn) {
    if (!parser || !fn) {
        return ERR_NULL_PTR;
    }
    parser->fin = fopen(fn, "r");
    if (!parser->fin) {
        return ERR_FILE_IO;
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

    StackCharacter500_initialize(&parser->token_buf);
    StackCharacter5000_initialize(&parser->str_buf);
    HashSetString200_initialize(
        &parser->str_lit,
        hash_function_string,
        eq_function_string
    );

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
    HashSetString200Token *iter_str_lit;
    Token *iter;
    if (!parser || !fout) {
        return ERR_NULL_PTR;
    }
    fputs("str_lit:\n", fout);
    for (
        i = 0, iter_str_lit = parser->str_lit.data;
        i < 200;
        ++i, ++iter_str_lit) {
        if (iter_str_lit->in_use) {
            fprintf(fout, "%s\n", iter_str_lit->item);
        }
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
        case TOKEN_STR_LIT:
            fprintf(
                fout,
                "STR_LIT\nidx: %lu\nstr: %s\n",
                iter->data.str_lit,
                parser->str_lit.data[iter->data.str_lit].item
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
        ret = StackCharacter500_push(&parser->token_buf, &chr);
        if (ret) {
            return ret;
        }
    }
    chr = 0;
    ret = StackCharacter500_push(&parser->token_buf, &chr);
    if (ret) {
        return ret;
    }
    val = atoi(parser->token_buf.data);
    StackCharacter500_clear(&parser->token_buf);
    token.type = TOKEN_INT_LIT;
    token.data.int_lit = val;
    ret = StackToken2000_push(parser->cur_token_buf, &token);
    if (ret) {
        return ret;
    }
    return ERR_OK;
}

int handle_str_lit(Parser *parser) {
    int in, ret;
    Character chr;
    Token token;
    unsigned char escaped;
    unsigned char found;
    Idx idx;
    escaped = 0;
    for (
        in = fgetc(parser->fin);
        !feof(parser->fin) &&
        in != '"';
        in = fgetc(parser->fin)) {
        chr = in;
        if (!escaped && chr == '\\') {
            escaped = 1;
            continue;
        }
        if (escaped) {
            escaped = 0;
            switch (chr) {
            case 't':
                chr = '\t';
                break;
            case 'n':
                chr = '\n';
                break;
            case '\\':
                chr = '\\';
                break;
            default:
                return ERR_INVALID_TOKEN;
            }
        }
        ret = StackCharacter500_push(&parser->token_buf, &chr);
        if (ret) {
            return ret;
        }
    }
    chr = 0;
    ret = StackCharacter500_push(&parser->token_buf, &chr);
    if (ret) {
        return ret;
    }
    puts(parser->token_buf.data);
    ret =
        HashSetString200_find(
            &parser->str_lit,
            parser->token_buf.data,
            &found,
            &idx
        );
    if (ret) {
        return ret;
    }
    if (!found) {
        strcpy(parser->str_buf.tail, parser->token_buf.data);

        parser->str_buf.size += parser->token_buf.size;
        ret =
            HashSetString200_insert(
                &parser->str_lit,
                parser->str_buf.tail
            );
        if (ret) {
            return ret;
        }
        ret =
            HashSetString200_find(
                &parser->str_lit,
                parser->str_buf.tail,
                &found,
                &idx
            );
        parser->str_buf.tail += parser->token_buf.size;
    }
    StackCharacter500_clear(&parser->token_buf);
    token.type = TOKEN_STR_LIT;
    token.data.str_lit = idx;
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
            ret = StackCharacter500_push(&parser->token_buf, &chr);
            if (ret) {
                return ret;
            }
            ret = handle_int_lit(parser);
            if (ret) {
                return ret;
            }
            break;
        case '"':
            ret = handle_str_lit(parser);
            if (ret) {
                return ret;
            }
            break;
        case ' ':
        case '\t':
        case '\n':
            break;
        default:
            return ERR_INVALID_TOKEN;
        }
    }

    return ERR_OK;
}
