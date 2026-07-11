#ifndef PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER
#define PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER


#include "psammite_core.h"
#include "psammite_decoders.h"
#include "psammite_status_codes.h"

static inline InternalExitCodes psammite_addi(PsammiteVM *vm, uint32_t instruction) {
    uint8_t value_reg = psammite_decode_itype_rs(instruction);
    uint8_t address_reg = psammite_decode_itype_rd(instruction);
    uint16_t immediate = psammite_decode_itype_immediate(instruction);
    uint64_t value = psammite_read_register(vm, value_reg);
    uint64_t value_plus_immediate = value + (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, address_reg, value_plus_immediate);

    return VM_OK;

}

#endif