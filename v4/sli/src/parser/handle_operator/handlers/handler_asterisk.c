#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_literal_numeric(
    SLParser *parser, SLModule *module);

SLErr SLParser_parse_module_text_handle_operator_asterisk(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLInstruction instruction;
    SLValue op1;
    SLValue op2;
    SLValue value;
    if (!strcmp(parser->buf.data, "*")) {
        ret = DArraySLValue_pop_back(&parser->value_stack);
        if (ret) {
            return SL_ERR_INVALID_NUM_OPERAND;
        }
        ret = DArraySLValue_pop_back(&parser->value_stack);
        if (ret) {
            return SL_ERR_INVALID_NUM_OPERAND;
        }
        bool literal_flag = false;
        size_t offset_addition = 0;
        op1 = parser->value_stack.data[parser->value_stack.size];
        op2 = parser->value_stack.data[parser->value_stack.size + 1];
        if (!op1.is_literal) {
            switch (op1.data.identifier.type) {
            case SL_VALUE_TYPE_UINT8:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 1;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_UINT8:
                        instruction.res_type = SL_VALUE_TYPE_UINT8;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT8;
                        offset_addition = 1;
                        break;
                    case SL_VALUE_TYPE_INT8:
                        instruction.res_type = SL_VALUE_TYPE_INT8;
                        value.data.identifier.type = SL_VALUE_TYPE_INT8;
                        offset_addition = 1;
                        break;
                    case SL_VALUE_TYPE_UINT16:
                        instruction.res_type = SL_VALUE_TYPE_UINT16;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_INT16:
                        instruction.res_type = SL_VALUE_TYPE_INT16;
                        value.data.identifier.type = SL_VALUE_TYPE_INT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_UINT32;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_INT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_INT8:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 1;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT8:
                        instruction.res_type = SL_VALUE_TYPE_INT8;
                        value.data.identifier.type = SL_VALUE_TYPE_INT8;
                        offset_addition = 1;
                        break;
                    case SL_VALUE_TYPE_UINT16:
                        instruction.res_type = SL_VALUE_TYPE_UINT16;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_INT16:
                        instruction.res_type = SL_VALUE_TYPE_INT16;
                        value.data.identifier.type = SL_VALUE_TYPE_INT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_UINT32;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_INT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_UINT16:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 2;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_UINT16:
                        instruction.res_type = SL_VALUE_TYPE_UINT16;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_INT16:
                        instruction.res_type = SL_VALUE_TYPE_INT16;
                        value.data.identifier.type = SL_VALUE_TYPE_INT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_UINT32;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_INT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_INT16:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 2;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT16:
                        instruction.res_type = SL_VALUE_TYPE_INT16;
                        value.data.identifier.type = SL_VALUE_TYPE_INT16;
                        offset_addition = 2;
                        break;
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_UINT32;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_INT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_UINT32:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 4;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_UINT32;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_INT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_INT32:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 4;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                        instruction.res_type = SL_VALUE_TYPE_INT32;
                        value.data.identifier.type = SL_VALUE_TYPE_INT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_UINT64:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 8;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_INT64:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 8;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_FLOAT32:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 4;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_FLOAT64:
                if (
                    op1.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 8;
                }
                if (op2.is_literal) {
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_FLOAT32:
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            default:
                return SL_ERR_INVALID_OPERAND_TYPE;
            }
        } else {
            switch (op1.data.literal.type) {
            case SL_VALUE_TYPE_UINT64:
                if (op2.is_literal) {
                    literal_flag = true;
                    value.is_literal = true;
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                        value.data.literal.type = SL_VALUE_TYPE_UINT64;
                        value.data.literal.data.uint64 =
                            op1.data.literal.data.uint64 +
                            op2.data.literal.data.uint64;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        value.data.literal.type = SL_VALUE_TYPE_INT64;
                        value.data.literal.data.int64 =
                            op1.data.literal.data.uint64 +
                            op2.data.literal.data.int64;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        value.data.literal.type = SL_VALUE_TYPE_FLOAT64;
                        value.data.literal.data.float64 =
                            op1.data.literal.data.uint64 +
                            op2.data.literal.data.float64;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_UINT64;
                        value.data.identifier.type = SL_VALUE_TYPE_UINT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_INT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_INT64:
                if (op2.is_literal) {
                    literal_flag = true;
                    value.is_literal = true;
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_INT64:
                        value.data.literal.type = SL_VALUE_TYPE_INT64;
                        value.data.literal.data.int64 =
                            op1.data.literal.data.int64 +
                            op2.data.literal.data.int64;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        value.data.literal.type = SL_VALUE_TYPE_FLOAT64;
                        value.data.literal.data.float64 =
                            op1.data.literal.data.int64 +
                            op2.data.literal.data.float64;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                        instruction.res_type = SL_VALUE_TYPE_INT64;
                        value.data.identifier.type = SL_VALUE_TYPE_INT64;
                        offset_addition = 8;
                        break;
                    case SL_VALUE_TYPE_FLOAT32:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT32;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT32;
                        offset_addition = 4;
                        break;
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            case SL_VALUE_TYPE_FLOAT64:
                if (op2.is_literal) {
                    literal_flag = true;
                    value.is_literal = true;
                    switch (op2.data.literal.type) {
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_FLOAT64:
                        value.data.literal.type = SL_VALUE_TYPE_FLOAT64;
                        value.data.literal.data.float64 =
                            op1.data.literal.data.int64 +
                            op2.data.literal.data.float64;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                } else {
                    if (
                        op2.data.identifier.data.variable.location ==
                        SL_VALUE_VARIABLE_LOCATION_TEMP) {
                        switch (op2.data.identifier.type) {
                        case SL_VALUE_TYPE_INT8:
                        case SL_VALUE_TYPE_UINT8:
                            parser->temp_offset -= 1;
                            break;
                        case SL_VALUE_TYPE_INT16:
                        case SL_VALUE_TYPE_UINT16:
                            parser->temp_offset -= 2;
                            break;
                        case SL_VALUE_TYPE_INT32:
                        case SL_VALUE_TYPE_UINT32:
                        case SL_VALUE_TYPE_FLOAT32:
                            parser->temp_offset -= 4;
                            break;
                        case SL_VALUE_TYPE_UINT64:
                        case SL_VALUE_TYPE_INT64:
                        case SL_VALUE_TYPE_FLOAT64:
                            parser->temp_offset -= 8;
                            break;
                        default:
                            return SL_ERR_INVALID_OPERAND_TYPE;
                        }
                    }
                    switch (op2.data.identifier.type) {
                    case SL_VALUE_TYPE_INT8:
                    case SL_VALUE_TYPE_UINT8:
                    case SL_VALUE_TYPE_INT16:
                    case SL_VALUE_TYPE_UINT16:
                    case SL_VALUE_TYPE_INT32:
                    case SL_VALUE_TYPE_UINT32:
                    case SL_VALUE_TYPE_INT64:
                    case SL_VALUE_TYPE_UINT64:
                    case SL_VALUE_TYPE_FLOAT32:
                    case SL_VALUE_TYPE_FLOAT64:
                        instruction.res_type = SL_VALUE_TYPE_FLOAT64;
                        value.data.identifier.type = SL_VALUE_TYPE_FLOAT64;
                        offset_addition = 8;
                        break;
                    default:
                        return SL_ERR_INVALID_OPERAND_TYPE;
                    }
                }
                break;
            default:
                return SL_ERR_INVALID_OPERAND_TYPE;
            }
        }
        if (!literal_flag) {
            value.is_literal = false;
            value.data.identifier.data.variable.location =
                SL_VALUE_VARIABLE_LOCATION_TEMP;
            instruction.operator = SL_INSTRUCTION_OPERATOR_MULTIPLY;
            instruction.operand.binary.operand1 = op1;
            instruction.operand.binary.operand2 = op2;
            instruction.res_offset = parser->temp_offset;
            ret =
                DArraySLInstruction_push_back(&module->code.code, &instruction);
            if (ret) {
                return SL_ERR_OUT_OF_MEMORY;
            }
            value.data.identifier.data.variable.offset =
                parser->temp_offset;
            parser->temp_offset += offset_addition;
            if (parser->temp_offset_max < parser->temp_offset) {
                parser->temp_offset_max = parser->temp_offset;
            }
        }
        ret =
            DArraySLValue_push_back(&parser->value_stack, &value);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    } else {
        return SL_ERR_INVALID_OPERATOR;
    }
    return SL_ERR_OK;
}
