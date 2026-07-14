#ifndef PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER
#define PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER


#include "core.h"
#include "decoders.h"
#include "status_codes.h"

static inline InternalExitCodes psammite_addi(PsammiteVM *vm, uint32_t instruction) {
    uint8_t src_reg = psammite_decode_itype_rs(instruction);
    uint8_t dst_reg = psammite_decode_itype_rd(instruction);
    uint16_t immediate = psammite_decode_itype_immediate(instruction);
    uint64_t value = psammite_read_register(vm, src_reg);
    uint64_t value_plus_immediate = value + (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, dst_reg, value_plus_immediate);

    return VM_OK;

}

#endif