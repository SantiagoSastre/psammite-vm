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
    default:
        return VM_ERROR_UNRECOGNIZED;
    }
}