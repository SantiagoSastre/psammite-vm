#pragma once

#include "core.h"
#include "opcodes.h"
#include "decoders.h"
#include "imath.h"
#include "fmath.h"
#include "system.h"

static inline PsammiteStatusCodes psammite_route_execute(PsammiteVM *vm, uint32_t instruction)
{
  uint8_t func4 = psammite_decode_rtype_func4(instruction);
  uint8_t func7 = psammite_decode_rtype_func7(instruction);
  uint8_t rs1 = psammite_decode_rtype_rs1(instruction);
  uint8_t rs2 = psammite_decode_rtype_rs2(instruction);
  uint8_t rd = psammite_decode_rtype_rd(instruction);
  switch (func4)
  {
  case SYSTEM:
    return psammite_system_execute(vm, func7, rd);
  case IMATH:
    return psammite_imath_execute(vm, func7, rs1, rs2, rd);
  case FMATH:
    return psammite_fmath_execute(vm, func7, rs1, rs2, rd);
  default:
    return VM_ERROR_UNRECOGNIZED;
  }
}
