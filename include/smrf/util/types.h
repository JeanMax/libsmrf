#ifndef _TYPES_h
#define _TYPES_h

#include <stdlib.h> // size_t
#include <sys/types.h> // pid_t

#include <stdint.h>
typedef uint8_t  BYTE;
typedef uint16_t WORD;
typedef uint32_t DWORD;
typedef uint64_t QWORD;
typedef uint64_t PTR;

typedef int      BOOL;
#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

#define IS_ALIGNED(ptr) !((PTR)(ptr) % sizeof(PTR))

#endif
