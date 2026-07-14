#ifndef PSAMMITE_SYSTEM
#define PSAMMITE_SYSTEM

#include <stdio.h>

#include "core.h"
#include "opcodes.h"
#include "status_codes.h"



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



#endif