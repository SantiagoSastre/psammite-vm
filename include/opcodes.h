#ifndef PSAMMITE_OPCODES_HEADER
#define PSAMMITE_OPCODES_HEADER

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
  BLT = 0x0A,
  BGE = 0x0B,
  SBLT = 0x0C,
  SBGE = 0x0D,
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
  SSLT = 0x0B
} IMathCodes;

#endif