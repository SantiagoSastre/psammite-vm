#include "test_common.h"

void test_vm_imf64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_L64(ZR, R5, 12),
      ASM_IMF64(R5, FR0),
      ASM_HALT,
      ASM_64_BIT_CONST(0xFCFBFAFDFEFCFBFCULL)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_f_register(&vm, FR0).bits == 0xFCFBFAFDFEFCFBFCULL);

  psammite_free_memory(&vm);
}

void test_vm_f64mi()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LF64(ZR, FR0, 12),
      ASM_F64MI(FR0, R4),
      ASM_HALT,
      ASM_64_BIT_CONST(0xFCFBFAFDFEFCFBFCULL)};
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0xFCFBFAFDFEFCFBFCULL);

  psammite_free_memory(&vm);
}

void test_vm_icf64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 5),
      ASM_SLI(R5, -5),
      ASM_ICF64(R4, 0, FR0),
      ASM_ICF64(R5, 1, FR1),
      ASM_ICF64(R5, 0, FR2),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR0).f64 - 5.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR1).f64 + 5.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR2).f64 - 18446744073709552000.0) < 0.0000001);

  psammite_free_memory(&vm);
}

void test_vm_f64ci()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_F64CI(FR0, TRUNCATION, 0, R4),
      ASM_F64CI(FR1, CEIL, 0, R5),
      ASM_F64CI(FR2, CEIL, 0, R6),
      ASM_F64CI(FR3, FLOOR, 0, R7),
      ASM_F64CI(FR4, FLOOR, 1, R8),
      ASM_F64CI(FR5, MATH_ROUNDING, 0, R9),
      ASM_F64CI(FR6, MATH_ROUNDING, 0, R10),
      ASM_F64CI(FR7, MATH_ROUNDING, 1, R11),
      ASM_F64CI(FR8, BANKER_ROUNDING, 0, R12),
      ASM_F64CI(FR9, BANKER_ROUNDING, 0, R13),
      ASM_F64CI(FR10, BANKER_ROUNDING, 0, R14),
      ASM_F64CI(FR11, BANKER_ROUNDING, 0, R15),
      ASM_F64CI(FR11, BANKER_ROUNDING, 1, R16),
      ASM_HALT,
  };
  // to be truncated
  vm._f_registers[FR0].f64 = 2.7;
  // to be ceiled
  vm._f_registers[FR1].f64 = 2.1;
  vm._f_registers[FR2].f64 = 2.9;
  // to be floored
  vm._f_registers[FR3].f64 = 4.1;
  vm._f_registers[FR4].f64 = -4.9;
  // to be math rounded
  vm._f_registers[FR5].f64 = 1.1;
  vm._f_registers[FR6].f64 = 3.5;
  vm._f_registers[FR7].f64 = -4.5;
  // to be rounded with banker's rounding
  vm._f_registers[FR8].f64 = 7.1;
  vm._f_registers[FR9].f64 = 2.5;
  vm._f_registers[FR10].f64 = 7.5;
  vm._f_registers[FR11].f64 = INFINITY;
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 2);
  VM_EXPECT(psammite_read_register(&vm, R5) == 3);
  VM_EXPECT(psammite_read_register(&vm, R6) == 3);
  VM_EXPECT(psammite_read_register(&vm, R7) == 4);
  VM_EXPECT((int64_t)psammite_read_register(&vm, R8) == -5);
  VM_EXPECT(psammite_read_register(&vm, R9) == 1);
  VM_EXPECT(psammite_read_register(&vm, R10) == 4);
  VM_EXPECT((int64_t)psammite_read_register(&vm, R11) == -5);
  VM_EXPECT(psammite_read_register(&vm, R12) == 7);
  VM_EXPECT(psammite_read_register(&vm, R13) == 2);
  VM_EXPECT(psammite_read_register(&vm, R14) == 8);
  VM_EXPECT(psammite_read_register(&vm, R15) == UINT64_MAX);
  VM_EXPECT(psammite_read_register(&vm, R16) == (uint64_t)INT64_MAX);

  psammite_free_memory(&vm);
}

void test_vm_fpu()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_IMF64(ZR, FR5),
      ASM_FADD64(FR0, FR3, FR6),
      ASM_FSUB64(FR0, FR3, FR7),
      ASM_FMUL64(FR2, FR3, FR8),
      ASM_FDIV64(FR4, FR1, FR9),
      ASM_FDIV64(FR0, FR5, FR10),
      ASM_FSQRT64(FR11, FR12),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = 2.0;
  vm._f_registers[FR2].f64 = -3.5;
  vm._f_registers[FR3].f64 = -4.2;
  vm._f_registers[FR4].f64 = 4.5;
  vm._f_registers[FR11].f64 = 16.0;
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 + 1.2) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR7).f64 - 7.2) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR8).f64 - 14.7) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR9).f64 - 2.25) < 0.0000001);
  VM_EXPECT(isinf(psammite_read_f_register(&vm, FR10).f64));
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR12).f64 - 4.0) < 0.0000001);
}

void test_vm_fsi64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FSI64(FR0,FR1,FR4),
      ASM_FSI64(FR2,FR1,FR5),
      ASM_FSI64(FR0,FR3,FR6),
      ASM_FSI64(FR1,FR0,FR7),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;
  vm._f_registers[FR2].f64 = -3.0;
  vm._f_registers[FR3].f64 = 2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR4).f64 + 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR5).f64 + 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR7).f64 - 2.0) < 0.0000001);

}
void test_vm_fsin64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FSIN64(FR0,FR1,FR4),
      ASM_FSIN64(FR2,FR1,FR5),
      ASM_FSIN64(FR0,FR3,FR6),
      ASM_FSIN64(FR1,FR0,FR7),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;
  vm._f_registers[FR2].f64 = -3.0;
  vm._f_registers[FR3].f64 = 2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR4).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR5).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 + 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR7).f64 + 2.0) < 0.0000001);

}

void test_vm_fsix64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FSIX64(FR0,FR1,FR4),
      ASM_FSIX64(FR2,FR1,FR5),
      ASM_FSIX64(FR0,FR3,FR6),
      ASM_FSIX64(FR1,FR0,FR7),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;
  vm._f_registers[FR2].f64 = -3.0;
  vm._f_registers[FR3].f64 = 2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR4).f64 + 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR5).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR7).f64 + 2.0) < 0.0000001);

}

void test_vm_fmin64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FMIN64(FR0,FR1,FR4),
      ASM_FMIN64(FR2,FR1,FR5),
      ASM_FMIN64(FR0,FR3,FR6),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;
  vm._f_registers[FR2].f64 = -2.0;
  vm._f_registers[FR3].f64 = 2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR4).f64 + 2.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR5).f64 + 2.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 - 2.0) < 0.0000001);
}

void test_vm_fmax64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FMAX64(FR0,FR1,FR4),
      ASM_FMAX64(FR2,FR1,FR5),
      ASM_FMAX64(FR0,FR3,FR6),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;
  vm._f_registers[FR2].f64 = -2.0;
  vm._f_registers[FR3].f64 = 2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR4).f64 - 3.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR5).f64 + 2.0) < 0.0000001);
  VM_EXPECT(fabs(psammite_read_f_register(&vm, FR6).f64 - 3.0) < 0.0000001);
}

void test_vm_feq64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FEQ64(FR0,FR1,R4),
      ASM_FEQ64(FR1,FR1,R5),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 1);

}

void test_vm_flt64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FLT64(FR0,FR1,R4),
      ASM_FLT64(FR1,FR1,R5),
      ASM_FLT64(FR1,FR0,R6),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 0);
  VM_EXPECT(psammite_read_register(&vm, R6) == 1);

}

void test_vm_fle64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FLE64(FR0,FR1,R4),
      ASM_FLE64(FR1,FR1,R5),
      ASM_FLE64(FR1,FR0,R6),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = 3.0;
  vm._f_registers[FR1].f64 = -2.0;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 0);
  VM_EXPECT(psammite_read_register(&vm, R5) == 1);
  VM_EXPECT(psammite_read_register(&vm, R6) == 1);

}

void test_vm_fclass64()
{
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_FCLASS64(FR0, R4),
      ASM_FCLASS64(FR1, R5),
      ASM_FCLASS64(FR2, R6),
      ASM_FCLASS64(FR3, R7),
      ASM_FCLASS64(FR4, R8),
      ASM_FCLASS64(FR5, R9),
      ASM_FCLASS64(FR6, R10),
      ASM_FCLASS64(FR7, R11),
      ASM_FCLASS64(FR8, R12),
      ASM_HALT,
  };
  vm._f_registers[FR0].f64 = -INFINITY;
  vm._f_registers[FR1].f64 = -1.0;
  vm._f_registers[FR2].f64 = -(DBL_MIN / 2);
  vm._f_registers[FR3].f64 = -0.0;
  vm._f_registers[FR4].f64 = 0.0;
  vm._f_registers[FR5].f64 = DBL_MIN / 2;
  vm._f_registers[FR6].f64 = 1.0;
  vm._f_registers[FR7].f64 = INFINITY;
  vm._f_registers[FR8].f64 = NAN;

  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_EXPECT(status == 0);
  VM_EXPECT(psammite_read_register(&vm, R4) == 1);
  VM_EXPECT(psammite_read_register(&vm, R5) == 1 << 1);
  VM_EXPECT(psammite_read_register(&vm, R6) == 1 << 2);
  VM_EXPECT(psammite_read_register(&vm, R7) == 1 << 3);
  VM_EXPECT(psammite_read_register(&vm, R8) == 1 << 4);
  VM_EXPECT(psammite_read_register(&vm, R9) == 1 << 5);
  VM_EXPECT(psammite_read_register(&vm, R10) == 1 << 6);
  VM_EXPECT(psammite_read_register(&vm, R11) == 1 << 7);
  VM_EXPECT(psammite_read_register(&vm, R12) == 1 << 8);

}