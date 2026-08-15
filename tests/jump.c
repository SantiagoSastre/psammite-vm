#include "test_common.h"


void test_vm_jal()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_JAL(R4, 4),
      ASM_HALT,
      ASM_LI(R5, 5),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 4);
  VM_EXPECT(psammite_read_register(&vm, R5) == 5);
  VM_EXPECT(vm._pc == 16);

  psammite_free_memory(&vm);
}

void test_vm_jalr()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_JALR(R6, R4, 4),
      ASM_HALT,
      ASM_LI(R5, 5),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 8);
  VM_EXPECT(psammite_read_register(&vm, R5) == 5);
  VM_EXPECT(vm._pc == 20);

  psammite_free_memory(&vm);
}