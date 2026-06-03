#ifndef ENDIAN_HEADER
#define ENDIAN_HEADER

#if defined(_MSC_VER)
  //It is assumed the host is little endian if they use MSVC
  #define VM_TO_HOST32(instruction) (instruction)

#elif (defined(__GNUC__) || defined(__clang__)) && defined(__BYTE_ORDER__) && defined(__ORDER_LITTLE_ENDIAN__) && defined(__ORDER_BIG_ENDIAN__)

  #if __BYTE_ORDER__ == __ORDER_LITTLE_ENDIAN__
    #define VM_TO_HOST32(instruction) (instruction)
  #elif __BYTE_ORDER__ == __ORDER_BIG_ENDIAN__
    #define VM_TO_HOST32(instruction) __builtin_bswap32(instruction)
  #else
    #error __BYTE_ORDER__ set to invalid value.
  #endif

#else
  #error Incompatible/unrecognized compiler.

#endif


#endif
