#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <string.h>
#include <limits.h>
#include <unistd.h>
#include <sys/stat.h>
#define MAX_PATH 256

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
    int g = 0;
    if (strcmp(buffer, "get\n") == 0){
        printf("its get\n");
        g = 0;
        //return 0;
    } else if (strcmp(buffer, "set\n") == 0){
        printf("its set\n");
        g = 1;
        //return 0;
    } else {
        fprintf(stderr, "urmom\n");
        return 1;
    }

    // get file name
    char *buffer2 = malloc((MAX_PATH + 1) * sizeof(char));
    if (buffer2 == NULL) {
        printf("Error: Unable to allocate memory.\n");
        return 1;
    }

    int i = 0;
    char ch;
    while ((ch = getchar()) != '\n' && ch != EOF) {
        if (i >= MAX_PATH) {
            printf("Error: Input exceeds MAX_PATH.\n");
            free(buffer2);
            return 1;
        }
        buffer2[i++] = ch;
    }

    buffer2[i] = '\0'; // Replace newline with null byte

    printf("Buffer: %s\n", buffer2);

    if (g){
        int fd = open(buffer2, R_ONLY);
    }
}