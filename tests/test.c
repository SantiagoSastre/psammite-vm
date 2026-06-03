#include <stdlib.h>
#include <stdio.h>
#include "psammite.h"
#include "endian.h"

#define VM_ASSERT(condition)                                                   \
do {                                                                           \
  if(!(condition)) {                                                           \
    fprintf(stderr, "Test:     %s\n", __func__);                               \
    fprintf(stderr, "Location: %s:%d\n", __FILE__, __LINE__);                  \
    fprintf(stderr, "[PSAMMITE ERROR] Assertion failed:%s\n",#condition);      \
    exit(1);                                                                   \
  }                                                                            \
} while(0)


void test_vm_initialization() {
  PsammiteVM* vm = psammite_new();
  VM_ASSERT(vm!=NULL);
  VM_ASSERT(vm->pc==0);
  psammite_free(vm);
}


void test_vm_endianness() {
  PsammiteVM* vm = psammite_new();
  vm->memory[0] = 0xDD;
  vm->memory[1] = 0xCC;
  vm->memory[2] = 0xBB;
  vm->memory[3] = 0xAA;
  psammite_fetch_to_ir(vm);
  VM_ASSERT(vm->ir==0xAABBCCDD);
  psammite_free(vm);
  
}

int main() {
  test_vm_initialization();
  test_vm_endianness();
  return 0;
}
