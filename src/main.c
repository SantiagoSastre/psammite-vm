#include "psammite.h"

int main() {
  PsammiteVM vm = {0};
  psammite_init(&vm, 0x100000);
  psammite_dump(&vm);
  psammite_free_memory(&vm);
  return 0;
}
