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
            1, hash_function_string,
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
            1, hash_function_string,
            eq_function_string
        );
    if (ret) {
        DArrayToken_finalize(&parser->tokens);
        DArrayToken_finalize(&parser->stack);
        DArrayCharacter_finalize(&parser->str_buf);
        HashSetString_finalize(&parser->int_name);
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
    fclose(parser->fin);
    return 0;
}

int parser_parse(Parser *parser) {
    if (!parser) {
        return ERR_NULL_PTR;
    }

    return 0;
}
