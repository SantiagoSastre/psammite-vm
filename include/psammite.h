#ifndef PSAMMITE_HEADER
#define PSAMMITE_HEADER


// Psammite uses arithmetic right shift for sign-extending immediate offsets.
#if ((-1 >> 1) != -1)
# error Psammite requires arithmetic right shift on signed integers
#endif

#include "core.h"
#include "status_codes.h"
#include "opcodes.h"
#include "execute/execute_router.h"
#include "load_store.h"
#include "jump.h"
#include "branch.h"
#include "immediate.h"










int psammite_reset(PsammiteVM *vm);
int psammite_init(PsammiteVM *vm, size_t memory_size);
void psammite_free_memory(PsammiteVM *vm);
int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size);
void psammite_dump(PsammiteVM *vm);
void psammite_print_registers(PsammiteVM *vm);
void psammite_print_f_registers(PsammiteVM *vm);
void psammite_print_memory_window(PsammiteVM *vm);
int psammite_run(PsammiteVM *vm);











static inline PsammiteStatusCodes psammite_step(PsammiteVM *vm) {
    PsammiteStatusCodes code;
  if (psammite_fetch_to_ir(vm)!=0) {
    return VM_ERR_GENERIC;
  }
  uint32_t instruction = vm->ir;
  uint8_t opcode = psammite_decode_opcode(instruction);
  switch (opcode) {
    case EXECUTE:
      code = psammite_route_execute(vm, instruction);
      break;
    case AC:
        code = psammite_ac(vm, instruction);
        break;
    case LPCR:
        code = psammite_lpcr(vm, instruction);
        break;
    case L64:
        code = psammite_l64(vm, instruction);
        break;
    case S64:
        code = psammite_s64(vm, instruction);
        break;
    case ADDI:
        code = psammite_addi(vm, instruction);
        break;
    case ANDI:
        code = psammite_andi(vm, instruction);
        break;
    case ORI:
        code = psammite_ori(vm, instruction);
        break;
    case XORI:
        code = psammite_xori(vm, instruction);
        break;
    case SLLI:
        code = psammite_slli(vm, instruction);
        break;
    case SRLI:
        code = psammite_srli(vm, instruction);
        break;
    case SRAI:
        code = psammite_srai(vm, instruction);
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
