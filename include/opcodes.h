#ifndef PSAMMITE_OPCODES_HEADER
#define PSAMMITE_OPCODES_HEADER

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
  SBGE    = 0x1C
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

#endif