#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <inttypes.h>
#include "psammite.h"
#include "endian.h"


int psammite_reset(PsammiteVM *vm) {
  vm->pc = 0;
  vm->ir = 0;
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
  printf("IR  : 0x%08X            |    PC  : 0x%016" PRIX64 "                                                                      |\n",vm->ir, vm->pc);
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
  printf("-----------------------PC(0x%016" PRIX64 ")-----------------------------------------------------------------------------------\n",vm->pc);
  uint64_t aligned_pc = vm->pc & (~((uint64_t)0x0F));
  uint64_t start_address, end_address;
  if(aligned_pc < 16){
    start_address = 0;
  } else {
    start_address = aligned_pc-16;
  }
  end_address = start_address+48;
  if (end_address > VM_MEM_SIZE) {
    end_address = VM_MEM_SIZE;
    start_address = end_address - 48;
    if (end_address < 48) {
      start_address = 0;
    }
  }
  for(uint64_t i = start_address; i<end_address;i++) {
    if(i%16==0){
      if(i!=start_address) {
        printf("                                        |\n");
      }
      printf("0x%016" PRIX64 "  |  " , i );
    }
    printf("%02X  ",vm->memory[i]);
  }
  printf("                                        |");
  printf("\n--------------------------------------------------------------------------------------------------------------------------------\n");
}

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


InternalExitCodes psammite_step(PsammiteVM *vm) {
  if (psammite_fetch_to_ir(vm)!=0) {
    return VM_ERR_GENERIC;
  }
  uint32_t instruction = vm->ir;
  uint8_t opcode = psammite_decode_opcode(instruction);
  switch (opcode) {
    case EXECUTE:
      return psammite_route_execute(vm, instruction);
    default:
      fprintf(stderr, "Unrecognized Opcode, halting.");
      return VM_ERR_GENERIC;
  
  }
  return VM_OK;
}

int psammite_run(PsammiteVM *vm) {
  InternalExitCodes code = VM_OK;
  while(code == VM_OK){
    code = psammite_step(vm);
  }
  if (code != VM_HALT) {
    return 1;
  }
  return 0;
}


