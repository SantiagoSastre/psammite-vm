#ifndef PSAMMITE_STATUS_CODES_HEADER
#define PSAMMITE_STATUS_CODES_HEADER

typedef enum {
  VM_OK = 0,
  VM_ERR_GENERIC = 1,
  VM_HALT = 2
} InternalExitCodes;

#endif