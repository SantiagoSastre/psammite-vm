#ifndef PSAMMITE_IMATH
#define PSAMMITE_IMATH

#include <stdio.h>

#include "core.h"
#include "opcodes.h"
#include "status_codes.h"

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
    case AND:
      psammite_write_register(vm, rd, val1 & val2);
      return VM_OK;
    case OR:
      psammite_write_register(vm, rd, val1 | val2);
      return VM_OK;
    case XOR:
      psammite_write_register(vm, rd, val1 ^ val2);
      return VM_OK;
    case SLT:
      if (val1 < val2) {
        psammite_write_register(vm, rd, 1);
      } else {
        psammite_write_register(vm, rd, 0);
      }
      return VM_OK;
    case SSLT:
      if ( ( (int64_t) val1) < ( (int64_t) val2)) {
        psammite_write_register(vm, rd, 1);
      } else {
        psammite_write_register(vm, rd, 0);
      }
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute IMath instruction, halting.\n");
      return VM_ERR_GENERIC;

  }
}


#endif