#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>

// windows users are color blind or something, I don't know
#ifdef _WIN32
# define NO_COLORS
#endif
#ifdef __CYGWIN__
# define NO_COLORS
#endif

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

#ifdef NDEBUG
# define LOG_INFO(str, ...) \
    printf(CLR_BLUE "[INFO]: " CLR_RESET str "\n", ##__VA_ARGS__)
# define LOG_DEBUG(str, ...) \
    fprintf(stderr, CLR_MAGENTA "[DEBUG]: " CLR_RESET str "\n", ##__VA_ARGS__)
# define LOG_WARNING(str, ...) \
    fprintf(stderr, CLR_YELLOW "[WARNING]: " CLR_RESET str "\n", ##__VA_ARGS__)
#else
# define LOG_INFO(str, ...) printf(str "\n", ##__VA_ARGS__)
# define LOG_DEBUG(str, ...) do {} while (0)
# define LOG_WARNING(str, ...) do {} while (0)
#endif
#define LOG_ERROR(str, ...) \
    fprintf(stderr, CLR_RED "[ERROR]: " CLR_RESET str "\n", ##__VA_ARGS__)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT CLR_WHITE __FILE__ ":" TOSTRING(__LINE__) CLR_RESET

void hex_dump(void *ptr, size_t len);

#endif
