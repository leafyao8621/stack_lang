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
    ret =
        DArrayChar_initialize(&module->code.str, 20);
    if (ret) {
        return SL_ERR_OUT_OF_MEMORY;
    }
    ret =
        DArraySLInstruction_initialize(&module->code.code, 10);
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
    DArrayChar_finalize(&module->code.str);
    DArraySLInstruction_finalize(&module->code.code);
    return SL_ERR_OK;
}

SLErr SLModule_log(SLModule *module, FILE *fout) {
    if (!module || !fout) {
        return SL_ERR_NULL_PTR;
    }
    printf("sz %lu\n", module->symbol_table.table.size);
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
    fputs("Code:\n", fout);
    SLInstruction *inst = module->code.code.data;
    for (size_t i = 0; i < module->code.code.size; ++i, ++inst) {
        switch (inst->oprator) {
        case SL_INSTRUCTION_OPERATOR_ASSIGN:
            fputs("Instruction: =\n", fout);
            fputs("OP1:\n", fout);
            switch (inst->operand.binary.operand1.data.identifier.type) {
            case SL_VALUE_TYPE_INT64:
                fputs("Type: int64\n", fout);
                break;
            default:
                break;
            }
            switch (
                inst
                    ->operand
                    .binary
                    .operand1
                    .data
                    .identifier
                    .data
                    .variable
                    .location) {
            case SL_VALUE_VARIABLE_LOCATION_GLOBAL:
                fputs("Location: global\n", fout);
                break;
            default:
                break;
            }
            fprintf(
                fout,
                "Offset: %lu\n",
                inst
                    ->operand
                    .binary
                    .operand1
                    .data
                    .identifier
                    .data
                    .variable
                    .offset
            );
            fputs("OP2:\n", fout);
            if (!inst->operand.binary.operand2.is_literal) {
                switch (inst->operand.binary.operand2.data.identifier.type) {
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    break;
                default:
                    break;
                }
                switch (
                    inst
                        ->operand
                        .binary
                        .operand2
                        .data
                        .identifier
                        .data
                        .variable
                        .location) {
                case SL_VALUE_VARIABLE_LOCATION_GLOBAL:
                    fputs("Location: global\n", fout);
                    break;
                default:
                    break;
                }
                fprintf(
                    fout,
                    "Offset: %lu\n",
                    inst
                        ->operand
                        .binary
                        .operand2
                        .data
                        .identifier
                        .data
                        .variable
                        .offset
                );
            } else {
                switch (inst->operand.binary.operand2.data.literal.type) {
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    #if INTPTR_MAX == INT64_MAX
                    fprintf(
                        fout,
                        "Value: %ld 0x%016lx\n",
                        inst->operand.binary.operand2.data.literal.data.int64,
                        (uint64_t)
                            inst
                                ->operand
                                .binary
                                .operand2
                                .data
                                .literal
                                .data
                                .int64
                    );
                    #else
                    fprintf(
                        fout,
                        "Value: %lld 0x%016llx\n",
                        inst->operand.binary.operand2.data.literal.data.int64,
                        (uint64_t)
                            inst
                                ->operand
                                .binary
                                .operand2
                                .data
                                .literal
                                .data
                                .int64
                    );
                    #endif
                    break;
                case SL_VALUE_TYPE_UINT64:
                    fputs("Type: uint64\n", fout);
                    #if INTPTR_MAX == INT64_MAX
                    fprintf(
                        fout,
                        "Value: %ld 0x%016lx\n",
                        inst->operand.binary.operand2.data.literal.data.uint64,
                        inst
                            ->operand
                            .binary
                            .operand2
                            .data
                            .literal
                            .data
                            .uint64
                    );
                    #else
                    fprintf(
                        fout,
                        "Value: %lld 0x%016llx\n",
                        inst->operand.binary.operand2.data.literal.data.uint64,
                        inst
                            ->operand
                            .binary
                            .operand2
                            .data
                            .literal
                            .data
                            .int64
                    );
                    #endif
                    break;
                default:
                    break;
                }
            }
            break;
        default:
            break;
        }
    }
    return SL_ERR_OK;
}

SLErr SLModule_find_identifier(
    SLModule *module, char *token, SLSymbolTableValue *symbol) {
    if (!module || !token || !symbol) {
        return SL_ERR_NULL_PTR;
    }
    char *iter_buf = module->symbol_table.symbol_buf.data;
    SLSymbolTableValue *iter_symbol = module->symbol_table.table.data;
    for (
        size_t i = 0;
        i < module->symbol_table.table.size;
        ++i,
        ++iter_buf,
        ++iter_symbol) {
        if (!strcmp(iter_buf, token)) {
            *symbol = *iter_symbol;
            return SL_ERR_OK;
        } else {
            for (; *iter_buf; ++iter_buf);
        }
    }
    return SL_ERR_IDENTIFIER_NOT_FOUND;
}
