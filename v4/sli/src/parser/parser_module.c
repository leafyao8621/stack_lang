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
        case SL_VALUE_TYPE_CHAR:
            fputs("Type: char\n", fout);
            break;
        case SL_VALUE_TYPE_INT8:
            fputs("Type: int8\n", fout);
            break;
        case SL_VALUE_TYPE_UINT8:
            fputs("Type: uint8\n", fout);
            break;
        case SL_VALUE_TYPE_INT16:
            fputs("Type: int16\n", fout);
            break;
        case SL_VALUE_TYPE_UINT16:
            fputs("Type: uint16\n", fout);
            break;
        case SL_VALUE_TYPE_INT32:
            fputs("Type: int32\n", fout);
            break;
        case SL_VALUE_TYPE_UINT32:
            fputs("Type: uint32\n", fout);
            break;
        case SL_VALUE_TYPE_INT64:
            fputs("Type: int64\n", fout);
            break;
        case SL_VALUE_TYPE_UINT64:
            fputs("Type: uint64\n", fout);
            break;
        case SL_VALUE_TYPE_FLOAT32:
            fputs("Type: float32\n", fout);
            break;
        case SL_VALUE_TYPE_FLOAT64:
            fputs("Type: float64\n", fout);
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
    static const char *operator_lookup[] =
        {
            "+",
            "=",
            "-",
            "*"
        };
    fputs("Code:\n", fout);
    SLInstruction *inst = module->code.code.data;
    for (size_t i = 0; i < module->code.code.size; ++i, ++inst) {
        switch (inst->operator) {
        case SL_INSTRUCTION_OPERATOR_ADD:
        case SL_INSTRUCTION_OPERATOR_ASSIGN:
        case SL_INSTRUCTION_OPERATOR_SUBTRACT:
        case SL_INSTRUCTION_OPERATOR_MULTIPLY:
            fprintf(fout, "Instruction: %s\n", operator_lookup[inst->operator]);
            fputs("OP1:\n", fout);
            if (!inst->operand.binary.operand1.is_literal) {
                switch (inst->operand.binary.operand1.data.identifier.type) {
                case SL_VALUE_TYPE_CHAR:
                    fputs("Type: char\n", fout);
                    break;
                case SL_VALUE_TYPE_INT8:
                    fputs("Type: int8\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT8:
                    fputs("Type: uint8\n", fout);
                    break;
                case SL_VALUE_TYPE_INT16:
                    fputs("Type: int16\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT16:
                    fputs("Type: uint16\n", fout);
                    break;
                case SL_VALUE_TYPE_INT32:
                    fputs("Type: int32\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT32:
                    fputs("Type: uint32\n", fout);
                    break;
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT64:
                    fputs("Type: uint64\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT32:
                    fputs("Type: float32\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT64:
                    fputs("Type: float64\n", fout);
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
                case SL_VALUE_VARIABLE_LOCATION_TEMP:
                    fputs("Location: temp\n", fout);
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
            } else {
                switch (inst->operand.binary.operand1.data.literal.type) {
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    #if INTPTR_MAX == INT64_MAX
                    fprintf(
                        fout,
                        "Value: %ld 0x%016lx\n",
                        inst->operand.binary.operand1.data.literal.data.int64,
                        (uint64_t)
                            inst
                                ->operand
                                .binary
                                .operand1
                                .data
                                .literal
                                .data
                                .int64
                    );
                    #else
                    fprintf(
                        fout,
                        "Value: %lld 0x%016llx\n",
                        inst->operand.binary.operand1.data.literal.data.int64,
                        (uint64_t)
                            inst
                                ->operand
                                .binary
                                .operand1
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
                        inst->operand.binary.operand1.data.literal.data.uint64,
                        inst
                            ->operand
                            .binary
                            .operand1
                            .data
                            .literal
                            .data
                            .uint64
                    );
                    #else
                    fprintf(
                        fout,
                        "Value: %lld 0x%016llx\n",
                        inst->operand.binary.operand1.data.literal.data.uint64,
                        inst
                            ->operand
                            .binary
                            .operand1
                            .data
                            .literal
                            .data
                            .int64
                    );
                    #endif
                    break;
                case SL_VALUE_TYPE_FLOAT64:
                    fputs("Type: float64\n", fout);
                    fprintf(
                        fout,
                        "Value: %lf\n",
                        inst->operand.binary.operand1.data.literal.data.float64
                    );
                    break;
                default:
                    break;
                }
            }
            fputs("OP2:\n", fout);
            if (!inst->operand.binary.operand2.is_literal) {
                switch (inst->operand.binary.operand2.data.identifier.type) {
                case SL_VALUE_TYPE_CHAR:
                    fputs("Type: char\n", fout);
                    break;
                case SL_VALUE_TYPE_INT8:
                    fputs("Type: int8\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT8:
                    fputs("Type: uint8\n", fout);
                    break;
                case SL_VALUE_TYPE_INT16:
                    fputs("Type: int16\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT16:
                    fputs("Type: uint16\n", fout);
                    break;
                case SL_VALUE_TYPE_INT32:
                    fputs("Type: int32\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT32:
                    fputs("Type: uint32\n", fout);
                    break;
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT64:
                    fputs("Type: uint64\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT32:
                    fputs("Type: float32\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT64:
                    fputs("Type: float64\n", fout);
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
                case SL_VALUE_VARIABLE_LOCATION_TEMP:
                    fputs("Location: temp\n", fout);
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
                case SL_VALUE_TYPE_FLOAT64:
                    fputs("Type: float64\n", fout);
                    fprintf(
                        fout,
                        "Value: %lf\n",
                        inst->operand.binary.operand2.data.literal.data.float64
                    );
                    break;
                default:
                    break;
                }
            }
            switch (inst->operator) {
            case SL_INSTRUCTION_OPERATOR_ADD:
            case SL_INSTRUCTION_OPERATOR_SUBTRACT:
            case SL_INSTRUCTION_OPERATOR_MULTIPLY:
                fputs("Result:\n", fout);
                switch (inst->res_type) {
                case SL_VALUE_TYPE_CHAR:
                    fputs("Type: char\n", fout);
                    break;
                case SL_VALUE_TYPE_INT8:
                    fputs("Type: int8\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT8:
                    fputs("Type: uint8\n", fout);
                    break;
                case SL_VALUE_TYPE_INT16:
                    fputs("Type: int16\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT16:
                    fputs("Type: uint16\n", fout);
                    break;
                case SL_VALUE_TYPE_INT32:
                    fputs("Type: int32\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT32:
                    fputs("Type: uint32\n", fout);
                    break;
                case SL_VALUE_TYPE_INT64:
                    fputs("Type: int64\n", fout);
                    break;
                case SL_VALUE_TYPE_UINT64:
                    fputs("Type: uint64\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT32:
                    fputs("Type: float32\n", fout);
                    break;
                case SL_VALUE_TYPE_FLOAT64:
                    fputs("Type: float64\n", fout);
                    break;
                default:
                    break;
                }
                fprintf(fout, "Offset: %lu\n", inst->res_offset);
                break;
            default:
                break;
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
