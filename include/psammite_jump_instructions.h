#ifndef PSAMMITE_JUMP_INSTRUCTIONS_HEADER
#define PSAMMITE_JUMP_INSTRUCTIONS_HEADER


#include "psammite_core.h"
#include "psammite_decoders.h"
#include "psammite_status_codes.h"


static inline InternalExitCodes psammite_jal(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rd = psammite_decode_jtype_rd(instruction);
    uint32_t offset = psammite_decode_jtype_offset(instruction);
    int64_t signed_offset = (int64_t)(((int32_t)(offset << 11)) >> 11) * 4;
    psammite_write_register(vm, rd, vm->pc);
    vm->pc = (vm->pc + (uint64_t) signed_offset);

    return VM_OK;
}

static inline InternalExitCodes psammite_jalr(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs = psammite_decode_itype_rs(instruction);
    uint8_t rd = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t base_address = psammite_read_register(vm, rs);
    psammite_write_register(vm, rd, vm->pc);
    vm->pc = (base_address + (uint64_t) signed_offset) & 0xFFFFFFFFFFFFFFFC;

    return VM_OK;
}



#endif