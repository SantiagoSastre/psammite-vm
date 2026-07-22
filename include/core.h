#ifndef PSAMMITE_CORE_HEADER
#define PSAMMITE_CORE_HEADER

#include <stdint.h>
#include <stddef.h>
#include <string.h>



#include "registers.h"
#include "endian.h"



#define PSAMMITE_MIN_MEM_SIZE 65536

typedef union {
  uint64_t bits;
  double as_float;
} PsammiteFloat;

typedef struct {
  uint64_t pc;
  uint32_t ir;
  size_t _memory_size;
  uint64_t _registers[NUM_REGISTER];
  PsammiteFloat _f_registers[NUM_REGISTER];
  uint8_t* _memory;
  
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


static inline int psammite_read_memory64(PsammiteVM *vm, uint64_t address, uint64_t *out_value) {
    if (address > vm->_memory_size) return 1;
    if (address + 8 > vm->_memory_size) return 1;
    memcpy(out_value,&vm->_memory[address],sizeof(uint64_t));
    *out_value = VM_TO_HOST64(*out_value);
    return 0;
}

static inline int psammite_read_memory32(PsammiteVM *vm, uint64_t address, uint32_t *out_value) {
    if (address > vm->_memory_size) return 1;
    if (address + 4 > vm->_memory_size) return 1;
    memcpy(out_value,&vm->_memory[address],sizeof(uint32_t));
    *out_value = VM_TO_HOST32(*out_value);
    return 0;
}

static inline int psammite_read_memory16(PsammiteVM *vm, uint64_t address, uint16_t *out_value) {
    if (address > vm->_memory_size) return 1;
    if (address + 2 > vm->_memory_size) return 1;
    memcpy(out_value,&vm->_memory[address],sizeof(uint16_t));
    *out_value = VM_TO_HOST16(*out_value);
    return 0;
}

static inline int psammite_read_memory8(PsammiteVM *vm, uint64_t address, uint8_t *out_value) {
    if (address > vm->_memory_size) return 1;
    if (address + 1 > vm->_memory_size) return 1;
    memcpy(out_value,&vm->_memory[address],sizeof(uint8_t));
    return 0;
}

static inline int psammite_write_memory64(PsammiteVM *vm, uint64_t address, uint64_t value) {
    if (address > vm->_memory_size) return 1;
    if (address + 8 > vm->_memory_size) return 1;
    uint64_t tmp = HOST_TO_VM64(value);
    memcpy(&vm->_memory[address], &tmp, sizeof(uint64_t));
    return 0;
}

static inline int psammite_write_memory32(PsammiteVM *vm, uint64_t address, uint32_t value) {
    if (address > vm->_memory_size) return 1;
    if (address + 4 > vm->_memory_size) return 1;
    uint32_t tmp = HOST_TO_VM32(value);
    memcpy(&vm->_memory[address], &tmp, sizeof(uint32_t));
    return 0;
}

static inline int psammite_write_memory16(PsammiteVM *vm, uint64_t address, uint16_t value) {
    if (address > vm->_memory_size) return 1;
    if (address + 2 > vm->_memory_size) return 1;
    uint16_t tmp = HOST_TO_VM16(value);
    memcpy(&vm->_memory[address], &tmp, sizeof(uint16_t));
    return 0;
}

static inline int psammite_write_memory8(PsammiteVM *vm, uint64_t address, uint8_t value) {
    if (address > vm->_memory_size) return 1;
    if (address + 1 > vm->_memory_size) return 1;
    memcpy(&vm->_memory[address], &value, sizeof(uint8_t));
    return 0;
}

static inline int psammite_fetch_to_ir(PsammiteVM *vm) {
  if (vm->pc+4>vm->_memory_size) {
    return 1;
  }
  memcpy(&vm->ir,&vm->_memory[vm->pc],sizeof(uint32_t));
  vm->ir = VM_TO_HOST32(vm->ir);
  vm->pc = vm->pc + 4;
  return 0;
}


#endif