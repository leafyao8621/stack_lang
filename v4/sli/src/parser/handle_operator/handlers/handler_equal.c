#include <sliv4/parser/parser.h>

SLErr SLParser_parse_module_text_handle_operator_equal(
    SLParser *parser, SLModule *module) {
    if (!parser || !module) {
        return SL_ERR_NULL_PTR;
    }
    int ret = 0;
    SLValue op1;
    SLValue op2;
    SLInstruction instruction;
    if (!strcmp(parser->buf.data, "=")) {
        ret = DArraySLValue_pop_back(&parser->value_stack);
        if (ret) {
            return SL_ERR_INVALID_NUM_OPERAND;
        }
        ret = DArraySLValue_pop_back(&parser->value_stack);
        if (ret) {
            return SL_ERR_INVALID_NUM_OPERAND;
        }
        op1 = parser->value_stack.data[parser->value_stack.size];
        op2 = parser->value_stack.data[parser->value_stack.size + 1];
        if (
            op1.is_literal ||
            op1.data.identifier.data.variable.location ==
            SL_VALUE_VARIABLE_LOCATION_TEMP) {
            return SL_ERR_INVALID_OPERAND_TYPE;
        }
        switch (op1.data.identifier.type) {
        case SL_VALUE_TYPE_UINT8:
        case SL_VALUE_TYPE_INT8:
        case SL_VALUE_TYPE_UINT16:
        case SL_VALUE_TYPE_INT16:
        case SL_VALUE_TYPE_UINT32:
        case SL_VALUE_TYPE_INT32:
        case SL_VALUE_TYPE_UINT64:
        case SL_VALUE_TYPE_INT64:
        case SL_VALUE_TYPE_FLOAT32:
        case SL_VALUE_TYPE_FLOAT64:
            if (op2.is_literal) {
                switch (op2.data.literal.type) {
                case SL_VALUE_TYPE_UINT64:
                case SL_VALUE_TYPE_INT64:
                case SL_VALUE_TYPE_FLOAT64:
                    break;
                default:
                    return SL_ERR_INVALID_OPERAND_TYPE;
                }
            } else {
                if (
                    op2.data.identifier.data.variable.location ==
                    SL_VALUE_VARIABLE_LOCATION_TEMP) {
                    parser->temp_offset -= 8;
                }
                switch (op2.data.identifier.type) {
                case SL_VALUE_TYPE_UINT8:
                case SL_VALUE_TYPE_INT8:
                case SL_VALUE_TYPE_UINT16:
                case SL_VALUE_TYPE_INT16:
                case SL_VALUE_TYPE_UINT32:
                case SL_VALUE_TYPE_INT32:
                case SL_VALUE_TYPE_UINT64:
                case SL_VALUE_TYPE_INT64:
                case SL_VALUE_TYPE_FLOAT32:
                case SL_VALUE_TYPE_FLOAT64:
                    break;
                default:
                    return SL_ERR_INVALID_OPERAND_TYPE;
                }
            }
            break;
        default:
            return SL_ERR_INVALID_OPERAND_TYPE;
        }
        instruction.operator = SL_INSTRUCTION_OPERATOR_ASSIGN;
        instruction.operand.binary.operand1 = op1;
        instruction.operand.binary.operand2 = op2;
        ret =
            DArraySLInstruction_push_back(&module->code.code, &instruction);
        if (ret) {
            return SL_ERR_OUT_OF_MEMORY;
        }
    } else if (!strcmp(parser->buf.data, "==")) {

    } else {
        return SL_ERR_INVALID_OPERATOR;
    }
    return SL_ERR_OK;
}
