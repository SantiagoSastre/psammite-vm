#ifndef PSAMMITE_ASM_MACROS_HEADER
#define PSAMMITE_ASM_MACROS_HEADER


#define CHUNK_ENCODER(opcode, rs, chunk, immediate) \
    (uint8_t) ((immediate) & 0xFF), \
    (uint8_t) (((immediate) >> 8) & 0xFF), \
    (uint8_t) (( (((chunk) & 0x03)<< 3) | (((rs) & 0x07) << 5)) & 0xF8), \
    (uint8_t) ((((rs) >> 3) & 0x03) | (((opcode) & 0x3F) << 2))


#define I1_ENCODER(opcode, rs, immediate) \
    (uint8_t) ((immediate) & 0xFF), \
    (uint8_t) (((immediate) >> 8) & 0xFF), \
    (uint8_t) ((((immediate) >> 16) & 0x1F) | (((rs) & 0x07)<<5)), \
    (uint8_t) ((((rs) >> 3) & 0x03) | (((opcode) & 0x3F) << 2))


#define I2_ENCODER(opcode, rs, rd, immediate) \
    (uint8_t) ((immediate) & 0xFF), \
    (uint8_t) (((immediate) >> 8) & 0xFF), \
    (uint8_t) (((rd) & 0x1F) | (((rs) & 0x07)<<5)), \
    (uint8_t) ((((rs) >> 3) & 0x03) | (((opcode) & 0x3F) << 2))


#define R_ENCODER(opcode, rs1, rs2, rd, func4, func7) \
    (uint8_t) (((func7) & 0x7F) | (((func4) & 0x1)<<7) ), \
    (uint8_t) ((((func4) >> 1) & 0x07) | (((rd) & 0x1F) << 3 ) ), \
    (uint8_t) (((rs2) & 0x1F) | (((rs1) & 0x07)<<5)), \
    (uint8_t) ((((rs1) >> 3) & 0x03) | (((opcode) & 0x3F) << 2))


#define ASM_64_BIT_CONST(constant) \
    (uint8_t) (((uint64_t)(constant)) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant))>>8) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant))>>16) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant))>>24) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant)) >> 32) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant)) >> 40) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant)) >> 48) & 0xFF), \
    (uint8_t) ((((uint64_t)(constant)) >> 56) & 0xFF)


#define ASM_HALT R_ENCODER(EXECUTE,0,0,0,SYSTEM,HALT)
#define ASM_NOP I2_ENCODER(ADDI,ZR,ZR,0) // pseudo-instruction

#define ASM_ADD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,ADD)
#define ASM_SUB(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SUB)
#define ASM_MUL(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,MUL)
#define ASM_DIV(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,DIV)
#define ASM_MOD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,MOD)
#define ASM_SDIV(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SDIV)
#define ASM_SMOD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SMOD)

#define ASM_LDC(rs, chunk, immediate) CHUNK_ENCODER(LDC, rs, chunk, immediate)
#define ASM_LDR(rs, immediate) I1_ENCODER(LDR, rs, immediate)
#define ASM_LD(rs, rd, offset) I2_ENCODER(LD, rs, rd, offset)
#define ASM_SD(rs, rmemd, offset) I2_ENCODER(SD, rs, rmemd, offset)

#define ASM_ADDI(rs,rd,immediate) I2_ENCODER(ADDI,rs,rd,immediate)
#define ASM_LOADI(rd,immediate) I2_ENCODER(ADDI,ZR,rd,immediate) // pseudo-instruction


#endif
