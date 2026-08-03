#ifndef PSAMMITE_SYSTEM
#define PSAMMITE_SYSTEM


#include "core.h"
#include "opcodes.h"



//No rs1, rs2 arguments for now to prevent compiler errors with strict flags, will add as more system opcodes use more elements
static inline PsammiteStatusCodes psammite_system_execute(PsammiteVM *vm, uint8_t func7, uint8_t rd) {
  switch (func7) {
    case HALT:
      vm->_status = VM_STATE_HALTED;
      return VM_OK;
    case GET_MEMORY_SIZE:
      psammite_write_register(vm, rd, vm->_memory_size);
      return VM_OK;
    default:
      return VM_ERROR_UNRECOGNIZED;
  }
}



#endif