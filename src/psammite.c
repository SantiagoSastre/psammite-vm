#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
#include <inttypes.h>

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



int psammite_reset(PsammiteVM *vm) {
  vm->pc = 0;
  memset(vm->registers, 0, sizeof(vm->registers));
  memset(vm->f_registers,0,sizeof(vm->f_registers));
  memset(vm->memory, 0, sizeof(vm->memory));
  
  //Set the Stack Pointer to the end of memory
  vm->registers[SP] = VM_MEM_SIZE;
  
  
  return 0;
}

int psammite_init(PsammiteVM *vm) {
  if (psammite_reset(vm)!= 0 ) {
    return 1;
  }
  return 0;
  
}

PsammiteVM* psammite_new() {
  PsammiteVM* vm = malloc(sizeof(PsammiteVM));
  if (vm == NULL) {
    return NULL;
  }
  psammite_init(vm);
  return vm;
}

void psammite_free(PsammiteVM *vm) {
  free(vm);
}

int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size){
  if (program_size > VM_MEM_SIZE) {
    fprintf(stderr, "FATAL ERROR: Program too large for VM memory.");
    return 1;
  }
  memcpy(vm->memory, program, program_size);
  return 0;
}


void psammite_dump(PsammiteVM *vm) {
  printf("---------------------------Psammite VM------------------------------------------------------------------------------------------\n");
  printf("IR: 0x%08X              |    PC: 0x%016" PRIX64 "\n",vm->ir, vm->pc);
  printf("-----------------------Integer Registers----------------------------------------------------------------------------------------\n");
  for(Register i = ZR; i<NUM_REGISTER; i++) {
    if (i%4==0 && i!=ZR) {
      printf("\n");
    }
    char reg_name[4];
    switch (i) {
      case ZR:
        snprintf(reg_name,sizeof(reg_name), "ZR");
        break;
      case SP:
        snprintf(reg_name,sizeof(reg_name), "SP");
        break;
      case BP:
        snprintf(reg_name,sizeof(reg_name), "BP");
        break;
      case LR:
        snprintf(reg_name,sizeof(reg_name), "LR");
        break;
      default:
        snprintf(reg_name, sizeof(reg_name), "R%02d", i);
        break;
    }
    printf("%-4s: 0x%016" PRIX64 "    |    ",reg_name , vm->registers[i] );
  }
  printf("\n");

  printf("-----------------------Float Registers------------------------------------------------------------------------------------------\n");
  
  for(FRegister i = FR0; i<NUM_REGISTER; i++) {
    if (i%4==0 && i!=FR0) {
      printf("\n");
    }
    char reg_name[5];
    snprintf(reg_name, sizeof(reg_name), "FR%02d", i);
    printf("%-4s: %018.6lf    |    ",reg_name , vm->f_registers[i].as_float );
  }
  printf("\n");
}

//int psammite_run(PsammiteVM *vm) {
//
//  return 0;
//}


int main() {
  PsammiteVM* vm = psammite_new();
  if (vm == NULL) {
    fprintf(stderr, "VM could not be instantiated.");
    return 1;
  }
  psammite_dump(vm);
  psammite_free(vm);
  vm = NULL;
  return 0;
}
