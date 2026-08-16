#include "test_common.h"


void test_vm_l64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L64(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xCAFECAFECAFECAFE)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xCAFECAFECAFECAFE);

  psammite_free_memory(&vm);
}

void test_vm_l32()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L32(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xCFFFFFFFF)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xFFFFFFFF);

  psammite_free_memory(&vm);
}

void test_vm_l32s()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L32S(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xC80000000)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xFFFFFFFF80000000);

  psammite_free_memory(&vm);
}

void test_vm_l16()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L16(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xC1234)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0x1234);

  psammite_free_memory(&vm);
}

void test_vm_l16s()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L16S(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xC8000)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xFFFFFFFFFFFF8000);

  psammite_free_memory(&vm);
}

void test_vm_l8()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L8(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xC7F)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0x7F);

  psammite_free_memory(&vm);
}

void test_vm_l8s()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_L8S(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xC80)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0xFFFFFFFFFFFFFF80);

  psammite_free_memory(&vm);
}

void test_vm_lf64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 8),
      ASM_LF64(R4, FR5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(0x40091EB851EB851F)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_f_register(&vm, FR5).f64 == 3.14);

  psammite_free_memory(&vm);
}

void test_vm_s64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R5, 0, 0xBAAD),
      ASM_AC(R5, 1, 0xCAFE),
      ASM_AC(R5, 2, 0xBEEF),
      ASM_AC(R5, 3, 0xDEAD),
      ASM_LI(R4, 24),
      ASM_S64(R5, R4, 4),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int vm_status = psammite_run(&vm);
  uint64_t ram_value = 0;
  PsammiteStatusCodes read_status = psammite_read_memory64(&vm, 28, &ram_value);
  VM_EXPECT(vm_status == 0);
  VM_EXPECT(read_status == VM_OK);
  VM_EXPECT(ram_value == 0xDEADBEEFCAFEBAAD);

  psammite_free_memory(&vm);
}

void test_vm_s32()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R5, 0, 0xBAAD),
      ASM_AC(R5, 1, 0xCAFE),
      ASM_AC(R5, 2, 0xBEEF),
      ASM_AC(R5, 3, 0xDEAD),
      ASM_LI(R4, 24),
      ASM_S32(R5, R4, 4),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int vm_status = psammite_run(&vm);
  uint64_t ram_value = 0;
  PsammiteStatusCodes read_status = psammite_read_memory64(&vm, 28, &ram_value);
  VM_EXPECT(vm_status == 0);
  VM_EXPECT(read_status == VM_OK);
  VM_EXPECT(ram_value == 0xCAFEBAAD);

  psammite_free_memory(&vm);
}

void test_vm_s16()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R5, 0, 0xBAAD),
      ASM_AC(R5, 1, 0xCAFE),
      ASM_AC(R5, 2, 0xBEEF),
      ASM_AC(R5, 3, 0xDEAD),
      ASM_LI(R4, 24),
      ASM_S16(R5, R4, 4),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int vm_status = psammite_run(&vm);
  uint64_t ram_value = 0;
  PsammiteStatusCodes read_status = psammite_read_memory64(&vm, 28, &ram_value);
  VM_EXPECT(vm_status == 0);
  VM_EXPECT(read_status == VM_OK);
  VM_EXPECT(ram_value == 0xBAAD);

  psammite_free_memory(&vm);
}

void test_vm_s8()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R5, 0, 0xBAAD),
      ASM_AC(R5, 1, 0xCAFE),
      ASM_AC(R5, 2, 0xBEEF),
      ASM_AC(R5, 3, 0xDEAD),
      ASM_LI(R4, 24),
      ASM_S8(R5, R4, 4),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int vm_status = psammite_run(&vm);
  uint64_t ram_value = 0;
  PsammiteStatusCodes read_status = psammite_read_memory64(&vm, 28, &ram_value);
  VM_EXPECT(vm_status == 0);
  VM_EXPECT(read_status == VM_OK);
  VM_EXPECT(ram_value == 0xAD);

  psammite_free_memory(&vm);
}

void test_vm_sf64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 12),
      ASM_SF64(FR5, R4, 0),
      ASM_HALT,
  };
  PsammiteFloat value;
  value.f64 = 3.14;
  psammite_load_program(&vm, program, sizeof(program));
  psammite_write_f_register(&vm, FR5, value);
  int vm_status = psammite_run(&vm);
  uint64_t ram_value = 0;
  PsammiteStatusCodes read_status = psammite_read_memory64(&vm, 12, &ram_value);
  VM_EXPECT(vm_status == 0);
  VM_EXPECT(read_status == VM_OK);
  VM_EXPECT(ram_value == 0x40091EB851EB851F);

  psammite_free_memory(&vm);
}