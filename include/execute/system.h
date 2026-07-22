#ifndef PSAMMITE_SYSTEM
#define PSAMMITE_SYSTEM

#include <stdio.h>

#include "core.h"
#include "opcodes.h"
#include "status_codes.h"



//No rs1, rs2 arguments for now to prevent compiler errors with strict flags, will add as more system opcodes use more elements
static inline PsammiteStatusCodes psammite_system_execute(PsammiteVM *vm, uint8_t func7, uint8_t rd) {
  switch (func7) {
    case HALT:
      return VM_HALT;
    case GET_MEMORY_SIZE:
      psammite_write_register(vm, rd, vm->_memory_size);
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute System instruction, halting.\n");
      return VM_ERR_GENERIC;
  }
}



#endif