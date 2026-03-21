/* dir_list_sys.c */
#include <fcntl.h>
#include <unistd.h>
#include <dirent.h>
#include <sys/stat.h>
#include <string.h>
#include <stdio.h>    // only for snprintf to format numbers into strings

int main() {
    char buffer[512];
    // YOUR CODE HERE
    // 1. Open current directory with opendir(".")
    DIR *dir = opendir(".");
    if (dir == NULL) {
         write(1,"Error opening directory\n", 24);
        return 1;
    };
    // 2. Print header line using write()
    write(1, "Filename                       Size (bytes)\n", 45);
    write(1, "------------------------------ ----------\n", 43);

    struct dirent *entry;
    struct stat st;

    while ((entry = readdir(dir)) != NULL) {
        
        if (stat(entry->d_name, &st) == -1) {
            int n = snprintf(buffer, sizeof(buffer), "%-30s %10s\n", entry->d_name, "?");
            if (n > 0) write(1, buffer, (size_t)n);
            continue;
        }

        int n = snprintf(buffer, sizeof(buffer), "%-30s %10ld\n", entry->d_name,st.st_size);
        if (n > 0) write(1, buffer, (size_t)n);
    }

    closedir(dir);
    return 0;
}