#pragma once

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
#include <float.h>

#include "psammite.h"
#include "asm_macros.h"

#define VM_EXPECT(condition)                                                  \
  do                                                                          \
  {                                                                           \
    if (!(condition))                                                         \
    {                                                                         \
      fprintf(stderr, "Test:     %s\n", __func__);                            \
      fprintf(stderr, "Location: %s:%d\n", __FILE__, __LINE__);               \
      fprintf(stderr, "[PSAMMITE ERROR] Assertion failed: %s\n", #condition); \
      failed_tests++;                                                         \
    }                                                                         \
  } while (0)
