#include <stdlib.h>
#include <stdio.h>

#include <inttypes.h>
#include "psammite.h"

int psammite_reset(PsammiteVM *vm)
{
  vm->_pc = 0;
  vm->_ir = 0;
  vm->_status = VM_STATE_READY;
  vm->_panic_motive = VM_OK;
  memset(vm->_registers, 0, sizeof(vm->_registers));
  memset(vm->_f_registers, 0, sizeof(vm->_f_registers));
  memset(vm->_memory, 0, vm->_memory_size);

  return 0;
}

// Expects a zero initialized vm struct reference
int psammite_init(PsammiteVM *vm, size_t memory_size)
{
  if (memory_size < PSAMMITE_MIN_MEM_SIZE)
  {
    return 1;
  }
  vm->_status = VM_STATE_READY;
  vm->_panic_motive = VM_OK;
  vm->_memory_size = memory_size;
  vm->_memory = calloc(vm->_memory_size, 1);
  if (vm->_memory == NULL)
  {
    return 1;
  }
  return 0;
}

void psammite_free_memory(PsammiteVM *vm)
{
  free(vm->_memory);
  vm->_memory_size = 0;
  vm->_memory = NULL;
}

int psammite_load_program(PsammiteVM *vm, uint8_t *program, size_t program_size)
{
  if (program_size > vm->_memory_size)
  {
    return 1;
  }
  memcpy(vm->_memory, program, program_size);
  return 0;
}

static void psammite_print_registers(PsammiteVM *vm)
{
  printf("-----------------------Integer Registers----------------------------------------------------------------------------------------\n");
  for (PsammiteRegister i = ZR; i < NUM_REGISTER; i++)
  {
    if (i % 4 == 0 && i != ZR)
    {
      printf("\n");
    }
    char reg_name[4];
    switch (i)
    {
    case ZR:
      snprintf(reg_name, sizeof(reg_name), "ZR");
      break;
    case SP:
      snprintf(reg_name, sizeof(reg_name), "SP");
      break;
    case BP:
      snprintf(reg_name, sizeof(reg_name), "BP");
      break;
    case LR:
      snprintf(reg_name, sizeof(reg_name), "LR");
      break;
    default:
      snprintf(reg_name, sizeof(reg_name), "R%02d", i);
      break;
    }
    printf("%-4s: 0x%016" PRIX64 "    |    ", reg_name, psammite_read_register(vm, i));
  }
  printf("\n");
}
static void psammite_print_f_registers(PsammiteVM *vm)
{
  printf("-----------------------Float Registers------------------------------------------------------------------------------------------\n");

  for (PsammiteFRegister i = FR0; i < NUM_REGISTER; i++)
  {
    if (i % 4 == 0 && i != FR0)
    {
      printf("\n");
    }
    char reg_name[5];
    snprintf(reg_name, sizeof(reg_name), "FR%02d", i);
    printf("%-4s: %018.6lf    |    ", reg_name, psammite_read_f_register(vm, i).f64);
  }
  printf("\n");
}
static void psammite_print_memory_window(PsammiteVM *vm)
{
  printf("-----------------------PC(0x%016" PRIX64 ")-----------------------------------------------------------------------------------\n", vm->_pc);
  uint64_t aligned_pc = vm->_pc & (~((uint64_t)0x0F));
  uint64_t start_address, end_address;
  if (aligned_pc < 16)
  {
    start_address = 0;
  }
  else
  {
    start_address = aligned_pc - 16;
  }
  end_address = start_address + 48;
  if (end_address > vm->_memory_size)
  {
    end_address = vm->_memory_size;
    start_address = end_address - 48;
    if (end_address < 48)
    {
      start_address = 0;
    }
  }
  for (uint64_t i = start_address; i < end_address; i++)
  {
    if (i % 16 == 0)
    {
      if (i != start_address)
      {
        printf("                                        |\n");
      }
      printf("0x%016" PRIX64 "  |  ", i);
    }
    printf("%02X  ", vm->_memory[i]);
  }
  printf("                                        |\n");
}

static const char *dump_status_string(PsammiteVMState status)
{
  switch (status)
  {
  case VM_STATE_READY:
    return "READY";
  case VM_STATE_RUNNING:
    return "RUNNING";
  case VM_STATE_HALTED:
    return "HALTED";
  case VM_STATE_PANIC:
    return "PANIC";
  default:
    return "UNKNOWN";
  }
}

static const char *dump_motive_string(PsammiteStatusCodes motive)
{
  switch (motive)
  {
  case VM_OK:
    return "VM_OK";
  case VM_ERROR_UNRECOGNIZED:
    return "UNRECOGNIZED_OPCODE";
  case VM_ERROR_MEM_BOUNDS:
    return "MEM_BOUNDS";
  default:
    return "UNKNOWN_MOTIVE";
  }
}

void psammite_dump(PsammiteVM *vm)
{
  printf("---------------------------Psammite VM------------------------------------------------------------------------------------------\n");
  printf("STATUS: %-10s          |    MOTIVE: %-30s                                                        |\n",
         dump_status_string(vm->_status),
         dump_motive_string(vm->_panic_motive));
  printf("IR  : 0x%08X            |    PC  : 0x%016" PRIX64 "                                                                      |\n", vm->_ir, vm->_pc);
  psammite_print_registers(vm);
  psammite_print_f_registers(vm);
  psammite_print_memory_window(vm);
  printf("--------------------------------------------------------------------------------------------------------------------------------\n");
}

int psammite_run(PsammiteVM *vm)
{
  vm->_status = VM_STATE_RUNNING;
  while (vm->_status == VM_STATE_RUNNING)
  {
    psammite_step(vm);
  }
  if (vm->_status != VM_STATE_HALTED)
  {
    return 1;
  }
  return 0;
}

PsammiteStatusCodes psammite_get_panic_motive(const PsammiteVM *vm)
{
  return vm->_panic_motive;
}

PsammiteVMState psammite_get_status(const PsammiteVM *vm)
{
  return vm->_status;
}