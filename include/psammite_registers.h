#ifndef PSAMMITE_REGISTERS_HEADER
#define PSAMMITE_REGISTERS_HEADER

#define NUM_REGISTER 32

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


#endif