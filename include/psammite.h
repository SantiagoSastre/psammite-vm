#ifndef PSAMMITE_HEADER
#define PSAMMITE_HEADER

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "endian.h"

#define NUM_REGISTER 32
#define VM_MEM_SIZE 65536


typedef enum {
  ZR = 0,
  SP = 1,
  BP = 2,
  LR = 3,
  R4 = 4,
  R5 = 5,
  R6 = 6,
  R7 = 7,
  R8 = 8,
  R9 = 9,
  R10 = 10,
  R11 = 11,
  R12 = 12,
  R13 = 13,
  R14 = 14,
  R15 = 15,
  R16 = 16,
  R17 = 17,
  R18 = 18,
  R19 = 19,
  R20 = 20,
  R21 = 21,
  R22 = 22,
  R23 = 23,
  R24 = 24,
  R25 = 25,
  R26 = 26,
  R27 = 27,
  R28 = 28,
  R29 = 29,
  R30 = 30,
  R31 = 31
} Register;



typedef enum {
  FR0 = 0,
  FR1 = 1,
  FR2 = 2,
  FR3 = 3,
  FR4 = 4,
  FR5 = 5,
  FR6 = 6,
  FR7 = 7,
  FR8 = 8,
  FR9 = 9,
  FR10 = 10,
  FR11 = 11,
  FR12 = 12,
  FR13 = 13,
  FR14 = 14,
  FR15 = 15,
  FR16 = 16,
  FR17 = 17,
  FR18 = 18,
  FR19 = 19,
  FR20 = 20,
  FR21 = 21,
  FR22 = 22,
  FR23 = 23,
  FR24 = 24,
  FR25 = 25,
  FR26 = 26,
  FR27 = 27,
  FR28 = 28,
  FR29 = 29,
  FR30 = 30,
  FR31 = 31
} FRegister;


typedef enum {
  EXECUTE = 0x00,
  LDC = 0x01,
  LDR = 0x02,
  LD = 0x03,
  SD = 0x04,
} Opcodes;

typedef enum {
  SYSTEM = 0x00,
  IMATH = 0x01,
  
} ExecuteCategoryCode;

typedef enum {
  HALT = 0x00,
  NOP = 0x01,
} SystemCodes;

typedef enum {
  ADD = 0x00,
  SUB = 0x01,
  MUL = 0x02,
  DIV = 0x03,
  MOD = 0x04,
  SDIV = 0x05,
  SMOD = 0x06
} IMathCodes;


typedef enum {
  VM_OK = 0,
  VM_ERR_GENERIC = 1,
  VM_HALT = 2
} InternalExitCodes;

typedef union {
  uint64_t bits;
  double as_float;
} PsammiteFloat;

typedef struct {
  uint64_t pc;
  uint32_t ir;
  uint64_t registers[NUM_REGISTER];
  PsammiteFloat f_registers[NUM_REGISTER];
  uint8_t memory[VM_MEM_SIZE];
} PsammiteVM;


int psammite_reset(PsammiteVM *vm);
int psammite_init(PsammiteVM *vm);
PsammiteVM* psammite_new();
void psammite_free(PsammiteVM *vm);
int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size);
void psammite_dump(PsammiteVM *vm);
InternalExitCodes psammite_step(PsammiteVM *vm);
int psammite_run(PsammiteVM *vm);

static inline void psammite_write_register(PsammiteVM *vm, uint8_t reg, uint64_t value) {
  vm->registers[reg] = value;
  vm->registers[ZR] = 0; //Hard-wiring ZR to zero
}


static inline int psammite_fetch_to_ir(PsammiteVM *vm) {
  if (vm->pc+4>VM_MEM_SIZE) {
    return 1;
  }
  memcpy(&vm->ir,&vm->memory[vm->pc],sizeof(uint8_t)*4);
  vm->ir = VM_TO_HOST32(vm->ir);
  vm->pc = vm->pc + 4;
  return 0;
}

static inline uint8_t psammite_decode_opcode(uint32_t instruction) {
  uint8_t opcode = (instruction>>26) & 0x3F;
  return opcode;
}

static inline uint8_t psammite_decode_func4(uint32_t instruction) {
  uint8_t func4 = (instruction>>7) & 0xF;
  return func4;
}

static inline uint8_t psammite_decode_func7(uint32_t instruction) {
  uint8_t func7 = instruction & 0x7F;
  return func7;
}

static inline uint8_t psammite_decode_rs1(uint32_t instruction) {
  uint8_t rs1 = (instruction>>21) & 0x1F;
  return rs1;
}

static inline uint8_t psammite_decode_rs2(uint32_t instruction) {
  uint8_t rs2 = (instruction>>16) & 0x1F;
  return rs2;
}

static inline uint8_t psammite_decode_rd(uint32_t instruction) {
  uint8_t rd = (instruction>>11) & 0x1F;
  return rd;
}

//No other arguments for now to prevent compiler errors with strict flags, will add as more system opcodes use more elements
static inline InternalExitCodes psammite_system_execute(uint8_t func7) {
  switch (func7) {
    case HALT:
      return VM_HALT;
    case NOP:
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute System instruction, halting.");
      return VM_ERR_GENERIC;
  }
}

static inline InternalExitCodes psammite_imath_execute(PsammiteVM *vm, uint8_t func7, uint8_t rs1, uint8_t rs2, uint8_t rd) {
  switch (func7) {
    case ADD:
      psammite_write_register(vm, rd,  vm->registers[rs1] + vm->registers[rs2]);
      return VM_OK;
    case SUB:
      psammite_write_register(vm, rd, vm->registers[rs1] - vm->registers[rs2]);
      return VM_OK;
    case MUL:
      psammite_write_register(vm, rd,  vm->registers[rs1] * vm->registers[rs2]);
      return VM_OK;
    case DIV:
      if (vm->registers[rs2]==0){
        psammite_write_register(vm,rd,0xFFFFFFFFFFFFFFFF);
      } else {
      psammite_write_register(vm, rd, vm->registers[rs1] / vm->registers[rs2]);
      }
      return VM_OK;
    case MOD:
      if (vm->registers[rs2]==0){
        psammite_write_register(vm, rd, vm->registers[rs1]);
      } else {
      psammite_write_register(vm, rd, vm->registers[rs1] % vm->registers[rs2]);
      }
      return VM_OK;
    case SDIV:
      if (vm->registers[rs2]==0){
        psammite_write_register(vm,rd,0xFFFFFFFFFFFFFFFF);
      }
      else if ((int64_t)vm->registers[rs1]==INT64_MIN && (int64_t)vm->registers[rs2]==-1) {
        psammite_write_register(vm, rd, INT64_MAX);
      }
      else {
      psammite_write_register(vm, rd,(int64_t)vm->registers[rs1] / (int64_t)vm->registers[rs2]);
      }
      return VM_OK;
    case SMOD:
      if (vm->registers[rs2]==0){
        psammite_write_register(vm, rd, vm->registers[rs1]);
      }
      else if ((int64_t)vm->registers[rs1]==INT64_MIN && (int64_t)vm->registers[rs2]==-1) {
        psammite_write_register(vm, rd, 0);
      }
      else {
      psammite_write_register(vm, rd, (int64_t)vm->registers[rs1] % (int64_t)vm->registers[rs2]);
      }
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute IMath instruction, halting.");
      return VM_ERR_GENERIC;
  
  }
}

static inline InternalExitCodes psammite_route_execute(PsammiteVM *vm, uint32_t instruction) {
  uint8_t func4 = psammite_decode_func4(instruction);
  uint8_t func7 = psammite_decode_func7(instruction);
  uint8_t rs1 = psammite_decode_rs1(instruction);
  uint8_t rs2 = psammite_decode_rs2(instruction);
  uint8_t rd = psammite_decode_rd(instruction);
  switch (func4) {
    case SYSTEM:
      return psammite_system_execute(func7);
    case IMATH:
      return psammite_imath_execute(vm, func7, rs1, rs2, rd);
    default:
      fprintf(stderr, "Unrecognized Function 4 parameter in Execute instruction, halting.");
      return VM_ERR_GENERIC;
  }
}


#endif
