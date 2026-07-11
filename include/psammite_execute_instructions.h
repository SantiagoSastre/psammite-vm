#ifndef PSAMMITE_EXECUTE_INSTRUCTIONS_HEADER
#define PSAMMITE_EXECUTE_INSTRUCTIONS_HEADER

#include <stdio.h>
#include "psammite_core.h"
#include "psammite_opcodes.h"
#include "psammite_decoders.h"
#include "psammite_status_codes.h"








//No other arguments for now to prevent compiler errors with strict flags, will add as more system opcodes use more elements
static inline InternalExitCodes psammite_system_execute(uint8_t func7) {
  switch (func7) {
    case HALT:
      return VM_HALT;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute System instruction, halting.\n");
      return VM_ERR_GENERIC;
  }
}

static inline InternalExitCodes psammite_imath_execute(PsammiteVM *vm, uint8_t func7, uint8_t rs1, uint8_t rs2, uint8_t rd) {
  uint64_t val1 = psammite_read_register(vm, rs1);
  uint64_t val2 = psammite_read_register(vm, rs2);
  switch (func7) {
    case ADD:
      psammite_write_register(vm, rd,  val1 + val2);
      return VM_OK;
    case SUB:
      psammite_write_register(vm, rd, val1 - val2);
      return VM_OK;
    case MUL:
      psammite_write_register(vm, rd,  val1 * val2);
      return VM_OK;
    case DIV:
      if (val2==0){
        psammite_write_register(vm,rd,(uint64_t)-1);
      } else {
      psammite_write_register(vm, rd, val1 / val2);
      }
      return VM_OK;
    case MOD:
      if (val2== 0){
        psammite_write_register(vm, rd, val1);
      } else {
      psammite_write_register(vm, rd, val1 % val2);
      }
      return VM_OK;
    case SDIV:
      if (val2==0){
        psammite_write_register(vm,rd,(uint64_t)-1);
      }
      else if ((int64_t)val1==INT64_MIN && (int64_t)val2==-1) {
        psammite_write_register(vm, rd, (uint64_t)INT64_MIN);
      }
      else {
          psammite_write_register(vm, rd, (uint64_t)((int64_t)val1 / (int64_t)val2));
      }
      return VM_OK;
    case SMOD:
      if (val2==0){
        psammite_write_register(vm, rd, val1);
      }
      else if ((int64_t)val1==INT64_MIN && (int64_t)val2==-1) {
        psammite_write_register(vm, rd, 0);
      }
      else {
          psammite_write_register(vm, rd, (uint64_t)((int64_t)val1 % (int64_t)val2));
      }
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute IMath instruction, halting.\n");
      return VM_ERR_GENERIC;

  }
}

static inline InternalExitCodes psammite_route_execute(PsammiteVM *vm, uint32_t instruction) {
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