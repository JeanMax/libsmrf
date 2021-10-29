#ifndef _TYPES_h
#define _TYPES_h

#include <stdlib.h> // size_t
#include <sys/types.h> // pid_t

// prevent your compiler from thinking you are on *nix when cygwin building
#ifdef __CYGWIN__
# define _WIN32
#endif

#include <stdint.h>
typedef uint8_t  byte;
typedef uint16_t word;
typedef uint32_t dword;
typedef uint64_t qword;
typedef uint64_t ptr_t;

typedef int      bool;
#ifndef TRUE
# define TRUE  1
# define FALSE 0
#endif

#define IS_ALIGNED(ptr) !((ptr_t)(ptr) % sizeof(dword))

#endif
