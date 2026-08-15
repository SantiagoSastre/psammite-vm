#include "test_common.h"



void test_vm_addi()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_ADDI(ZR, R4, 3),
      ASM_ADDI(R4, R5, 14),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 3);
  VM_EXPECT(psammite_read_register(&vm, R5) == 17);

  psammite_free_memory(&vm);
}



void test_vm_alu()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 2),
      ASM_LI(R5, 1),
      ASM_LI(R6, 3),
      ASM_LI(R7, 4),
      ASM_SLI(R20, -2),
      ASM_SLI(R21, -6),
      ASM_ADD(R4, R5, R8),
      ASM_SUB(R4, R5, R9),
      ASM_MUL(R4, R6, R10),
      ASM_DIV(R7, R4, R11),
      ASM_MOD(R7, R4, R12),
      ASM_SDIV(R21, R20, R13),
      ASM_SMOD(R21, R20, R14),
      ASM_ADD(R20, R6, R15),

      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R8) == 3);
  VM_EXPECT(psammite_read_register(&vm, R9) == 1);
  VM_EXPECT(psammite_read_register(&vm, R10) == 6);
  VM_EXPECT(psammite_read_register(&vm, R11) == 2);
  VM_EXPECT(psammite_read_register(&vm, R12) == 0);
  VM_EXPECT(psammite_read_register(&vm, R13) == 3);
  VM_EXPECT(psammite_read_register(&vm, R14) == 0);
  VM_EXPECT(psammite_read_register(&vm, R15) == 1);

  psammite_free_memory(&vm);
}

void test_vm_lpcr()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LPCR(R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(196)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 196);

  psammite_free_memory(&vm);
}

void test_vm_ac()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R5, 0, 0xBAAD),
      ASM_AC(R5, 1, 0xCAFE),
      ASM_AC(R5, 2, 0xBEEF),
      ASM_AC(R5, 3, 0xDEAD),
      ASM_MOV(R5, R6),
      ASM_ACZ(R6, 1, 0xCAAF),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xDEADBEEFCAFEBAAD);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x00000000CAAF0000);

  psammite_free_memory(&vm);
}


void test_vm_and()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_AND(R4, R5, R6),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x8);

  psammite_free_memory(&vm);
}

void test_vm_or()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_OR(R4, R5, R6),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0xA);

  psammite_free_memory(&vm);
}

void test_vm_xor()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_XOR(R4, R5, R6),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x2);

  psammite_free_memory(&vm);
}

void test_vm_andi()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_ANDI(R4, R5, 0x8),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0x8);

  psammite_free_memory(&vm);
}

void test_vm_ori()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_ORI(R4, R5, 0x8),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xA);

  psammite_free_memory(&vm);
}

void test_vm_xori()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_XORI(R4, R5, 0x8),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0x2);

  psammite_free_memory(&vm);
}

void test_vm_sll()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_LI(R5, 1),
      ASM_SLL(R4, R5, R6),
      ASM_LI(R8, 64),
      ASM_SLL(R4, R8, R7),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0xa);
  VM_EXPECT(psammite_read_register(&vm, R7) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_srl()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_LI(R5, 1),
      ASM_SRL(R4, R5, R6),
      ASM_LI(R8, 64),
      ASM_SRL(R4, R8, R7),
      ASM_SRL(R4, ZR, R4),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x2);
  VM_EXPECT(psammite_read_register(&vm, R7) == 0x5);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_sra()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -8), // 0xfffffffffffffff8
      ASM_LI(R5, 1),
      ASM_SRA(R4, R5, R6),

      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0xFFFFFFFFFFFFFFFCULL);

  psammite_free_memory(&vm);
}

void test_vm_slli()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_SLLI(R4, R5, 1),
      ASM_SLLI(R4, R6, 64),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xa);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_srli()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_SRLI(R4, R6, 1),
      ASM_SRLI(R4, R7, 64),
      ASM_SRLI(R4, R4, 0),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0x2);
  VM_EXPECT(psammite_read_register(&vm, R7) == 0x5);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_srai()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -8), // 0xfffffffffffffff8
      ASM_SRAI(R4, R6, 1),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0xFFFFFFFFFFFFFFFCULL);

  psammite_free_memory(&vm);
}