#ifndef _SEED_H
#define _SEED_H

#define _GNU_SOURCE

#include "d2structs.h"

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#ifdef NO_COLORS
# define CLR_BLACK ""
# define CLR_RED ""
# define CLR_GREEN ""
# define CLR_YELLOW ""
# define CLR_BLUE ""
# define CLR_MAGENTA ""
# define CLR_CYAN ""
# define CLR_WHITE ""
# define CLR_RESET ""
#else
# define CLR_BLACK   "\033[30;01m"
# define CLR_RED     "\033[31;01m"
# define CLR_GREEN   "\033[32;01m"
# define CLR_YELLOW  "\033[33;01m"
# define CLR_BLUE    "\033[34;01m"
# define CLR_MAGENTA "\033[35;01m"
# define CLR_CYAN    "\033[36;01m"
# define CLR_WHITE   "\033[37;01m"
# define CLR_RESET   "\033[0m"
#endif

#ifdef DEBUG_MODE
# define LOG_DEBUG(str, ...) \
    fprintf(stderr, CLR_MAGENTA "[DEBUG]: " CLR_RESET str CLR_RESET "\n", ##__VA_ARGS__)
#else
# define LOG_DEBUG(str, ...) do {} while (0)
#endif
#define LOG_INFO(str, ...) \
    fprintf(stdout, CLR_BLUE "[INFO]: " CLR_RESET str CLR_RESET "\n", ##__VA_ARGS__)
#define LOG_WARNING(str, ...) \
    fprintf(stderr, CLR_YELLOW "[WARNING]: " CLR_RESET str CLR_RESET "\n", ##__VA_ARGS__)
#define LOG_ERROR(str, ...) \
    fprintf(stderr, CLR_RED "[ERROR]: " CLR_RESET str CLR_RESET "\n", ##__VA_ARGS__)


#define MIN(a, b) ((a) > (b) ? (b) : (a))
#define MAX(a, b) ((a) > (b) ? (a) : (b))
#define ABS(x)    ((x) < 0 ? -(x) : (x))

#include <byteswap.h>
/* #define SWAP16(x) ((x >> 8) | (x << 8)) */

/* #define SWAP32(x) (((x >> 24) & 0x000000ff)     \ */
/*                  | ((x << 8)  & 0x00ff0000)     \ */
/*                  | ((x >> 8)  & 0x0000ff00)     \ */
/*                  | ((x << 24) & 0xff000000)) */




pid_t pidof(const char*pname);

#endif
