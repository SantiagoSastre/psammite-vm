#ifndef PSAMMITE_LOAD_STORE_INSTRUCTIONS_HEADER
#define PSAMMITE_LOAD_STORE_INSTRUCTIONS_HEADER


#include "core.h"
#include "decoders.h"
#include "status_codes.h"



static inline InternalExitCodes psammite_ldc(PsammiteVM *vm, uint32_t instruction) {
  uint8_t rd = psammite_decode_chunk_rd(instruction);
  uint8_t chunk_selector = psammite_decode_chunk_selector(instruction);
  uint8_t zero_flag = psammite_decode_chunk_zero_flag(instruction);
  uint16_t immediate = psammite_decode_chunk_immediate(instruction);
  uint64_t new_val;
  uint64_t immediate_at_chunk = ((uint64_t)immediate)<<(16*chunk_selector);
  if (zero_flag) {
      new_val = immediate_at_chunk;
  } else {
    uint64_t val = psammite_read_register(vm, rd);
    uint64_t mask = ~(((uint64_t)0xFFFF)<<(16*chunk_selector));
    uint64_t masked_val = val & mask;

    new_val = immediate_at_chunk | masked_val;
  }
  psammite_write_register(vm, rd, new_val);
  return VM_OK;
}

static inline InternalExitCodes psammite_ldr(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rd = psammite_decode_jtype_rd(instruction);
    uint32_t offset = psammite_decode_jtype_offset(instruction);
    int32_t signed_offset = ((int32_t)(offset<<11)) >> 11;
    uint64_t value;
    int code = psammite_read_memory(vm, vm->pc+signed_offset, &value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    psammite_write_register(vm, rd,value);
    return VM_OK;

}

static inline InternalExitCodes psammite_ld(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs = psammite_decode_itype_rs(instruction);
    uint8_t rd = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int16_t signed_offset = (int16_t) offset;
    uint64_t base_address = psammite_read_register(vm, rs);
    uint64_t final_address = base_address + signed_offset;
    uint64_t value;
    int code = psammite_read_memory(vm, final_address, &value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    psammite_write_register(vm, rd,value);
    return VM_OK;
}

static inline InternalExitCodes psammite_sd(PsammiteVM *vm, uint32_t instruction) {
    uint8_t value_reg = psammite_decode_itype_rs(instruction);
    uint8_t address_reg = psammite_decode_itype_rd(instruction);
    uint16_t offset = psammite_decode_itype_immediate(instruction);
    int16_t signed_offset = (int16_t) offset;
    uint64_t base_address = psammite_read_register(vm, address_reg);
    uint64_t final_address = base_address + signed_offset;
    uint64_t value = psammite_read_register(vm, value_reg);
    int code = psammite_write_memory(vm, final_address, value);
    if (code != 0) {
        return VM_ERR_GENERIC;
    }
    return VM_OK;

}

#endif