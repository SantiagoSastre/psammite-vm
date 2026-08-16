#include "test_common.h"

void test_vm_get_memory_size()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_GET_MEMORY_SIZE(R4),
      ASM_HALT};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == PSAMMITE_MIN_MEM_SIZE);

  psammite_free_memory(&vm);
}