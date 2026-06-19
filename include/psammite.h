#ifndef PSAMMITE_HEADER
#define PSAMMITE_HEADER

#include <stdio.h>
#include <stdint.h>
#include <stddef.h>
#include <string.h>
#include "psammite_endian.h"
#include "psammite_decoders.h"

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
  ADDI = 0x05,
  JAL = 0x06,
  JALR = 0x07,
  BEQ = 0x08,
  BNE = 0x09,
  BLT = 0x10,
  BGE = 0x11,
  SBLT = 0x12,
  SBGE = 0x13,
} Opcodes;

typedef enum {
  SYSTEM = 0x00,
  IMATH = 0x01,

} ExecuteCategoryCode;

typedef enum {
  HALT = 0x00,
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
  uint64_t _registers[NUM_REGISTER];
  PsammiteFloat _f_registers[NUM_REGISTER];
  uint8_t _memory[VM_MEM_SIZE];
} PsammiteVM;


int psammite_reset(PsammiteVM *vm);
int psammite_init(PsammiteVM *vm);
PsammiteVM* psammite_new();
void psammite_free(PsammiteVM *vm);
int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size);
void psammite_dump(PsammiteVM *vm);
void psammite_print_registers(PsammiteVM *vm);
void psammite_print_f_registers(PsammiteVM *vm);
void psammite_print_memory_window(PsammiteVM *vm);
int psammite_run(PsammiteVM *vm);

static inline void psammite_write_register(PsammiteVM *vm, uint8_t reg, uint64_t value) {
  vm->_registers[reg] = value;
  vm->_registers[ZR] = 0; //Hard-wiring ZR to zero
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

static inline InternalExitCodes psammite_imath_execute(PsammiteVM *vm, uint8_t func7, uint8_t rs1, uint8_t rs2, uint8_t rd) {
  uint64_t val1 = psammite_read_register(vm, rs1);
  uint64_t val2 = psammite_read_register(vm, rs2);
  switch (func7) {
    case ADD:
      psammite_write_register(vm, rd,  val1 + val2);
      return VM_OK;
    case SUB:
      psammite_write_register(vm, rd, val1 - val2);
      return VM_OK;
    case MUL:
      psammite_write_register(vm, rd,  val1 * val2);
      return VM_OK;
    case DIV:
      if (val2==0){
        psammite_write_register(vm,rd,(uint64_t)-1);
      } else {
      psammite_write_register(vm, rd, val1 / val2);
      }
      return VM_OK;
    case MOD:
      if (val2== 0){
        psammite_write_register(vm, rd, val1);
      } else {
      psammite_write_register(vm, rd, val1 % val2);
      }
      return VM_OK;
    case SDIV:
      if (val2==0){
        psammite_write_register(vm,rd,(uint64_t)-1);
      }
      else if ((int64_t)val1==INT64_MIN && (int64_t)val2==-1) {
        psammite_write_register(vm, rd, (uint64_t)INT64_MIN);
      }
      else {
          psammite_write_register(vm, rd, (uint64_t)((int64_t)val1 / (int64_t)val2));
      }
      return VM_OK;
    case SMOD:
      if (val2==0){
        psammite_write_register(vm, rd, val1);
      }
      else if ((int64_t)val1==INT64_MIN && (int64_t)val2==-1) {
        psammite_write_register(vm, rd, 0);
      }
      else {
          psammite_write_register(vm, rd, (uint64_t)((int64_t)val1 % (int64_t)val2));
      }
      return VM_OK;
    default:
      fprintf(stderr, "Unrecognized Function 7 parameter in Execute IMath instruction, halting.\n");
      return VM_ERR_GENERIC;

  }
}

static inline InternalExitCodes psammite_route_execute(PsammiteVM *vm, uint32_t instruction) {
  uint8_t func4 = psammite_decode_rtype_func4(instruction);
  uint8_t func7 = psammite_decode_rtype_func7(instruction);
  uint8_t rs1 = psammite_decode_rtype_rs1(instruction);
  uint8_t rs2 = psammite_decode_rtype_rs2(instruction);
  uint8_t rd = psammite_decode_rtype_rd(instruction);
  switch (func4) {
    case SYSTEM:
      return psammite_system_execute(func7);
    case IMATH:
      return psammite_imath_execute(vm, func7, rs1, rs2, rd);
    default:
      fprintf(stderr, "Unrecognized Function 4 parameter in Execute instruction, halting.\n");
      return VM_ERR_GENERIC;
  }
}

static inline InternalExitCodes psammite_ldc(PsammiteVM *vm, uint32_t instruction) {
  uint8_t rd = psammite_decode_chunk_rd(instruction);
  uint8_t chunk_selector = psammite_decode_chunk_selector(instruction);
  uint8_t zero_flag = psammite_decode_chunk_zero_flag(instruction);
  uint16_t immediate = psammite_decode_chunk_immediate(instruction);
  uint64_t new_val;
  uint64_t immediate_at_chunk = ((uint64_t)immediate)<<(16*chunk_selector);
  if (zero_flag) {
      new_val = immediate_at_chunk;
  } else {
    uint64_t val = psammite_read_register(vm, rd);
    uint64_t mask = ~(((uint64_t)0xFFFF)<<(16*chunk_selector));
    uint64_t masked_val = val & mask;

    new_val = immediate_at_chunk | masked_val;
  }
  psammite_write_register(vm, rd, new_val);
  return VM_OK;
}

static inline InternalExitCodes psammite_ldr(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rd = psammite_decode_jtype_rd(instruction);
    uint32_t offset = psammite_decode_jtype_offset(instruction);
    int32_t signed_offset = ((int32_t)(offset<<11)) >> 11;
    uint64_t value;
    int code = psammite_read_memory(vm, vm->pc+signed_offset, &value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    psammite_write_register(vm, rd,value);
    return VM_OK;

}

static inline InternalExitCodes psammite_ld(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs = psammite_decode_itype_rs(instruction);
    uint8_t rd = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int16_t signed_offset = (int16_t) offset;
    uint64_t base_address = psammite_read_register(vm, rs);
    uint64_t final_address = base_address + signed_offset;
    uint64_t value;
    int code = psammite_read_memory(vm, final_address, &value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    psammite_write_register(vm, rd,value);
    return VM_OK;
}

static inline InternalExitCodes psammite_sd(PsammiteVM *vm, uint32_t instruction) {
    uint8_t value_reg = psammite_decode_itype_rs(instruction);
    uint8_t address_reg = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int16_t signed_offset = (int16_t) offset;
    uint64_t base_address = psammite_read_register(vm, address_reg);
    uint64_t final_address = base_address + signed_offset;
    uint64_t value = psammite_read_register(vm, value_reg);
    int code = psammite_write_memory(vm, final_address, value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    return VM_OK;

}

static inline InternalExitCodes psammite_addi(PsammiteVM *vm, uint32_t instruction) {
    uint8_t value_reg = psammite_decode_itype_rs(instruction);
    uint8_t address_reg = psammite_decode_itype_rd(instruction);
    uint16_t immediate = psammite_decode_itype_immediate(instruction);
    uint64_t value = psammite_read_register(vm, value_reg);
    uint64_t value_plus_immediate = value + (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, address_reg, value_plus_immediate);

    return VM_OK;

}

static inline InternalExitCodes psammite_jal(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rd = psammite_decode_jtype_rd(instruction);
    uint32_t offset = psammite_decode_jtype_offset(instruction);
    int64_t signed_offset = (int64_t)(((int32_t)(offset << 11)) >> 11) * 4;
    psammite_write_register(vm, rd, vm->pc);
    vm->pc = (vm->pc + (uint64_t) signed_offset);

    return VM_OK;
}

static inline InternalExitCodes psammite_jalr(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs = psammite_decode_itype_rs(instruction);
    uint8_t rd = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t base_address = psammite_read_register(vm, rs);
    psammite_write_register(vm, rd, vm->pc);
    vm->pc = (base_address + (uint64_t) signed_offset) & 0xFFFFFFFFFFFFFFFC;

    return VM_OK;
}

static inline InternalExitCodes psammite_beq(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 == val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_bne(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 != val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_blt(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 < val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_bge(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 >= val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_sblt(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    int64_t val1 = (int64_t) psammite_read_register(vm, rs1);
    int64_t val2 = (int64_t) psammite_read_register(vm, rs2);
    if (val1 < val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_sbge(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    int64_t val1 = (int64_t) psammite_read_register(vm, rs1);
    int64_t val2 = (int64_t) psammite_read_register(vm, rs2);
    if (val1 >= val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}


static inline InternalExitCodes psammite_step(PsammiteVM *vm) {
    InternalExitCodes code;
  if (psammite_fetch_to_ir(vm)!=0) {
    return VM_ERR_GENERIC;
  }
  uint32_t instruction = vm->ir;
  uint8_t opcode = psammite_decode_opcode(instruction);
  switch (opcode) {
    case EXECUTE:
      code = psammite_route_execute(vm, instruction);
      break;
    case LDC:
        code = psammite_ldc(vm, instruction);
        break;
    case LDR:
        code = psammite_ldr(vm, instruction);
        break;
    case LD:
        code = psammite_ld(vm, instruction);
        break;
    case SD:
        code = psammite_sd(vm, instruction);
        break;
    case ADDI:
        code = psammite_addi(vm, instruction);
        break;
    case JAL:
        code = psammite_jal(vm, instruction);
        break;
    case JALR:
        code = psammite_jalr(vm, instruction);
        break;
    case BEQ:
        code = psammite_beq(vm, instruction);
        break;
    case BNE:
        code = psammite_bne(vm, instruction);
        break;
    case BLT:
        code = psammite_blt(vm, instruction);
        break;
    case BGE:
        code = psammite_bge(vm, instruction);
        break;
    case SBLT:
        code = psammite_sblt(vm, instruction);
        break;
    case SBGE:
        code = psammite_sbge(vm, instruction);
        break;
    default:
      fprintf(stderr, "Unrecognized Opcode, halting.\n");
      code = VM_ERR_GENERIC;
      break;
  }
  return code;
}


#endif
