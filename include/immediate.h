#ifndef PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER
#define PSAMMITE_IMMEDIATE_INSTRUCTIONS_HEADER


#include "core.h"
#include "decoders.h"
#include "status_codes.h"

static inline PsammiteStatusCodes psammite_addi(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value + (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, rd, result);

    return VM_OK;

}

static inline PsammiteStatusCodes psammite_andi(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value & (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, rd, result);

    return VM_OK;

}

static inline PsammiteStatusCodes psammite_ori(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value | (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, rd, result);

    return VM_OK;

}

static inline PsammiteStatusCodes psammite_xori(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value ^ (uint64_t) ((int16_t)immediate);
    psammite_write_register(vm, rd, result);

    return VM_OK;

}


static inline PsammiteStatusCodes psammite_slli(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value <<  (immediate & 0x3F);
    psammite_write_register(vm, rd, result);
    return VM_OK;

}

static inline PsammiteStatusCodes psammite_srli(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result = value >>  (immediate & 0x3F);
    psammite_write_register(vm, rd, result);
    return VM_OK;

}

static inline PsammiteStatusCodes psammite_srai(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs, rd;
    uint16_t immediate;
    psammite_decode_itype(instruction, &rs, &rd, &immediate);
    uint64_t value = psammite_read_register(vm, rs);
    uint64_t result =  (uint64_t) ((int64_t) value >>  (immediate & 0x3F));
    psammite_write_register(vm, rd, result);
    return VM_OK;

}

#endif