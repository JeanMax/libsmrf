#include "util/log.h"
#include "util/types.h"

inline static char byte_to_char(byte b)
{
	return (b >= 127 || b < 32) ? '.' : (char)b;
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
