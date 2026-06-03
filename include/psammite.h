#ifndef PSAMMITE_HEADER
#define PSAMMITE_HEADER


#include <stdint.h>
#include <stddef.h>

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
} IMathCodes;


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
void psammite_fetch_to_ir(PsammiteVM *vm);
int psammite_step(PsammiteVM *vm);
int psammite_run(PsammiteVM *vm);

#endif
