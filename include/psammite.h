#pragma once


// Psammite uses arithmetic right shift for sign-extending immediate offsets.
#if ((-1 >> 1) != -1)
# error Psammite requires arithmetic right shift on signed integers
#endif

#include "core.h"
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
PsammiteStatusCodes psammite_get_panic_motive(const PsammiteVM *vm);
PsammiteVMState psammite_get_status(const PsammiteVM *vm);
int psammite_run(PsammiteVM *vm);


static inline PsammiteVMState psammite_step(PsammiteVM *vm) {
    PsammiteStatusCodes code = VM_OK;
    code = psammite_fetch_to_ir(vm);
  if (code != VM_OK) {
    vm->_status = VM_STATE_PANIC;
    vm->_panic_motive = code;
    return vm->_status;
  }
  uint32_t instruction = vm->_ir;
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
    case L32:
        code = psammite_l32(vm, instruction);
        break;
    case L32S:
        code = psammite_l32s(vm, instruction);
        break;
    case S32:
        code = psammite_s32(vm, instruction);
        break;
    case L16:
        code = psammite_l16(vm, instruction);
        break;
    case L16S:
        code = psammite_l16s(vm, instruction);
        break;
    case S16:
        code = psammite_s16(vm, instruction);
        break;
    case L8:
        code = psammite_l8(vm, instruction);
        break;
    case L8S:
        code = psammite_l8s(vm, instruction);
        break;
    case S8:
        code = psammite_s8(vm, instruction);
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
    case LF64:
        code = psammite_lf64(vm, instruction);
        break;
    case SF64:
        code = psammite_sf64(vm, instruction);
        break;
    default:
      code = VM_ERROR_UNRECOGNIZED;
      break;
  }
  if (code != VM_OK) {
    vm->_status = VM_STATE_PANIC;
    vm->_panic_motive = code;

  }
  return vm->_status;
  
}


