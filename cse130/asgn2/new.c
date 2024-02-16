#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <regex.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "asgn2_helper_funcs.h"
/*
void handle_get(Listener_Socket *sock, char *uri);

void handle_put();

void handle_request();

void parse_urmom();*/



int main(int argc, char *argv[]){
    printf("%lu\n", strlen("Hello World, I am foo"));
    if (argc != 2) {
        fprintf(stderr, "Invalid arguments. Usage: ./httpserver <port>\n");
        return 1;
    }

    int port = atoi(argv[1]);
    if (port < 1 || port > 65535) {
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }

    Listener_Socket listener;
    if (listener_init(&listener, port) == -1) {
        fprintf(stderr, "Invalid Port\n");
        return 1;
    }

    while(1){

        int client_fd = listener_accept(&listener);
        if (client_fd == -1){
            fprintf(stderr, "ur mom\n"); //errno
            return 1;
        }

        char buffer[2048]; // Buffer to store the request
        ssize_t bytes_read = read_until(client_fd, buffer, sizeof(buffer), "\r\n\r\n");
        if (bytes_read <= 0){
            continue;
        }
        //printf("%.20s\n", buffer);
        for (int i = 0; i < (int)strlen(buffer); i++){

            printf("%d: %c\n", i, buffer[i]);
            if (buffer[i] == '\r'){

                printf("YOOOOOOOOOOOOOOOOOOOOOOOO %d\n", i);
            }
        }
        
        char method[9], uri[65], version[9];
        sscanf(buffer, "%8s %64s %8s", method, uri, version);  // change this to regex


        printf("method: %s || uri: %s || version: %s\n", method, uri, version);

        if (strcmp(version, "HTTP/1.1") != 0){
            
        }
    }

    return 0;
}
