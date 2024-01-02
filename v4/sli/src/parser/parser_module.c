#include <sliv4/parser/parser.h>

SLErr SLModule_initialize(SLModule *module) {
    if (!module) {
        return SL_ERR_NULL_PTR;
    }
    int ret =
        DArrayChar_initialize(&module->symbol_table.symbol_buf, 20);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        DArraySLSymbolTableValue_initialize(&module->symbol_table.table, 10);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    return SL_ERR_OK;
}

SLErr SLModule_finalize(SLModule *module) {
    if (!module) {
        return SL_ERR_NULL_PTR;
    }
    DArrayChar_finalize(&module->symbol_table.symbol_buf);
    DArraySLSymbolTableValue_finalize(&module->symbol_table.table);
    return SL_ERR_OK;
}

SLErr SLModule_log(SLModule *module, FILE *fout) {
    if (!module || !fout) {
        return SL_ERR_NULL_PTR;
    }
    fputs("Symbol Table:\n", fout);
    char *iter_buf = module->symbol_table.symbol_buf.data;
    SLSymbolTableValue *symbol = module->symbol_table.table.data;
    for (
        size_t i = 0;
        i < module->symbol_table.table.size;
        ++i,
        ++iter_buf,
        ++symbol) {
        fprintf(fout, "Identifier: %s\n", iter_buf);
        switch (symbol->type) {
        case SL_VALUE_TYPE_INT64:
            fputs("Type: int64\n", fout);
            break;
        default:
            break;
        }
        switch (symbol->location) {
        case SL_VALUE_VARIABLE_LOCATION_GLOBAL:
            fputs("Location: global\n", fout);
            break;
        default:
            break;
        }
        fprintf(fout, "Offset: %lu\n", symbol->offset);
        for (; *iter_buf; ++iter_buf);
    }
    return SL_ERR_OK;
}

SLErr SLModule_find_identifier(
    SLModule *module, char *token, SLSymbolTableValue *symbol) {
    if (!module || !token || !symbol) {
        return SL_ERR_NULL_PTR;
    }
    puts("find identifier");
    char *iter_buf = module->symbol_table.symbol_buf.data;
    SLSymbolTableValue *iter_symbol = module->symbol_table.table.data;
    for (
        size_t i = 0;
        i < module->symbol_table.table.size;
        ++i,
        ++iter_buf,
        ++iter_symbol) {
        puts(iter_buf);
        if (!strcmp(iter_buf, token)) {
            *symbol = *iter_symbol;
            return SL_ERR_OK;
        } else {
            for (; *iter_buf; ++iter_buf);
        }
    }
    return SL_ERR_IDENTIFIER_NOT_FOUND;
}
