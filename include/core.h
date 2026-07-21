#ifndef PSAMMITE_CORE_HEADER
#define PSAMMITE_CORE_HEADER

#include <stdint.h>
#include <stddef.h>
#include <string.h>



#include "registers.h"
#include "endian.h"



#define VM_MEM_SIZE 65536

typedef union {
  uint64_t bits;
  double as_float;
} PsammiteFloat;

typedef struct {
  uint64_t pc;
  uint32_t ir;
  size_t memory_size;
  uint64_t _registers[NUM_REGISTER];
  PsammiteFloat _f_registers[NUM_REGISTER];
  uint8_t _memory[VM_MEM_SIZE];
} PsammiteVM;






static inline void psammite_write_register(PsammiteVM *vm, uint8_t reg, uint64_t value) {
    if (reg != 0) {
        vm->_registers[reg] = value;
    }
}

static inline void psammite_write_f_register(PsammiteVM *vm, uint8_t reg, PsammiteFloat value) {
    vm->_f_registers[reg] = value;
}

static inline uint64_t psammite_read_register(PsammiteVM *vm, uint8_t reg) {
    return vm->_registers[reg];
}

static inline PsammiteFloat psammite_read_f_register(PsammiteVM *vm, uint8_t reg) {
    return vm->_f_registers[reg];
}


static inline int psammite_read_memory(PsammiteVM *vm, uint64_t address, uint64_t *out_value) {
    if (address+8>VM_MEM_SIZE) {
        return 1;
    }
    memcpy(out_value,&vm->_memory[address],sizeof(uint64_t));
    *out_value = VM_TO_HOST64(*out_value);
    return 0;
}

static inline int psammite_write_memory(PsammiteVM *vm, uint64_t address, uint64_t value) {
    if (address+8>VM_MEM_SIZE) {
        return 1;
    }
    uint64_t tmp = HOST_TO_VM64(value);
    memcpy(&vm->_memory[address], &tmp, sizeof(uint64_t));
    return 0;
}

static inline int psammite_fetch_to_ir(PsammiteVM *vm) {
  if (vm->pc+4>VM_MEM_SIZE) {
    return 1;
  }
  memcpy(&vm->ir,&vm->_memory[vm->pc],sizeof(uint32_t));
  vm->ir = VM_TO_HOST32(vm->ir);
  vm->pc = vm->pc + 4;
  return 0;
}


#endif