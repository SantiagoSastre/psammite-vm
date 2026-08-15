#include "test_common.h"


void test_vm_memory_initialization()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  VM_EXPECT(vm._memory != NULL);
  VM_EXPECT(vm._pc == 0);
  psammite_free_memory(&vm);
}

void test_vm_endianness()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  vm._memory[0] = 0xDD;
  vm._memory[1] = 0xCC;
  vm._memory[2] = 0xBB;
  vm._memory[3] = 0xAA;
  int status = psammite_fetch_to_ir(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(vm._ir == 0xAABBCCDD);
  psammite_free_memory(&vm);
}

void test_vm_zr_hardwiring()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(ZR, 3),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, ZR) == 0);

  psammite_free_memory(&vm);
}

void test_vm_oob()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_AC(R4,0, 0xFFFF),
      ASM_AC(R4,1, 0xFFFF),
      ASM_L64(R4, R5, 0),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 1);
  VM_EXPECT(vm._panic_motive == VM_ERROR_MEM_BOUNDS);

  psammite_free_memory(&vm);
}