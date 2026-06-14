#ifndef PSAMMITE_DECODERS_HEADER
#define PSAMMITE_DECODERS_HEADER

#include <stdint.h>


static inline uint8_t psammite_decode_opcode(uint32_t instruction) {
  uint8_t opcode = (instruction>>26) & 0x3F;
  return opcode;
}



static inline uint8_t psammite_decode_25_21(uint32_t instruction) {
  return (instruction>>21) & 0x1F;
}

static inline uint8_t psammite_decode_20_16(uint32_t instruction) {
  return (instruction>>16) & 0x1F;
}

static inline uint16_t psammite_decode_15_0(uint32_t instruction) {
  return instruction & 0xFFFF;
}

//------- R --------------------------

static inline uint8_t psammite_decode_rtype_func4(uint32_t instruction) {
  uint8_t func4 = (instruction>>7) & 0xF;
  return func4;
}

static inline uint8_t psammite_decode_rtype_func7(uint32_t instruction) {
  uint8_t func7 = instruction & 0x7F;
  return func7;
}

static inline uint8_t psammite_decode_rtype_rs1(uint32_t instruction) {
  return psammite_decode_25_21(instruction);
}

static inline uint8_t psammite_decode_rtype_rs2(uint32_t instruction) {
  return psammite_decode_20_16(instruction);
}

static inline uint8_t psammite_decode_rtype_rd(uint32_t instruction) {
  return (instruction>>11) & 0x1F;
}

//-------- J ------------------------

static inline uint8_t psammite_decode_jtype_rd(uint32_t instruction) {
  return psammite_decode_25_21(instruction);
}
static inline uint32_t psammite_decode_jtype_offset(uint32_t instruction) {
  return instruction & 0x1FFFFF;
}

//-------- I ------------------------

static inline uint8_t psammite_decode_itype_rs(uint32_t instruction) {
  return psammite_decode_25_21(instruction);
}

static inline uint8_t psammite_decode_itype_rd(uint32_t instruction) {
  return psammite_decode_20_16(instruction);
}
static inline uint16_t psammite_decode_itype_immediate(uint32_t instruction) {
  return psammite_decode_15_0(instruction);
}

//-------- Chunk ------------------------

static inline uint8_t psammite_decode_chunk_rd(uint32_t instruction) {
  return psammite_decode_25_21(instruction);
}

static inline uint8_t psammite_decode_chunk_selector(uint32_t instruction) {
  return (instruction>>19) & 0x3;
}

static inline uint16_t psammite_decode_chunk_immediate(uint32_t instruction) {
  return psammite_decode_15_0(instruction);
}





#endif
