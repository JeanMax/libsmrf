#include "util/log.h"
#include "util/types.h"
#include <ctype.h>

bool g_verbose = true;

inline static char byte_to_char(byte b)
{
    if (isprint(b)) {
        return (char)b;
    }
    if (!b) {
        return '0';
    }
    return '.';
}

void hex_dump(void *ptr, size_t len)  // stolen from motoko <3
{
    size_t i, j, end;

    for (i = 0; i < len; i += 8) {
        end = (len - i < 8) ? len - i : 8;
        for (j = i; j < i + end; j++) {
            fprintf(stderr, "%02x ", ((byte *)ptr)[j]);
            if (!((j + 1) % 4)) {
                fprintf(stderr, " ");
            }
        }
        for (j = 8; j > end; j--) {
            fprintf(stderr, "   ");
        }
        fprintf(stderr, "   ");
        for (j = i; j < i + end; j++) {
            fprintf(stderr, "%c", byte_to_char(((byte *)ptr)[j]));
        }
        fprintf(stderr, "\n");
    }
}

//TODO: this is slow and stupid
void print_log(int fd, const char *log_format, ...)
{
    static char prev_msg[MAX_LOG_LEN];  // race-condition incoming
    char msg[MAX_LOG_LEN];
    va_list args;
    va_start(args, log_format);
    int msg_len = vsnprintf(msg, MAX_LOG_LEN, log_format, args);
    va_end(args);

#ifdef NDEBUG
    if (msg_len > MAX_LOG_LEN) {
        fprintf(stderr,
                CLR_YELLOW "[WARNING]: " CLR_RESET
                "log message too long (%d), truncating\n",
                msg_len);
    }
#endif
    if (msg_len < 0) {
        msg[0] = '\0';
    }

    if (!strcmp(msg, prev_msg)) {
        return; // spam
    }
    strcpy(prev_msg, msg);
    // TODO: add timestamp to log

    if (write(fd, msg, (size_t)msg_len) < msg_len) {
        LOG_WARNING("log.c: write fails");
    }
    if (fd == STDOUT_FILENO) {
        fflush(stdout);
    }
}
