#pragma once

typedef enum {
  EXECUTE = 0x00,
  AC      = 0x01,
  LPCR    = 0x02,
  L64     = 0x03,
  S64     = 0x04,
  L32     = 0x05,
  L32S    = 0x06,
  S32     = 0x07,
  L16     = 0x08,
  L16S    = 0x09,
  S16     = 0x0A,
  L8      = 0x0B,
  L8S     = 0x0C,
  S8      = 0x0D,
  ADDI    = 0x0E,
  ANDI    = 0x0F,
  ORI     = 0x10,
  XORI    = 0x11,
  SLLI    = 0x12,
  SRLI    = 0x13,
  SRAI    = 0x14,
  JAL     = 0x15,
  JALR    = 0x16,
  BEQ     = 0x17,
  BNE     = 0x18,
  BLT     = 0x19,
  BGE     = 0x1A,
  SBLT    = 0x1B,
  SBGE    = 0x1C,
  // Float instructions
  LF64 = 0x1D,
  SF64 = 0x1E,
} Opcodes;

typedef enum {
  SYSTEM = 0x00,
  IMATH = 0x01,
  FMATH = 0x02,

} ExecuteCategoryCode;

typedef enum {
  HALT = 0x00,
  GET_MEMORY_SIZE = 0x01,
} SystemCodes;

typedef enum {
  ADD = 0x00,
  SUB = 0x01,
  MUL = 0x02,
  DIV = 0x03,
  MOD = 0x04,
  SDIV = 0x05,
  SMOD = 0x06,
  AND = 0x07,
  OR = 0x08,
  XOR = 0x09,
  SLT = 0x0A,
  SSLT = 0x0B,
  SLL = 0x0C,
  SRL = 0x0D,
  SRA = 0x0E

} IMathCodes;

typedef enum {
  IMF64 = 0x01,
  F64MI = 0x02,
  ICF64 = 0x03,
  F64CI = 0x04,
  FADD64 = 0x05,
} FMathCodes;

