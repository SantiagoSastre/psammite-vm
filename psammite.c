#include <stdio.h>
#include <string.h>
#include <stdint.h>

#define NUM_REGISTER 32
#define VM_MEM_SIZE 65536


typedef union {
  uint64_t bits;
  double as_float;
} FRegister;

typedef struct {
  uint64_t pc;
  uint64_t registers[NUM_REGISTER];
  FRegister f_registers[NUM_REGISTER];
  uint8_t memory[VM_MEM_SIZE];
} PsammiteVM;



int psammite_reset(PsammiteVM *vm) {
  vm->pc = 0;
  memset(vm->registers, 0, sizeof(vm->registers));
  memset(vm->f_registers,0,sizeof(vm->f_registers));
  memset(vm->memory, 0, sizeof(vm->memory));
  return 0;
}

int psammite_init(PsammiteVM *vm) {
  if (psammite_reset(vm)!= 0 ) {
    return 1;
  }
  return 0;
  
}

int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size){
  if (program_size > VM_MEM_SIZE) {
    fprintf(stderr, "FATAL ERROR: Program too large for VM memory.");
    return 1;
  }
  memcpy(vm->memory, program, program_size);
  return 0;
}

int main() {
  return 0;
}
