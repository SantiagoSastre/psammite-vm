#pragma once

#include <math.h>

#include "core.h"
#include "opcodes.h"

typedef enum
{
    BANKER_ROUNDING,
    MATH_ROUNDING,
    TRUNCATION,
    CEIL,
    FLOOR
} RoundingModes;

static inline PsammiteStatusCodes psammite_fmath_execute(PsammiteVM *vm, uint8_t func7, uint8_t rs1, uint8_t rs2, uint8_t rd)
{
    switch (func7)
    {
    case IMF64:
    {
        PsammiteFloat value = {.bits = psammite_read_register(vm, rs1)};
        psammite_write_f_register(vm, rd, value);
        return VM_OK;
    }
    case F64MI:
    {
        PsammiteFloat value = psammite_read_f_register(vm, rs1);
        psammite_write_register(vm, rd, value.bits);
        return VM_OK;
    }
    case ICF64:
    {
        int is_signed = rs2 & 0x1;
        PsammiteFloat value;
        if (is_signed == 0)
        {
            value.f64 = (double)psammite_read_register(vm, rs1);
        }
        else
        {
            value.f64 = (double)(int64_t)psammite_read_register(vm, rs1);
        }
        psammite_write_f_register(vm, rd, value);
        return VM_OK;
    }
    case F64CI:
    {
        PsammiteFloat value = psammite_read_f_register(vm, rs1);
        int is_signed = rs2 & 0x1;
        RoundingModes round_mode = (RoundingModes)((rs2 >> 1) & 0x7);
        switch (round_mode)
        {
        case BANKER_ROUNDING:
            value.f64 = rint(value.f64);
            break;
        case MATH_ROUNDING:
            value.f64 = round(value.f64);
            break;
        case CEIL:
            value.f64 = ceil(value.f64);
            break;
        case FLOOR:
            value.f64 = floor(value.f64);
            break;
        case TRUNCATION:
            value.f64 = trunc(value.f64);
            break;
        default:
            return VM_ERROR_UNRECOGNIZED;
        }
        if (is_signed)
        {
            if (isnan(value.f64))
            {
                psammite_write_register(vm, rd, 0);
            }
            else if (value.f64 >= (double)INT64_MAX)
            {
                psammite_write_register(vm, rd, (uint64_t)INT64_MAX);
            }
            else if (value.f64 <= (double)INT64_MIN)
            {
                psammite_write_register(vm, rd, (uint64_t)INT64_MIN);
            }
            else
            {
                psammite_write_register(vm, rd, (uint64_t)(int64_t)value.f64);
            }
        }
        else
        {
            if (isnan(value.f64))
            {
                psammite_write_register(vm, rd, 0);
            }
            else if (value.f64 >= (double)UINT64_MAX)
            {
                psammite_write_register(vm, rd, UINT64_MAX);
            }
            else
            {
                if (value.f64 < 0)
                {
                    psammite_write_register(vm, rd, 0);
                }
                else
                {
                    psammite_write_register(vm, rd, (uint64_t)value.f64);
                }
            }
        }
        return VM_OK;
    }
    case FADD64:
    {
        PsammiteFloat result = {.f64 = psammite_read_f_register(vm, rs1).f64 + psammite_read_f_register(vm, rs2).f64};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FSUB64:
    {
        PsammiteFloat result = {.f64 = psammite_read_f_register(vm, rs1).f64 - psammite_read_f_register(vm, rs2).f64};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FMUL64:
    {
        PsammiteFloat result = {.f64 = psammite_read_f_register(vm, rs1).f64 * psammite_read_f_register(vm, rs2).f64};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FDIV64:
    {
        PsammiteFloat result = {.f64 = psammite_read_f_register(vm, rs1).f64 / psammite_read_f_register(vm, rs2).f64};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FSQRT64:
    {
        PsammiteFloat result = {.f64 = sqrt(psammite_read_f_register(vm, rs1).f64)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FSI64:
    {
        PsammiteFloat result = {.f64 = copysign(psammite_read_f_register(vm, rs1).f64, psammite_read_f_register(vm, rs2).f64)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FSIN64:
    {
        PsammiteFloat result = {.f64 = copysign(psammite_read_f_register(vm, rs1).f64, -psammite_read_f_register(vm, rs2).f64)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FSIX64:
    {
        PsammiteFloat val1 = {.f64 = psammite_read_f_register(vm, rs1).f64};
        PsammiteFloat val2 = {.f64 = psammite_read_f_register(vm, rs2).f64};
        double final_sign = signbit(val1.f64) != signbit(val2.f64) ? -1.0 : 1.0;

        PsammiteFloat result = {.f64 = copysign(val1.f64, final_sign)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FMAX64:
    {
        PsammiteFloat result = {.f64 = fmax(psammite_read_f_register(vm, rs1).f64, psammite_read_f_register(vm, rs2).f64)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FMIN64:
    {
        PsammiteFloat result = {.f64 = fmin(psammite_read_f_register(vm, rs1).f64, psammite_read_f_register(vm, rs2).f64)};
        psammite_write_f_register(vm, rd, result);
        return VM_OK;
    }
    case FEQ64:
    {
        PsammiteFloat val1 = psammite_read_f_register(vm, rs1);
        PsammiteFloat val2 = psammite_read_f_register(vm, rs2);
        int result = val1.f64 == val2.f64 ? 1 : 0;
        psammite_write_register(vm, rd, result);
        return VM_OK;
    }
    case FLT64:
    {
        PsammiteFloat val1 = psammite_read_f_register(vm, rs1);
        PsammiteFloat val2 = psammite_read_f_register(vm, rs2);
        int result = val1.f64 < val2.f64 ? 1 : 0;
        psammite_write_register(vm, rd, result);
        return VM_OK;
    }
    case FLE64:
    {
        PsammiteFloat val1 = psammite_read_f_register(vm, rs1);
        PsammiteFloat val2 = psammite_read_f_register(vm, rs2);
        int result = val1.f64 <= val2.f64 ? 1 : 0;
        psammite_write_register(vm, rd, result);
        return VM_OK;
    }
    case FCLASS64:
    {
        PsammiteFloat val = psammite_read_f_register(vm, rs1);
        uint64_t result = 0;
        switch (fpclassify(val.f64))
        {
        case FP_INFINITE:
            if (signbit(val.f64))
            {
                result = 1;
            }
            else
            {
                result = 1 << 7;
            }
            break;
        case FP_NAN:
        {
            result = 1 << 8;
            break;
        }
        case FP_NORMAL:
            if (signbit(val.f64))
            {
                result = 1 << 1;
            }
            else
            {
                result = 1 << 6;
            }
            break;
        case FP_SUBNORMAL:
            if (signbit(val.f64))
            {
                result = 1 << 2;
            }
            else
            {
                result = 1 << 5;
            }
            break;
        case FP_ZERO:
            if (signbit(val.f64))
            {
                result = 1 << 3;
            }
            else
            {
                result = 1 << 4;
            }
            break;
        default:
            break;
        }
    psammite_write_register(vm, rd, result);
    return VM_OK;
    }
    default:
        return VM_ERROR_UNRECOGNIZED;
    }
}