#ifndef PSAMMITE_BRANCH_INSTRUCTIONS_HEADER
#define PSAMMITE_BRANCH_INSTRUCTIONS_HEADER


#include "psammite_core.h"
#include "psammite_decoders.h"
#include "psammite_status_codes.h"


static inline InternalExitCodes psammite_beq(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 == val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_bne(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 != val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_blt(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 < val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_bge(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    uint64_t val1 = psammite_read_register(vm, rs1);
    uint64_t val2 = psammite_read_register(vm, rs2);
    if (val1 >= val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_sblt(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    int64_t val1 = (int64_t) psammite_read_register(vm, rs1);
    int64_t val2 = (int64_t) psammite_read_register(vm, rs2);
    if (val1 < val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

static inline InternalExitCodes psammite_sbge(PsammiteVM *vm, uint32_t instruction) {
    uint8_t rs1 = psammite_decode_btype_rs1(instruction);
    uint8_t rs2 = psammite_decode_btype_rs2(instruction);
    uint16_t offset = psammite_decode_btype_offset(instruction);
    int64_t signed_offset = (int64_t)((int16_t)offset) * 4;
    int64_t val1 = (int64_t) psammite_read_register(vm, rs1);
    int64_t val2 = (int64_t) psammite_read_register(vm, rs2);
    if (val1 >= val2) {
        vm->pc = vm->pc + ((uint64_t) signed_offset);
    }

    return VM_OK;
}

#endif