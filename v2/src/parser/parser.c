#include "parser.h"

DEF_DRRAY_FUNCTIONS(Token)
DEF_DRRAY_FUNCTIONS(Character)
DEF_HASHSET_FUNCTIONS(String)
DEF_HASHMAP_FUNCTIONS(String, Size)

int parser_initialize(Parser *parser, String ifn) {
    if (!parser || !ifn) {
        return ERR_NULL_PTR;
    }
    parser->fin = fopen(ifn, "r");
    if (!parser->fin) {
        return ERR_FILE_IO;
    }
    int ret = DArrayToken_initialize(&parser->tokens, 1000);
    if (ret) {
        return ret;
    }
    ret = DArrayToken_initialize(&parser->stack, 5);
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        return ret;
    }
    ret = DArrayCharacter_initialize(&parser->str_buf, 1000);
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        DArrayToken_finalize(&parser->stack);
        return ret;
    }
    ret =
        HashSetString_initialize(
            &parser->int_name,
            100, hash_function_string,
            eq_function_string
        );
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        DArrayToken_finalize(&parser->stack);
        DArrayCharacter_finalize(&parser->str_buf);
        return ret;
    }
    ret =
        HashSetString_initialize(
            &parser->str_name,
            100, hash_function_string,
            eq_function_string
        );
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        DArrayToken_finalize(&parser->stack);
        DArrayCharacter_finalize(&parser->str_buf);
        HashSetString_finalize(&parser->int_name);
        return ret;
    }
    ret =
        HashMapStringSize_initialize(
            &parser->arr_name,
            100, hash_function_string,
            eq_function_string
        );
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        DArrayToken_finalize(&parser->stack);
        DArrayCharacter_finalize(&parser->str_buf);
        HashSetString_finalize(&parser->int_name);
        HashSetString_finalize(&parser->str_name);
        return ret;
    }
    return 0;
}

int parser_finalize(Parser *parser) {
    if (!parser) {
        return ERR_NULL_PTR;
    }
    int ret = DArrayToken_finalize(&parser->tokens);
    if (ret) {
        return ret;
    }
    ret = DArrayToken_finalize(&parser->stack);
    if (ret) {
        return ret;
    }
    ret = DArrayCharacter_finalize(&parser->str_buf);
    if (ret) {
        return ret;
    }
    ret = HashSetString_finalize(&parser->int_name);
    if (ret) {
        return ret;
    }
    ret = HashSetString_finalize(&parser->str_name);
    if (ret) {
        return ret;
    }
    ret = HashMapStringSize_finalize(&parser->arr_name);
    fclose(parser->fin);
    return 0;
}

static int handle_int_name(Parser *parser) {
    size_t i = 0;
    int ret = 0;
    for (
        int ii = fgetc(parser->fin);
        !feof(parser->fin) &&
        ii != ' ' &&
        ii != '\t' &&
        ii != '\n';
        ++i,
        ii = fgetc(parser->fin)
    ) {
        if (!i) {
            if (
                (ii >= 'A' && ii <= 'Z') ||
                (ii >= 'a' && ii <= 'z')
            ) {
                char cur = (char)ii;
                ret = DArrayCharacter_push(&parser->str_buf, &cur);
                if (ret) {
                    return ret;
                }
            } else {
                return ERR_INVALID_VAR_NAME;
            }
        } else {
            if (
                (ii >= 'A' && ii <= 'Z') ||
                (ii >= 'a' && ii <= 'z') ||
                (ii >= '0' && ii <= '9') ||
                ii == '_'
            ) {
                char cur = (char)ii;
                ret = DArrayCharacter_push(&parser->str_buf, &cur);
                if (ret) {
                    return ret;
                }
            } else {
                return ERR_INVALID_VAR_NAME;
            }
        }
    }
    ret = DArrayCharacter_push(&parser->str_buf, "");
    if (ret) {
        return ret;
    }
    String str = parser->str_buf.data + parser->str_buf.size - i - 1;
    Token token;
    token.type = TOKEN_INT_NAME;
    token.data.int_name = str;
    ret = DArrayToken_push(&parser->tokens, &token);
    if (ret) {
        return ret;
    }
    ret = HashSetString_insert(&parser->int_name, &str);
    if (ret) {
        return ret;
    }
    return 0;
}

static int handle_str_name(Parser *parser) {
    size_t i = 0;
    int ret = 0;
    for (
        int ii = fgetc(parser->fin);
        !feof(parser->fin) &&
        ii != ' ' &&
        ii != '\t' &&
        ii != '\n';
        ++i,
        ii = fgetc(parser->fin)
    ) {
        if (!i) {
            if (
                (ii >= 'A' && ii <= 'Z') ||
                (ii >= 'a' && ii <= 'z')
            ) {
                char cur = (char)ii;
                ret = DArrayCharacter_push(&parser->str_buf, &cur);
                if (ret) {
                    return ret;
                }
            } else {
                return ERR_INVALID_VAR_NAME;
            }
        } else {
            if (
                (ii >= 'A' && ii <= 'Z') ||
                (ii >= 'a' && ii <= 'z') ||
                (ii >= '0' && ii <= '9') ||
                ii == '_'
            ) {
                char cur = (char)ii;
                ret = DArrayCharacter_push(&parser->str_buf, &cur);
                if (ret) {
                    return ret;
                }
            } else {
                return ERR_INVALID_VAR_NAME;
            }
        }
    }
    ret = DArrayCharacter_push(&parser->str_buf, "");
    if (ret) {
        return ret;
    }
    String str = parser->str_buf.data + parser->str_buf.size - i - 1;
    Token token;
    token.type = TOKEN_STR_NAME;
    token.data.str_name = str;
    ret = DArrayToken_push(&parser->tokens, &token);
    if (ret) {
        return ret;
    }
    ret = HashSetString_insert(&parser->str_name, &str);
    if (ret) {
        return ret;
    }
    return 0;
}

static int handle_int_lit(Parser *parser, bool negative) {
    printf("cur: %c\n", parser->str_buf.data[parser->str_buf.size - 1]);
    size_t i = negative ? 2 : 1;
    int ret = 0;
    for (
        int ii = fgetc(parser->fin);
        !feof(parser->fin) &&
        ii != ' ' &&
        ii != '\t' &&
        ii != '\n';
        ++i,
        ii = fgetc(parser->fin)
    ) {
        if (ii >= '0' && ii <= '9') {
            char cur = (char)ii;
            ret = DArrayCharacter_push(&parser->str_buf, &cur);
            if (ret) {
                return ret;
            }
        } else {
            return ERR_INVALID_VAR_NAME;
        }
    }
    ret = DArrayCharacter_push(&parser->str_buf, "");
    if (ret) {
        return ret;
    }
    String str = parser->str_buf.data + parser->str_buf.size - i - 1;
    puts(str);
    Token token;
    token.type = TOKEN_INT_LIT;
    token.data.int_lit = atol(str);
    ret = DArrayToken_push(&parser->tokens, &token);
    if (ret) {
        return ret;
    }
    return 0;
}

static int handle_str_lit(Parser *parser) {
    size_t i = 0;
    int ii = 0;
    int ret = 0;
    char cur = 0;
    for (
        ii = fgetc(parser->fin);
        !feof(parser->fin) &&
        ii != '"';
        ++i,
        ii = fgetc(parser->fin)
    ) {
        cur = (char)ii;
        if (cur == '\\') {
            ii = fgetc(parser->fin);
            cur = (char)ii;
            switch (cur) {
            case 'n':
                cur = '\n';
                break;
            case 't':
                cur = '\t';
                break;
            case '"':
                cur = '"';
                break;
            case '\\':
                cur = '\\';
                break;
            default:
                puts("fail");
                return ERR_INVALID_STR_LIT;
            }
        }
        ret = DArrayCharacter_push(&parser->str_buf, &cur);
        if (ret) {
            return ret;
        }
    }
    if (ii != '"') {
        return ERR_INVALID_STR_LIT;
    }
    ret = DArrayCharacter_push(&parser->str_buf, "");
    if (ret) {
        return ret;
    }
    String str = parser->str_buf.data + parser->str_buf.size - i - 1;
    Token token;
    token.type = TOKEN_STR_LIT;
    token.data.str_lit = str;
    ret = DArrayToken_push(&parser->tokens, &token);
    if (ret) {
        return ret;
    }
    return 0;
}

static int handle_operator(Parser *parser) {
    char last = parser->str_buf.data[parser->str_buf.size - 1];
    Token token;
    token.type = TOKEN_OPERATOR;
    int in = fgetc(parser->fin);
    char cur = (char)in;
    int ret = 0;
    switch (last) {
    case '+':
        if (cur != ' ' && cur != '\t' && cur != '\n') {
            return ERR_INVALID_OPERATOR;
        }
        token.data.operator = TOKEN_OPERATOR_ADD;
        break;
    case '-':
        if (cur >= '0' && cur <= '9') {
            ret = DArrayCharacter_push(&parser->str_buf, &cur);
            if (ret) {
                return ret;
            }
            return handle_int_lit(parser, true);
        }
        if (cur != ' ' && cur != '\t' && cur != '\n') {
            return ERR_INVALID_OPERATOR;
        }
        token.data.operator = TOKEN_OPERATOR_SUBTRACT;
        break;
    case '*':
        if (cur != ' ' && cur != '\t' && cur != '\n') {
            return ERR_INVALID_OPERATOR;
        }
        token.data.operator = TOKEN_OPERATOR_MULTIPLY;
        break;
    case '/':
        if (cur != ' ' && cur != '\t' && cur != '\n') {
            return ERR_INVALID_OPERATOR;
        }
        token.data.operator = TOKEN_OPERATOR_DIVIDE;
        break;
    case '=':
        switch (cur) {
        case '=':
            token.data.operator = TOKEN_OPERATOR_EQ;
            break;
        case ' ':
        case '\t':
        case '\n':
            token.data.operator = TOKEN_OPERATOR_ASSIGN;
            break;
        default:
            return ERR_INVALID_OPERATOR;
        }
        break;
    }
    ret = DArrayToken_push(&parser->tokens, &token);
    if (ret) {
        return ret;
    }
    return 0;
}

int parser_parse(Parser *parser) {
    if (!parser) {
        return ERR_NULL_PTR;
    }
    int ret = 0;
    char cur = 0;
    for (
        int i = fgetc(parser->fin);
        !feof(parser->fin);
        i = fgetc(parser->fin)
    ) {
        switch (i) {
        case '#':
            ret = handle_int_name(parser);
            break;
        case '$':
            ret = handle_str_name(parser);
            break;
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
            cur = (char)i;
            ret = DArrayCharacter_push(&parser->str_buf, &cur);
            if (ret) {
                return ret;
            }
            ret = handle_int_lit(parser, false);
            break;
        case '"':
            ret = handle_str_lit(parser);
            break;
        case '+':
        case '-':
        case '*':
        case '/':
        case '%':
        case '=':
        case '>':
        case '<':
        case '!':
        case '~':
        case '&':
        case '|':
        case '[':
            cur = (char)i;
            ret = DArrayCharacter_push(&parser->str_buf, &cur);
            if (ret) {
                return ret;
            }
            ret = handle_operator(parser);
            break;
        }
        if (ret) {
            return ret;
        }
    }
    return 0;
}

int parser_log(Parser *parser, FILE *fout) {
    if (!parser || !fout) {
        return ERR_NULL_PTR;
    }
    fputs("int_name:\n", fout);
    for (size_t i = 0; i < parser->int_name.capacity; ++i) {
        if (parser->int_name.data[i].in_use) {
            fprintf(fout, "%s\n", parser->int_name.data[i].item);
        }
    }
    fputs("str_name:\n", fout);
    for (size_t i = 0; i < parser->str_name.capacity; ++i) {
        if (parser->str_name.data[i].in_use) {
            fprintf(fout, "%s\n", parser->str_name.data[i].item);
        }
    }
    fputs("tokens:\n", fout);
    for (size_t i = 0; i < parser->tokens.size; ++i) {
        fprintf(fout, "idx: %lu\n", i);
        switch (parser->tokens.data[i].type) {
        case TOKEN_INT_NAME:
            fprintf(
                fout,
                "INT_NAME\n%s\n",
                parser->tokens.data[i].data.int_name
            );
            break;
        case TOKEN_STR_NAME:
            fprintf(
                fout,
                "STR_NAME\n%s\n",
                parser->tokens.data[i].data.str_name
            );
            break;
        case TOKEN_INT_LIT:
            fprintf(
                fout,
                "INT_LIT\n%ld\n",
                parser->tokens.data[i].data.int_lit
            );
            break;
        case TOKEN_STR_LIT:
            fprintf(
                fout,
                "STR_LIT\n%s\n",
                parser->tokens.data[i].data.str_lit
            );
            break;
        case TOKEN_OPERATOR:
            fprintf(
                fout,
                "OPERATOR\n%hhu\n",
                parser->tokens.data[i].data.operator
            );
            break;
        }
    }
    return 0;
}
