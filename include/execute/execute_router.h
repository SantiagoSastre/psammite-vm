#ifndef PSAMMITE_EXECUTE_INSTRUCTIONS_HEADER
#define PSAMMITE_EXECUTE_INSTRUCTIONS_HEADER

#include <stdio.h>
#include "core.h"
#include "opcodes.h"
#include "decoders.h"
#include "status_codes.h"
#include "imath.h"
#include "system.h"










static inline PsammiteStatusCodes psammite_route_execute(PsammiteVM *vm, uint32_t instruction) {
  uint8_t func4 = psammite_decode_rtype_func4(instruction);
  uint8_t func7 = psammite_decode_rtype_func7(instruction);
  uint8_t rs1 = psammite_decode_rtype_rs1(instruction);
  uint8_t rs2 = psammite_decode_rtype_rs2(instruction);
  uint8_t rd = psammite_decode_rtype_rd(instruction);
  switch (func4) {
    case SYSTEM:
      return psammite_system_execute(func7);
    case IMATH:
      return psammite_imath_execute(vm, func7, rs1, rs2, rd);
    default:
      fprintf(stderr, "Unrecognized Function 4 parameter in Execute instruction, halting.\n");
      return VM_ERR_GENERIC;
  }
}

#endif