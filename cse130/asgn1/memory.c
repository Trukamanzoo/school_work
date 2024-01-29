#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>

ssize_t write_n(int fd, const void *buf, size_t n) {
    size_t total = 0;
    const char *buffer = buf;

    while (total < n) {
        ssize_t written = write(fd, buffer + total, n - total);
        if (written <= 0) {
            // Handle the error here. For simplicity, we just return with an error.
            return -1;
        }
        total += written;
    }

    return total;
}

ssize_t read_n(int fd, void *buf, size_t n) {
    size_t total = 0;
    char *buffer = buf;

    while (total < n) {
        ssize_t bytes_read = read(fd, buffer + total, n - total);
        if (bytes_read < 0) {
            // Handle the error here. For simplicity, we just return with an error.
            return -1;
        }
        if (bytes_read == 0) {
            // We've hit EOF.
            break;
        }
        total += bytes_read;
    }

    return total;
}

int main() {
    char buffer[4];
    ssize_t bytes_read;
    bytes_read = read(STDIN_FILENO, buffer, 4);
    printf("%s", buffer);
    if (strcmp(buffer, "get\n") == 0){
        printf("its get\n");
        //return 0;
    } else if (strcmp(buffer, "set\n") == 0){
        printf("its set\n");
        //return 0;
    } else {
        fprintf(stderr, "urmom\n");
        return 1;
    }

    // get file name

    if (bytes_read == -1) {
        // Handle the error here. For simplicity, we just return with an error.
        return -1;
    }
}