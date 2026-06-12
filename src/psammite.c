#include <stdlib.h>
#include <inttypes.h>
#include "psammite.h"


int psammite_reset(PsammiteVM *vm) {
  vm->pc = 0;
  vm->ir = 0;
  memset(vm->_registers, 0, sizeof(vm->_registers));
  memset(vm->_f_registers,0,sizeof(vm->_f_registers));
  memset(vm->_memory, 0, sizeof(vm->_memory));

  //Set the Stack Pointer to the end of memory
  psammite_write_register(vm, SP, VM_MEM_SIZE);


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
  memcpy(vm->_memory, program, program_size);
  return 0;
}


void psammite_print_registers(PsammiteVM *vm) {
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
      printf("%-4s: 0x%016" PRIX64 "    |    ",reg_name , psammite_read_register(vm, i) );
    }
    printf("\n");
}
void psammite_print_f_registers(PsammiteVM *vm) {
    printf("-----------------------Float Registers------------------------------------------------------------------------------------------\n");

    for(FRegister i = FR0; i<NUM_REGISTER; i++) {
      if (i%4==0 && i!=FR0) {
        printf("\n");
      }
      char reg_name[5];
      snprintf(reg_name, sizeof(reg_name), "FR%02d", i);
      printf("%-4s: %018.6lf    |    ",reg_name , psammite_read_f_register(vm, i).as_float );
    }
    printf("\n");
}
void psammite_print_memory_window(PsammiteVM *vm) {
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
      printf("%02X  ",vm->_memory[i]);
    }
    printf("                                        |\n");
}

void psammite_dump(PsammiteVM *vm) {
  printf("---------------------------Psammite VM------------------------------------------------------------------------------------------\n");
  printf("IR  : 0x%08X            |    PC  : 0x%016" PRIX64 "                                                                      |\n",vm->ir, vm->pc);
  psammite_print_registers(vm);
  psammite_print_f_registers(vm);
  psammite_print_memory_window(vm);
  printf("--------------------------------------------------------------------------------------------------------------------------------\n");
}

InternalExitCodes psammite_step(PsammiteVM *vm) {
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
    default:
      fprintf(stderr, "Unrecognized Opcode, halting.");
      code = VM_ERR_GENERIC;
      break;
  }
  return code;
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
