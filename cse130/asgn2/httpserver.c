#include "asgn2_helper_funcs.h" // Assuming this is the header file containing the helper functions and structs
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

int main(int argc, char *argv[]) {
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
    fprintf(stderr, "Failed to initialize listener\n");
    return 1;
  }

  while (1) {
    int client_fd = listener_accept(&listener);
    if (client_fd == -1) {
      fprintf(stderr, "Failed to accept connection\n");
      continue;
    }

    char buffer[2048]; // Buffer to store the request
    ssize_t bytes_read =
        read_until(client_fd, buffer, sizeof(buffer), "\r\n\r\n");

    if (bytes_read <= 0) {
      
      continue;
    }

    // Parse the request
    char method[9], uri[65], version[9];
    sscanf(buffer, "%8s %64s %8s", method, uri, version);

    // Ensure the HTTP version is 1.1
    if (strcmp(version, "HTTP/1.1") != 0) {
      // Send a 505 HTTP Version Not Supported response
      char response[] =
          "HTTP/1.1 505 HTTP Version Not Supported\r\nContent-Length: "
          "27\r\n\r\nHTTP Version Not Supported\n";
      write_n_bytes(client_fd, response, strlen(response));
      continue;
    }
    // Inside the if-else block where we check the method

    if (strcmp(method, "GET") == 0) {
      // Handle the GET request
      char file_path[65];
      sprintf(file_path, ".%s", uri); // Assuming the server is running in the
                                      // directory with the files

      int file_fd = open(file_path, O_RDONLY);
      if (file_fd < 0) {
        // Send a 404 Not Found or 403 Forbidden response depending on the error
        if (errno == ENOENT) {
          char response[] =
              "HTTP/1.1 404 Not Found\r\nContent-Length: 10\r\n\r\nNot Found\n";
          write_n_bytes(client_fd, response, strlen(response));
        } else {
          char response[] =
              "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
          write_n_bytes(client_fd, response, strlen(response));
        }
      } else {
        // Read the file contents and send a 200 OK response with the file
        // contents as the message body
        char file_contents[2048]; // Adjust size as needed
        ssize_t len =
            read_n_bytes(file_fd, file_contents, sizeof(file_contents) - 1);
        file_contents[len] = '\0';

        // Send the response
        char response[4096]; // Adjust size as needed
        sprintf(response, "HTTP/1.1 200 OK\r\nContent-Length: %zu\r\n\r\n%s",
                len, file_contents);
        write_n_bytes(client_fd, response, strlen(response));

        close(file_fd);
      }
    } else if (strcmp(method, "PUT") == 0) {
      // Handle the PUT request
      char file_path[65];
      sprintf(file_path, ".%s", uri); // Assuming the server is running in the
                                      // directory with the files

      int file_fd = open(file_path, O_WRONLY | O_CREAT | O_TRUNC, 0644);
      if (file_fd < 0) {
        // Send a 403 Forbidden response
        char response[] =
            "HTTP/1.1 403 Forbidden\r\nContent-Length: 10\r\n\r\nForbidden\n";
        write_n_bytes(client_fd, response, strlen(response));
      } else {
        // Write the message body to the file and send a 200 OK or 201 Created
        // response
        char *body = strstr(buffer, "\r\n\r\n") +
                     4; // Find the start of the message body
        write_n_bytes(file_fd, body, strlen(body));

        // Send the response
        char response[4096]; // Adjust size as needed
        sprintf(response,
                "HTTP/1.1 200 OK\r\nContent-Length: 3\r\n\r\nOK\n"); // Or 201
                                                                     // Created
        write_n_bytes(client_fd, response, strlen(response));

        close(file_fd);
      }
    }
    return 0;
  }
}
