#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <unistd.h>

#define BUFFER_SIZE 512
#define REQUEST_SIZE 256
#define ZIP_CODE_SIZE 10

typedef struct {
    char *ptr;
    size_t len;
} String;

void initialize_string(String *s) {
    s->len = 0;
    s->ptr = malloc(s->len + 1);
    if (s->ptr == NULL) {
        perror("Memory allocation failed");
        exit(EXIT_FAILURE);
    }
    s->ptr[0] = '\0';
}

void append_to_string(String *s, char *buffer, size_t size) {
    size_t new_len = s->len + size;
    s->ptr = realloc(s->ptr, new_len + 1);
    if (s->ptr == NULL) {
        perror("Memory reallocation failed");
        exit(EXIT_FAILURE);
    }
    memcpy(s->ptr + s->len, buffer, size);
    s->ptr[new_len] = '\0';
    s->len = new_len;
}

int create_socket_and_connect(struct addrinfo *res) {
    int sockfd = socket(res->ai_family, res->ai_socktype, res->ai_protocol);
    if (sockfd == -1) {
        perror("Socket creation failed");
        exit(EXIT_FAILURE);
    }
    if (connect(sockfd, res->ai_addr, res->ai_addrlen) == -1) {
        perror("Connection failed");
        exit(EXIT_FAILURE);
    }
    return sockfd;
}

void send_request(int sockfd, char *zip) {
    char request[REQUEST_SIZE];
    sprintf(request, "GET /ws/%s/json/ HTTP/1.1\r\nHost: viacep.com.br\r\nConnection: close\r\n\r\n", zip);
    if (send(sockfd, request, strlen(request), 0) == -1) {
        perror("Request sending failed");
        exit(EXIT_FAILURE);
    }
}

void receive_response(int sockfd, String *s) {
    char buffer[BUFFER_SIZE];
    ssize_t bytes_received;
    while ((bytes_received = recv(sockfd, buffer, sizeof(buffer), 0)) > 0) {
        append_to_string(s, buffer, bytes_received);
    }
    if (bytes_received == -1) {
        perror("Response reception failed");
        exit(EXIT_FAILURE);
    }
}

void get_address_from_zip_code(char *zip) {
    struct addrinfo hints, *res;
    memset(&hints, 0, sizeof(hints));
    hints.ai_family = AF_UNSPEC;
    hints.ai_socktype = SOCK_STREAM;
    if (getaddrinfo("viacep.com.br", "80", &hints, &res) != 0) {
        perror("getaddrinfo");
        exit(EXIT_FAILURE);
    }
    int sockfd = create_socket_and_connect(res);
    send_request(sockfd, zip);
    String s;
    initialize_string(&s);
    receive_response(sockfd, &s);
    printf("%s\n", s.ptr);
    free(s.ptr);
    close(sockfd);
    freeaddrinfo(res);
}

int main(void) {
    char zip[ZIP_CODE_SIZE];
    printf("Enter Zip Code: ");
    scanf("%s", zip);
    get_address_from_zip_code(zip);
    return 0;
}
