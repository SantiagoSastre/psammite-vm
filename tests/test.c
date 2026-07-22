#include <stdlib.h>


#include "psammite.h"
#include "asm_macros.h"


#define VM_ASSERT(condition)                                                   \
  do {                                                                         \
    if (!(condition)) {                                                        \
      fprintf(stderr, "Test:     %s\n", __func__);                             \
      fprintf(stderr, "Location: %s:%d\n", __FILE__, __LINE__);                \
      fprintf(stderr, "[PSAMMITE ERROR] Assertion failed:%s\n", #condition);   \
      exit(1);                                                                 \
    }                                                                          \
  } while (0)

void test_vm_memory_initialization() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  VM_ASSERT(vm._memory != NULL);
  VM_ASSERT(vm.pc == 0);
  psammite_free_memory(&vm);
}

void test_vm_endianness() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  vm._memory[0] = 0xDD;
  vm._memory[1] = 0xCC;
  vm._memory[2] = 0xBB;
  vm._memory[3] = 0xAA;
  int status = psammite_fetch_to_ir(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(vm.ir == 0xAABBCCDD);
  psammite_free_memory(&vm);
}


void test_vm_get_mem_size() {
  PsammiteVM vm = {0};
  psammite_init(&vm, 0x100000);
  uint8_t program[] = {
      ASM_GET_MEMORY_SIZE(R4),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R4) == 0x100000);

  psammite_free_memory(&vm);
}

void test_vm_addi() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_ADDI(ZR,R4,3),
      ASM_ADDI(R4, R5, 14),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R4) == 3);
  VM_ASSERT(psammite_read_register(&vm,R5) == 17);

  psammite_free_memory(&vm);
}

void test_vm_zr_hardwiring() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(ZR, 3),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,ZR) == 0);

  psammite_free_memory(&vm);
}

void test_vm_oob() {
    PsammiteVM vm = {0};
    psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
    uint8_t program[] = {
        ASM_LI(R4, 0xFFFF),
        ASM_L64(R4, R5, 0),
        ASM_HALT
    };
    psammite_load_program(&vm, program, sizeof(program));
    int status = psammite_run(&vm);
    VM_ASSERT(status == 1);

    psammite_free_memory(&vm);
}


void test_vm_alu() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 2),
      ASM_LI(R5, 1),
      ASM_LI(R6, 3),
      ASM_LI(R7, 4),
      ASM_SLI(R20, -2),
      ASM_SLI(R21, -6),
      ASM_ADD(R4, R5, R8),
      ASM_SUB(R4, R5, R9),
      ASM_MUL(R4, R6, R10),
      ASM_DIV(R7, R4, R11),
      ASM_MOD(R7, R4, R12),
      ASM_SDIV(R21, R20, R13),
      ASM_SMOD(R21, R20, R14),
      ASM_ADD(R20, R6, R15),

      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R8) == 3);
  VM_ASSERT(psammite_read_register(&vm,R9) == 1);
  VM_ASSERT(psammite_read_register(&vm,R10) == 6);
  VM_ASSERT(psammite_read_register(&vm,R11) == 2);
  VM_ASSERT(psammite_read_register(&vm,R12) == 0);
  VM_ASSERT(psammite_read_register(&vm,R13) == 3);
  VM_ASSERT(psammite_read_register(&vm,R14) == 0);
  VM_ASSERT(psammite_read_register(&vm,R15) == 1);

  psammite_free_memory(&vm);
}

void test_vm_lpcr() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LPCR(R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(196)
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 196);

  psammite_free_memory(&vm);
}

void test_vm_ac() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
    ASM_AC(R5, 0, 0xBAAD),
    ASM_AC(R5, 1, 0xCAFE),
    ASM_AC(R5, 2, 0xBEEF),
    ASM_AC(R5, 3, 0xDEAD),
    ASM_MOV(R5, R6),
    ASM_ACZ(R6, 1, 0xCAAF),
    ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 0xDEADBEEFCAFEBAAD);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0x00000000CAAF0000);


  psammite_free_memory(&vm);
}

void test_vm_l64() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4,8),
      ASM_L64(R4, R5, 4),
      ASM_HALT,
      ASM_64_BIT_CONST(196)
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 196);

  psammite_free_memory(&vm);
}

void test_vm_s64() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4,196),
      ASM_LI(R5, 12),
      ASM_S64(R4, R5, 4),
      ASM_HALT,
  };
  psammite_load_program(&vm, program, sizeof(program));
  int vm_status = psammite_run(&vm);
  uint64_t ram_value;
  int read_status = psammite_read_memory64(&vm, 16, &ram_value);
  VM_ASSERT(vm_status == 0);
  VM_ASSERT(read_status == 0);
  VM_ASSERT(ram_value == 196);

  psammite_free_memory(&vm);
}

void test_vm_jal() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_JAL(R4,4),
      ASM_HALT,
      ASM_LI(R5, 5),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R4) == 4);
  VM_ASSERT(psammite_read_register(&vm,R5) == 5);
  VM_ASSERT(vm.pc==16);

  psammite_free_memory(&vm);
}

void test_vm_jalr() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R6, 8),
      ASM_JALR(R6,R4,4),
      ASM_HALT,
      ASM_LI(R5, 5),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R4) == 8);
  VM_ASSERT(psammite_read_register(&vm,R5) == 5);
  VM_ASSERT(vm.pc==20);

  psammite_free_memory(&vm);
}

void test_vm_beq() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);


  psammite_free_memory(&vm);
}

void test_vm_bne() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);


  psammite_free_memory(&vm);
}

void test_vm_blt() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);


  psammite_free_memory(&vm);
}

void test_vm_bge() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);
  VM_ASSERT(psammite_read_register(&vm,R11) == 4);


  psammite_free_memory(&vm);
}

void test_vm_sblt() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);


  psammite_free_memory(&vm);
}

void test_vm_sbge() {
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
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R10) == 2);
  VM_ASSERT(psammite_read_register(&vm,R11) == 4);


  psammite_free_memory(&vm);
}


void test_vm_slt() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 3),
      ASM_LI(R5, 4),
      ASM_SLT(R4,R5,R6),
      ASM_SLT(R5,R4,R7),
      ASM_LI(R8, 3),
      ASM_SLT(R8,R4,R9),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 1);
  VM_ASSERT(psammite_read_register(&vm,R7) == 0);
  VM_ASSERT(psammite_read_register(&vm,R9) == 0);

  psammite_free_memory(&vm);
}

void test_vm_sslt() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -3),
      ASM_SLI(R5, -4),
      ASM_SSLT(R4,R5,R6),
      ASM_SSLT(R5,R4,R7),
      ASM_LI(R8, 3),
      ASM_SSLT(R8,R4,R9),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0);
  VM_ASSERT(psammite_read_register(&vm,R7) == 1);
  VM_ASSERT(psammite_read_register(&vm,R9) == 0);

  psammite_free_memory(&vm);
}


void test_vm_and() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_AND(R4,R5,R6),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0x8);

  psammite_free_memory(&vm);
}

void test_vm_or() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_OR(R4,R5,R6),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0xA);

  psammite_free_memory(&vm);
}

void test_vm_xor() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_LI(R5, 0x8),
      ASM_XOR(R4,R5,R6),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0x2);

  psammite_free_memory(&vm);
}

void test_vm_andi() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_ANDI(R4,R5,0x8),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 0x8);

  psammite_free_memory(&vm);
}

void test_vm_ori() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_ORI(R4,R5,0x8),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 0xA);

  psammite_free_memory(&vm);
}

void test_vm_xori() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0xA),
      ASM_XORI(R4,R5,0x8),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 0x2);

  psammite_free_memory(&vm);
}

void test_vm_sll() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_LI(R5, 1),
      ASM_SLL(R4,R5,R6),
      ASM_LI(R8, 64),
      ASM_SLL(R4,R8,R7),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0xa);
  VM_ASSERT(psammite_read_register(&vm, R7) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_srl() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_LI(R5, 1),
      ASM_SRL(R4,R5,R6),
      ASM_LI(R8, 64),
      ASM_SRL(R4,R8,R7),
      ASM_SRL(R4, ZR, R4),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0x2);
  VM_ASSERT(psammite_read_register(&vm, R7) == 0x5);
  VM_ASSERT(psammite_read_register(&vm,R4) == 0x5);


  psammite_free_memory(&vm);
}


void test_vm_sra() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -8), // 0xfffffffffffffff8
      ASM_LI(R5, 1),
      ASM_SRA(R4,R5,R6),

      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0xFFFFFFFFFFFFFFFCULL);


  psammite_free_memory(&vm);
}

void test_vm_slli() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_SLLI(R4,R5,1),
      ASM_SLLI(R4,R6,64),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R5) == 0xa);
  VM_ASSERT(psammite_read_register(&vm, R6) == 0x5);

  psammite_free_memory(&vm);
}

void test_vm_srli() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_LI(R4, 0x5),
      ASM_SRLI(R4,R6,1),
      ASM_SRLI(R4,R7,64),
      ASM_SRLI(R4,R4,0),
      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0x2);
  VM_ASSERT(psammite_read_register(&vm, R7) == 0x5);
  VM_ASSERT(psammite_read_register(&vm,R4) == 0x5);


  psammite_free_memory(&vm);
}


void test_vm_srai() {
  PsammiteVM vm = {0};
  psammite_init(&vm, PSAMMITE_MIN_MEM_SIZE);
  uint8_t program[] = {
      ASM_SLI(R4, -8), // 0xfffffffffffffff8
      ASM_SRAI(R4,R6,1),

      ASM_HALT
  };
  psammite_load_program(&vm, program, sizeof(program));
  int status = psammite_run(&vm);
  VM_ASSERT(status == 0);
  VM_ASSERT(psammite_read_register(&vm,R6) == 0xFFFFFFFFFFFFFFFCULL);


  psammite_free_memory(&vm);
}

int main() {
  test_vm_memory_initialization();
  test_vm_endianness();
  test_vm_get_mem_size();
  test_vm_addi();
  test_vm_zr_hardwiring();
  test_vm_oob();
  test_vm_alu();
  test_vm_lpcr();
  test_vm_ac();
  test_vm_l64();
  test_vm_s64();
  test_vm_jal();
  test_vm_jalr();
  test_vm_beq();
  test_vm_bne();
  test_vm_blt();
  test_vm_bge();
  test_vm_sblt();
  test_vm_sbge();
  test_vm_slt();
  test_vm_sslt();
  test_vm_and();
  test_vm_or();
  test_vm_xor();
  test_vm_andi();
  test_vm_ori();
  test_vm_xori();
  test_vm_sll();
  test_vm_srl();
  test_vm_sra();
  test_vm_slli();
  test_vm_srli();
  test_vm_srai();

  return 0;
}
