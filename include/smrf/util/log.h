#ifndef _LOG_H
#define _LOG_H

#include <stdio.h>
#include <stdarg.h>
#include <string.h>
#include <unistd.h>

// windows users are color blind or something, I don't know
#ifdef _WIN32
# define NO_COLORS
#endif
#ifdef __CYGWIN__
# define NO_COLORS
#endif

//TODO: no colors if !isatty
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

#define MAX_LOG_LEN 4096

//TODO: ISO C99 requires at least one argument for the ‘...’ in a variadic macro
#ifdef NDEBUG
# define LOG_INFO(str, ...) \
    print_log(STDOUT_FILENO, CLR_BLUE "[INFO]: " CLR_RESET str "\n", ##__VA_ARGS__)
# define LOG_DEBUG(str, ...) \
    print_log(STDERR_FILENO, CLR_MAGENTA "[DEBUG]: " CLR_RESET str "\n", ##__VA_ARGS__)
# define LOG_WARNING(str, ...) \
    print_log(STDERR_FILENO, CLR_YELLOW "[WARNING]: " CLR_RESET str "\n", ##__VA_ARGS__)
#else
# define LOG_INFO(str, ...) print_log(STDOUT_FILENO, str "\n", ##__VA_ARGS__)
# define LOG_DEBUG(str, ...) do {} while (0)
# define LOG_WARNING(str, ...) do {} while (0)
#endif
#define LOG_ERROR(str, ...) \
    print_log(STDERR_FILENO, CLR_RED "[ERROR]: " CLR_RESET str "\n", ##__VA_ARGS__)

#define STRINGIFY(x) #x
#define TOSTRING(x) STRINGIFY(x)
#define AT CLR_WHITE __FILE__ ":" TOSTRING(__LINE__) CLR_RESET

void hex_dump(void *ptr, size_t len);
void print_log(int fd, const char *log_format, ...);

#endif
