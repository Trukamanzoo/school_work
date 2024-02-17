#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <regex.h>
#include <errno.h>
#include <fcntl.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include "asgn2_helper_funcs.h"

//dictionary of HTTP responses
void status(int code, char response[]){
    if (code == 200){
        response = "HTTP/1.1 200 OK\r\nContent-Length: ";
    } else if (code == 201){
        response = "HTTP/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n";
    } else if (code == 400){
        response = "HTTP/1.1 400 Bad Request\r\nContent-Length: 12\r\n\r\nBad Request\n";
    } else if (code == 403){
        response = "HTTP/1.1 400 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
    } else if (code == 404){
        response = "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
    } else if (code == 500){
        response = "HTTP/1.1 500 Internal Server Error\r\nContent-Length: 22\r\n\r\nInternal Server Error\n";
    } else if (code == 501) {
        response = "HTTP/1.1 501 Not Implemented\r\nContent-Length: 16\r\n\r\nNot Implemented\n";
    } else if (code == 505){
        response = "HTTP/1.1 505 Version Not Supported\r\nContent-Length: 22\r\n\r\nVersion Not Supported\n";
    }
}

void handle_get(int client_socket, char *uri){

    char file_path[65];
    sprintf(file_path, ".%s", uri);
    int file_fd = open(file_path, O_RDONLY);
    if (file_fd < 0){
        //either 404 or 403
        if (errno == ENOENT){
            //File doesn't exist
            char response[] = "";
            status(404, response);
            write_n_bytes(client_socket, response, strlen(response));
        } else if (errno == EACCES){
            //File is not accessible
            char response[] = "";
            status(403, response);
            write_n_bytes(client_socket, response, strlen(response));
        } else {
            //error occured
            char response[] = "";
            status(500, response);
            write_n_bytes(client_socket, response, strlen(response));
        }
    } else {
                
        char file_contents[2048]; // Adjust size as needed
        ssize_t len = read_n_bytes(file_fd, file_contents, sizeof(file_contents) - 1);
        file_contents[len] = '\0';
                
        char response[4096];
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n%s", len, file_contents);
        write_n_bytes(client_socket, response, strlen(response));
        close(file_fd);
    }
}

void handle_put(int client_socket, char *uri, char *message_body, int content_length){

    char file_path[65];
    sprintf(file_path, ".%s", uri);

    int file_exists = access(file_path, F_OK) != -1;

    int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
    if (file_fd < 0) {
        char *response = (errno == ENOENT)
                             ? "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n"
                             : "Http/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
        write_n_bytes(client_socket, response, strlen(response));
    } else {
        content_length = sizeof(message_body);
        ssize_t bytes = write_n_bytes(file_fd, message_body, 11);
        

        printf("bytes_wrote: %zd\n", bytes);
        char *response = file_exists
                             ? "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n"
                             : "http/1.1 201 Created\r\nContent-Length: 8\r\n\r\nCreated\n";
        write_n_bytes(client_socket, response, strlen(response));
        close(file_fd);
    }


}

void handle_request(int client_socket){
        char buffer[2048]; // Buffer to store the request
        ssize_t bytes_read = read_until(client_socket, buffer, sizeof(buffer), "\r\n\r\n");
        printf("bytes_read: %zd\n", bytes_read);
        if (bytes_read <= 0){
            char response[] = "";
            write_n_bytes(client_socket, response, strlen(response));
            return;
        }
        printf("%s\n", buffer);
        /*for (int i = 0; i < (int)strlen(buffer); i++){

            printf("%d: %c\n", i, buffer[i]);
            if (buffer[i] == '\r'){

                printf("YOOOOOOOOOOOOOOOOOOOOOOOO %d\n", i);
            }
        }*/
        
        char method[9], uri[65], version[9];
        sscanf(buffer, "%8s %64s %8s", method, uri, version);  // change this to regex


        printf("method: %s\nuri: %s\nversion: %s\n", method, uri, version);

        if (strcmp(version, "HTTP/1.1") != 0){
            // 505 unsupported version
            char response[] = "";
            status(505, response);
            write_n_bytes(client_socket, response, strlen(response));
            return;
        }

        if (strcmp(method, "GET") == 0){
            handle_get(client_socket, uri);
        } else if (strcmp(method, "PUT") == 0){
            char *message_body = strstr(buffer, "\r\r\r\n") + 4;
            int content_length = bytes_read - (message_body - buffer);
            handle_put(client_socket, uri, message_body, content_length);
        } else {
            char response[] = "";
            status(505, response);
            write_n_bytes(client_socket, response, strlen(response));
        }
        close(client_socket);
}

int main(int argc, char *argv[]){
    //printf("%lu\n", strlen("Hello World, I am foo"));
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

        int client_socket = listener_accept(&listener);
        if (client_socket == -1){
            fprintf(stderr, "ur mom\n"); //errno
            return 1;
        }

        // handle_request(client_socket)

        char buffer[2048]; // Buffer to store the request
        ssize_t bytes_read = read_until(client_socket, buffer, sizeof(buffer), "\r\n\r\n");
        printf("bytes_read: %zd\n", bytes_read);
        if (bytes_read <= 0){
            char response[] = "";
            write_n_bytes(client_socket, response, strlen(response));
            return 1;
        }
       // printf("%s\n", buffer);
        /*for (int i = 0; i < (int)strlen(buffer); i++){

            printf("%d: %c\n", i, buffer[i]);
            if (buffer[i] == '\r'){

                printf("YOOOOOOOOOOOOOOOOOOOOOOOO %d\n", i);
            }
        }*/
        
        char method[9], uri[65], version[9];
        sscanf(buffer, "%8s %64s %8s", method, uri, version);  // change this to regex


        printf("method: %s\nuri: %s\nversion: %s\n", method, uri, version);

        if (strcmp(version, "HTTP/1.1") != 0){
            // 505 unsupported version
            char response[] = "";
            status(505, response);
            write_n_bytes(client_socket, response, strlen(response));
            continue;
        }

        if (strcmp(method, "GET") == 0){
            handle_get(client_socket, uri);
        } else if (strcmp(method, "PUT") == 0){
            printf("buffer: %s\n", buffer);
            char *message_body = strstr(buffer, "\r\r\r\n");
            int content_length = bytes_read - (message_body - buffer);
            printf("content_length: %d\n", content_length);
            handle_put(client_socket, uri, message_body, content_length);
        } else {
            char response[] = "";
            status(505, response);
            write_n_bytes(client_socket, response, strlen(response));
        }
        close(client_socket);
    }

    return 0;
}
