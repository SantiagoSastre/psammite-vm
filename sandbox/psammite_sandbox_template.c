#include "psammite.h"
#include "asm_macros.h"

// Copy this file into a new file called psammite_sandbox.c


// Write your program here.
uint8_t program[] = {
    ASM_HALT
};

int main() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);

  psammite_load_program(&vm, program, sizeof(program));
  psammite_run(&vm);
  psammite_dump(&vm);
  psammite_free_memory(&vm);
  return 0;
}
