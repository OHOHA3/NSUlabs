#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include "cache.h"

#define BUFFER_SIZE 200000
#define HOSTNAME_LEN 1000
#define HEADER_LEN 1100

cache_t *cache;

void get_header(char *buffer, char *header) {
    for (int i = 0; buffer[i] != '\r'; i++) {
        header[i] = buffer[i];
    }
}

void parse_msg(const char *buffer, char *message, char *site_hostname) {
    int buf_it = 0;
    int mes_it = 0;
    int host_it = 0;

    while (buffer[buf_it] != ' ' && buffer[buf_it] != 9) {
        message[mes_it++] = buffer[buf_it++];
    }
    message[mes_it++] = buffer[buf_it++];

    buf_it += strlen("http://");

    while (buffer[buf_it] != '/') {
        site_hostname[host_it++] = buffer[buf_it++];
    }

    while (buffer[buf_it] != '\0') {
        message[mes_it++] = buffer[buf_it++];
    }
}

void *proxy_thread(void *arg) {
    int site_sock;

    struct sockaddr_in server_addr;
    struct hostent *server;
    char *buffer;
    char *message;
    char *site_hostname;
    char *header;

    buffer = (char *) calloc(BUFFER_SIZE, sizeof(char));
    message = (char *) calloc(BUFFER_SIZE, sizeof(char));
    site_hostname = (char *) calloc(HOSTNAME_LEN, sizeof(char));
    header = (char *) calloc(HEADER_LEN, sizeof(char));

    int clt_sock = *((int *) arg);

    if (read(clt_sock, buffer, BUFFER_SIZE) < 0) {
        printf("Error receiving request from client\n");
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);

        pthread_exit(NULL);
    }

    get_header(buffer, header);
    cnode_t *cnode = cache_find(cache, header);
    if (cnode != NULL) {
        printf("Founded in cache\n");
        int it = 0;
        while (1) {
            int len;
            len = get_message(cache, cnode, message, &it);
            if (len == -1) {
                break;
            }
            if (len == 0) {
                continue;
            }
            if (write(clt_sock, message, strlen(message)) == -1) {
                printf("Error sending to client\n");
                close(clt_sock);

                free(buffer);
                free(message);
                free(site_hostname);
                free(header);

                pthread_exit(NULL);
            }
        }
        printf("Proxied successfully\n");
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }

    parse_msg(buffer, message, site_hostname);

    server = gethostbyname(site_hostname);
    if (server == NULL) {
        printf("Cannot find server\n");
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }
    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);
    site_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (site_sock < 0) {
        printf("Error opening proxy client socket\n");
        close(site_sock);
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }
    if (connect(site_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Cannot connect to server\n");
        close(site_sock);
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }

    if (write(site_sock, message, strlen(message)) == -1) {
        printf("Error sending to server\n");
        close(site_sock);
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }

    cnode_t *c = cache_add(cache, header);
    set_filled(c, 0);
    printf("add to cache\n");

    memset(message, 0, sizeof(char) * BUFFER_SIZE);
    memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
    int isReading = read(site_sock, buffer, BUFFER_SIZE);
    while (isReading > 0) {
        node_fill(cache, c, buffer);
        int isWriting = write(clt_sock, buffer, isReading);
        if (isWriting < 0) {
            printf("Error sending to client\n");
            close(site_sock);
            close(clt_sock);

            free(buffer);
            free(message);
            free(site_hostname);
            free(header);

            pthread_exit(NULL);
        }
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        isReading = read(site_sock, buffer, BUFFER_SIZE);
    }
    if (isReading < 0) {
        printf("Error reading from socket\n");
        close(site_sock);
        close(clt_sock);

        free(buffer);
        free(message);
        free(site_hostname);
        free(header);

        pthread_exit(NULL);
    }
    set_filled(c, 1);

    close(site_sock);
    close(clt_sock);

    free(buffer);
    free(message);
    free(site_hostname);
    free(header);

    printf("Proxied successfully\n");
    pthread_exit(NULL);
}

int main(int argc, char *argv[]) {
   /* if (argc < 2) {
        printf("Error, no port provided\n");
        exit(0);
    }*/

    cache = cache_init(4);

    int proxy_port;
    int proxy_sock;
    struct sockaddr_in proxy_sockaddr;

    proxy_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (proxy_sock == -1) {
        printf("Error opening proxy listening socket\n");
        exit(0);
    }

    //proxy_port = atoi(argv[1]);
    proxy_port = 9012;
    memset(&proxy_sockaddr, 0, sizeof(proxy_sockaddr));
    proxy_sockaddr.sin_family = AF_INET;
    proxy_sockaddr.sin_addr.s_addr = INADDR_ANY;
    proxy_sockaddr.sin_port = htons(proxy_port);

    if (bind(proxy_sock, (struct sockaddr *) &proxy_sockaddr, sizeof(proxy_sockaddr)) == -1) {
        printf("Binding error\n");
        exit(0);
    }
    if (listen(proxy_sock, 50) == -1) {
        printf("Listen error\n");
        exit(0);
    }

    printf("Proxy started\n");

    while (1) {
        struct sockaddr_in clt_sockaddr;
        socklen_t client_len = sizeof(struct sockaddr);
        memset(&clt_sockaddr, 0, sizeof(clt_sockaddr));

        int clt_sock = accept(proxy_sock, (struct sockaddr *) &clt_sockaddr, &client_len);
        if (clt_sock == -1) {
            printf("Error accept\n");
            close(clt_sock);
        }
        printf("Client connected - %s:%d\n", inet_ntoa(clt_sockaddr.sin_addr), clt_sockaddr.sin_port);

        pthread_t tid;

        if (pthread_create(&tid, NULL, proxy_thread, &clt_sock)) {
            printf("Error creating thread\n");
        }
    }
}