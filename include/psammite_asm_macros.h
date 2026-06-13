#ifndef PSAMMITE_ASM_MACROS_HEADER
#define PSAMMITE_ASM_MACROS_HEADER



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


#define ASM_HALT R_ENCODER(EXECUTE,0,0,0,SYSTEM,HALT)
#define ASM_NOP R_ENCODER(EXECUTE,0,0,0,SYSTEM,NOP)

#define ASM_ADD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,ADD)
#define ASM_SUB(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SUB)
#define ASM_MUL(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,MUL)
#define ASM_DIV(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,DIV)
#define ASM_MOD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,MOD)
#define ASM_SDIV(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SDIV)
#define ASM_SMOD(rs1,rs2,rd) R_ENCODER(EXECUTE,rs1,rs2,rd,IMATH,SMOD)



#define ASM_ADDI(rs,rd,immediate) I2_ENCODER(ADDI,rs,rd,immediate)
#define ASM_LOADI(rd,immediate) I2_ENCODER(ADDI,ZR,rd,immediate)


#endif
