#include "psammite.h"

int main() {
  PsammiteVM* vm = psammite_new();
  if (vm == NULL) {
    fprintf(stderr, "VM could not be instantiated.");
    return 1;
  }
  psammite_dump(vm);
  psammite_free(vm);
  vm = NULL;
  return 0;
}
