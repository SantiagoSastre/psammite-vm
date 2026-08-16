#include "test_common.h"


void test_vm_beq()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_LI(R7, 8),
      ASM_LI(R8, 9),
      ASM_BEQ(R8, R6, 4),
      ASM_BEQ(R6, R7, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);

  psammite_free_memory(&vm);
}

void test_vm_bne()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_LI(R7, 8),
      ASM_LI(R8, 9),
      ASM_BNE(R7, R6, 4),
      ASM_BNE(R6, R8, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);

  psammite_free_memory(&vm);
}

void test_vm_blt()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_LI(R7, 8),
      ASM_LI(R8, 9),
      ASM_BLT(R6, R7, 4),
      ASM_BLT(R7, R8, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);

  psammite_free_memory(&vm);
}

void test_vm_bge()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_LI(R7, 8),
      ASM_LI(R8, 9),
      ASM_BGE(R7, R8, 4),
      ASM_BGE(R6, R7, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_BGE(R8, R6, 4),
      ASM_HALT,
      ASM_LI(R11, 4),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);
  VM_EXPECT(psammite_read_register(&vm, R11) == 4);

  psammite_free_memory(&vm);
}

void test_vm_sblt()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R6, -4),
      ASM_SLI(R7, -4),
      ASM_LI(R8, 4),
      ASM_SBLT(R6, R7, 4),
      ASM_SBLT(R7, R8, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);

  psammite_free_memory(&vm);
}

void test_vm_sbge()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R6, -4),
      ASM_SLI(R7, -4),
      ASM_LI(R8, 4),
      ASM_SBGE(R7, R8, 4),
      ASM_SBGE(R6, R7, 4),
      ASM_HALT,
      ASM_LI(R10, 2),
      ASM_SBGE(R8, R6, 4),
      ASM_HALT,
      ASM_LI(R11, 4),
      ASM_HALT

  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R10) == 2);
  VM_EXPECT(psammite_read_register(&vm, R11) == 4);

  psammite_free_memory(&vm);
}

void test_vm_slt()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 3),
      ASM_LI(R5, 4),
      ASM_SLT(R4, R5, R6),
      ASM_SLT(R5, R4, R7),
      ASM_LI(R8, 3),
      ASM_SLT(R8, R4, R9),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 1);
  VM_EXPECT(psammite_read_register(&vm, R7) == 0);
  VM_EXPECT(psammite_read_register(&vm, R9) == 0);

  psammite_free_memory(&vm);
}

void test_vm_sslt()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -3),
      ASM_SLI(R5, -4),
      ASM_SSLT(R4, R5, R6),
      ASM_SSLT(R5, R4, R7),
      ASM_LI(R8, 3),
      ASM_SSLT(R8, R4, R9),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 0);
  VM_EXPECT(psammite_read_register(&vm, R7) == 1);
  VM_EXPECT(psammite_read_register(&vm, R9) == 0);

  psammite_free_memory(&vm);
}