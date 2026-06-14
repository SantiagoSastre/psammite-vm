#include <stdint.h>
#include <stdio.h>
#include "psammite.h"
#include "psammite_asm_macros.h"

// Copy this file into a new file called psammite_sandbox.c


// Write your program here.
uint8_t program[] = {
    ASM_HALT
};

int main() {
  PsammiteVM* vm = psammite_new();
  if (vm == NULL) {
    fprintf(stderr, "VM could not be instantiated.\n");
    return 1;
  }

  psammite_load_program(vm, program, sizeof(program));
  psammite_run(vm);
  psammite_dump(vm);
  psammite_free(vm);
  vm = NULL;
  return 0;
}
