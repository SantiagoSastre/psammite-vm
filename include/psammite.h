#ifndef PSAMMITE_HEADER
#define PSAMMITE_HEADER


// Psammite uses arithmetic right shift for sign-extending immediate offsets.
#if ((-1 >> 1) != -1)
# error Psammite requires arithmetic right shift on signed integers
#endif

#include "psammite_core.h"
#include "psammite_status_codes.h"
#include "psammite_opcodes.h"
#include "psammite_execute_instructions.h"
#include "psammite_load_store_instructions.h"
#include "psammite_jump_instructions.h"
#include "psammite_branch_instructions.h"
#include "psammite_immediate_instructions.h"










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
