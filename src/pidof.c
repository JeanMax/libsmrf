#include "seed.h"
#include <dirent.h>

#define MAX_BUF 1024
#define MAX_PATH 128

pid_t pidof(const char*pname)
{
    DIR *proc_dir;
    FILE *comm_file;
    struct dirent *entry;
    char path[MAX_PATH], read_buf[MAX_BUF];

    proc_dir = opendir("/proc/");
    if (!proc_dir) {
        perror("Fail");
        return 0;
    }

    while ((entry = readdir(proc_dir))) {
        strcpy(path, "/proc/");
        strcat(path, entry->d_name);
        strcat(path, "/comm");

        comm_file = fopen(path, "r");
        if (comm_file) {
            fscanf(comm_file, "%s", read_buf);
            if (!strcmp(read_buf, pname)) {
                int ret = atoi(entry->d_name);
                fclose(comm_file);
                closedir(proc_dir);
                return ret;
            }
            fclose(comm_file);
        }
    }

    closedir(proc_dir);
    return 0;
}
