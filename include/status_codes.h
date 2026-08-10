#pragma once

typedef enum
{
  VM_STATE_READY = 0,
  VM_STATE_RUNNING,
  VM_STATE_HALTED,
  VM_STATE_PANIC,
} PsammiteVMState;

typedef enum
{
  VM_OK = 0,
  VM_ERROR,
  VM_ERROR_UNRECOGNIZED,
  VM_ERROR_MEM_BOUNDS,
} PsammiteStatusCodes;
