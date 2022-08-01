#include "parser.h"

DEF_DRRAY_FUNCTIONS(Token)
DEF_DRRAY_FUNCTIONS(Character)
DEF_HASHSET_FUNCTIONS(String)
DEF_HASHMAP_FUNCTIONS(String, String)
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
        HashMapStringString_initialize(
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
    ret = HashMapStringString_finalize(&parser->str_name);
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
    DArrayCharacter_push(&parser->str_buf, "a");
    DArrayCharacter_push(&parser->str_buf, "b");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "d");
    DArrayCharacter_push(&parser->str_buf, "");
    DArrayCharacter_push(&parser->str_buf, "a");
    DArrayCharacter_push(&parser->str_buf, "b");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "");
    DArrayCharacter_push(&parser->str_buf, "a");
    DArrayCharacter_push(&parser->str_buf, "b");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "d");
    DArrayCharacter_push(&parser->str_buf, "");
    DArrayCharacter_push(&parser->str_buf, "b");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "d");
    DArrayCharacter_push(&parser->str_buf, "e");
    DArrayCharacter_push(&parser->str_buf, "");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "d");
    DArrayCharacter_push(&parser->str_buf, "e");
    DArrayCharacter_push(&parser->str_buf, "f");
    DArrayCharacter_push(&parser->str_buf, "");
    DArrayCharacter_push(&parser->str_buf, "b");
    DArrayCharacter_push(&parser->str_buf, "c");
    DArrayCharacter_push(&parser->str_buf, "d");
    DArrayCharacter_push(&parser->str_buf, "");
    char *str = parser->str_buf.data;
    char **ptr = NULL;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "alpha";
    str += 5;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "bravo";
    str += 4;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "charlie";
    str += 5;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "delta";
    str += 5;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "echo";
    str += 5;
    HashSetString_insert(&parser->int_name, &str);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    *ptr = "foxtrot";
    for (size_t i = 0; i < parser->int_name.capacity; ++i) {
        printf("in_use: %lu\nitem: %s\n", parser->int_name.data[i].in_use, parser->int_name.data[i].item);
    }
    for (size_t i = 0; i < parser->str_name.capacity; ++i) {
        printf("in_use: %lu\nkey: %s\nvalue: %s\n", parser->str_name.data[i].in_use, parser->str_name.data[i].key, parser->str_name.data[i].value);
    }
    bool out = false;
    str = "cdef";
    HashSetString_check(&parser->int_name, &str, &out);
    printf("cdef: %d\n", out);
    HashMapStringString_check(&parser->str_name, &str, &out);
    HashMapStringString_fetch(&parser->str_name, &str, &ptr);
    printf("cdef: %d %s\n", out, *ptr);
    str = "abcde";
    HashSetString_check(&parser->int_name, &str, &out);
    printf("abcde: %d\n", out);
    HashMapStringString_check(&parser->str_name, &str, &out);
    printf("cdef: %d\n", out);
    return 0;
}
