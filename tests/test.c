#include "psammite.h"
#include "psammite_asm_macros.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <inttypes.h>

#define VM_ASSERT(condition)                                                   \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Test:     %s\n", __func__);                             \
      fprintf(stderr, "Location: %s:%d\n", __FILE__, __LINE__);                \
      fprintf(stderr, "[PSAMMITE ERROR] Assertion failed:%s\n", #condition);   \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

void test_vm_initialization() {
  PsammiteVM *vm = psammite_new();
  VM_ASSERT(vm != NULL);
  VM_ASSERT(vm->pc == 0);
  psammite_free(vm);
}



void test_vm_endianness() {
  PsammiteVM *vm = psammite_new();
  vm->_memory[0] = 0xDD;
  vm->_memory[1] = 0xCC;
  vm->_memory[2] = 0xBB;
  vm->_memory[3] = 0xAA;
  int status = psammite_fetch_to_ir(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(vm->ir == 0xAABBCCDD);
  psammite_free(vm);
}



void test_vm_addi() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LOADI(R4, 3), // LOADI is an alias for ADDI with its rs set to ZR
      ASM_ADDI(R4, R5, 14),
      ASM_HALT
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R4) == 3);
  VM_ASSERT(psammite_read_register(vm,R5) == 17);

  psammite_free(vm);
}


void test_vm_alu() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LOADI(R4, 2),
      ASM_LOADI(R5, 1),
      ASM_LOADI(R6, 3),
      ASM_LOADI(R7, 4),
      ASM_LOADI(R20, -2),
      ASM_LOADI(R21, -6),
      ASM_ADD(R4, R5, R8),
      ASM_SUB(R4, R5, R9),
      ASM_MUL(R4, R6, R10),
      ASM_DIV(R7, R4, R11),
      ASM_MOD(R7, R4, R12),
      ASM_SDIV(R21, R20, R13),
      ASM_SMOD(R21, R20, R14),
      ASM_ADD(R20, R6, R15),

      ASM_HALT
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R8) == 3);
  VM_ASSERT(psammite_read_register(vm,R9) == 1);
  VM_ASSERT(psammite_read_register(vm,R10) == 6);
  VM_ASSERT(psammite_read_register(vm,R11) == 2);
  VM_ASSERT(psammite_read_register(vm,R12) == 0);
  VM_ASSERT(psammite_read_register(vm,R13) == 3);
  VM_ASSERT(psammite_read_register(vm,R14) == 0);
  VM_ASSERT(psammite_read_register(vm,R15) == 1);

  psammite_free(vm);
}

void test_vm_ldr() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LDR(R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(196)
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R5) == 196);

  psammite_free(vm);
}

void test_vm_ldc() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
    ASM_LDC(R5, 0, 0xBAAD),
    ASM_LDC(R5, 1, 0xCAFE),
    ASM_LDC(R5, 2, 0xBEEF),
    ASM_LDC(R5, 3, 0xDEAD),
    ASM_HALT
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R5) == 0xDEADBEEFCAFEBAAD);

  psammite_free(vm);
}


void test_vm_ld() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LOADI(R4,8),
      ASM_LD(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(196)
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R5) == 196);

  psammite_free(vm);
}

void test_vm_sd() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LOADI(R4,196),
      ASM_LOADI(R5, 12),
      ASM_SD(R4, R5, 4),
      ASM_HALT,
  };
  psammite_load_program(vm, program, sizeof(program));
  int vm_status = psammite_run(vm);
  uint64_t ram_value;
  int read_status = psammite_read_memory(vm, 16, &ram_value);
  VM_ASSERT(vm_status == 0);
  VM_ASSERT(read_status == 0);
  VM_ASSERT(ram_value == 196);

  psammite_free(vm);
}

void test_vm_jal() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_JAL(R4,4),
      ASM_HALT,
      ASM_LOADI(R5, 5),
      ASM_HALT
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R4) == 4);
  VM_ASSERT(psammite_read_register(vm,R5) == 5);
  VM_ASSERT(vm->pc==16);

  psammite_free(vm);
}


void test_vm_jalr() {
  PsammiteVM *vm = psammite_new();
  uint8_t program[] = {
      ASM_LOADI(R6, 8),
      ASM_JALR(R6,R4,4),
      ASM_HALT,
      ASM_LOADI(R5, 5),
      ASM_HALT
  };
  psammite_load_program(vm, program, sizeof(program));
  int status = psammite_run(vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(vm,R4) == 8);
  VM_ASSERT(psammite_read_register(vm,R5) == 5);
  VM_ASSERT(vm->pc==20);

  psammite_free(vm);
}

int main() {
  test_vm_initialization();
  test_vm_endianness();
  test_vm_addi();
  test_vm_alu();
  test_vm_ldr();
  test_vm_ldc();
  test_vm_ld();
  test_vm_sd();
  test_vm_jal();
  test_vm_jalr();
  return 0;
}
