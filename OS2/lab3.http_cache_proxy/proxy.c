#include <stdlib.h>
#include <stdio.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <pthread.h>
#include <signal.h>
#include "cache.h"

#define BUFFER_SIZE 200000
#define HOSTNAME_LEN 1000
#define HEADER_LEN 1100

cache_t *cache;

void *proxy_thread(void *arg);

void listener(int sig);

void clean_data(int clt_sock, char *buffer, char *message, char *site_hostname, char *header);

void get_header(char *buffer, char *header);

int send_from_cache(char *buffer, int clt_sock, cnode_t *cnode);

void parse_msg(const char *buffer, char *message, char *site_hostname);

int connect_to_site(char *site_hostname);

int send_from_site(char *header, char *buffer, int site_sock, int clt_sock);

int main(int argc, char *argv[]) {
    if (argc < 2) {
        printf("Error, no port provided\n");
        exit(0);
    }

    cache = cache_init(20);

    int proxy_port;
    int proxy_sock;
    struct sockaddr_in proxy_sockaddr;

    proxy_sock = socket(AF_INET, SOCK_STREAM, 0);
    if (proxy_sock == -1) {
        printf("Error opening proxy listening socket\n");
        exit(0);
    }
    setsockopt(proxy_sock, SOL_SOCKET, SO_REUSEPORT, &(int) {1}, sizeof(int));

    proxy_port = atoi(argv[1]);

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

void *proxy_thread(void *arg) {
    signal(SIGPIPE, listener);

    char *buffer = (char *) calloc(BUFFER_SIZE, sizeof(char));
    char *message = (char *) calloc(BUFFER_SIZE, sizeof(char));
    char *site_hostname = (char *) calloc(HOSTNAME_LEN, sizeof(char));
    char *header = (char *) calloc(HEADER_LEN, sizeof(char));

    int clt_sock = *((int *) arg);
    int site_sock;

    if (read(clt_sock, buffer, BUFFER_SIZE) < 0) {
        printf("Error receiving request from client\n");
        clean_data(clt_sock, buffer, message, site_hostname, header);
        pthread_exit(NULL);
    }

    get_header(buffer, header);

    cnode_t *cnode = cache_find(cache, header);
    if (cnode != NULL) {
        printf("Founded in cache\n");

        if (send_from_cache(buffer, clt_sock, cnode)) {
            printf("Proxied successfully\n");
        }

        clean_data(clt_sock, buffer, message, site_hostname, header);
        pthread_exit(NULL);
    }

    parse_msg(buffer, message, site_hostname);

    site_sock = connect_to_site(site_hostname);
    if (site_sock == -1) {
        clean_data(clt_sock, buffer, message, site_hostname, header);
        pthread_exit(NULL);
    }

    if (write(site_sock, message, strlen(message)) == -1) {
        printf("Error sending to server\n");
        close(site_sock);
        clean_data(clt_sock, buffer, message, site_hostname, header);
        pthread_exit(NULL);
    }

    if (send_from_site(header, buffer, site_sock, clt_sock) == -1) {
        clean_data(clt_sock, buffer, message, site_hostname, header);
        close(site_sock);
        pthread_exit(NULL);
    }

    clean_data(clt_sock, buffer, message, site_hostname, header);
    close(site_sock);

    printf("Proxied successfully\n");
    pthread_exit(NULL);
}

void listener(int sig) {
    signal(sig, SIG_IGN);
}

void clean_data(int clt_sock, char *buffer, char *message, char *site_hostname, char *header) {
    close(clt_sock);

    free(buffer);
    free(message);
    free(site_hostname);
    free(header);
}

void get_header(char *buffer, char *header) {
    for (int i = 0; buffer[i] != '\r'; i++) {
        header[i] = buffer[i];
    }
}

int send_from_cache(char *buffer, int clt_sock, cnode_t *cnode) {
    int it = 0;
    while (1) {
        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        int len;
        len = get_message(cnode, buffer, &it, BUFFER_SIZE);
        if (len == -1) {
            break;
        }
        if (len == 0) {
            continue;
        }
        if (write(clt_sock, buffer, len) == -1) {
            printf("Error sending to client\n");
            return -1;
        }
    }
    return 1;
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

int connect_to_site(char *site_hostname) {
    struct sockaddr_in server_addr;
    struct hostent *server;
    int site_sock;

    server = gethostbyname(site_hostname);
    if (server == NULL) {
        printf("Cannot find server\n");
        return -1;
    }

    memset((char *) &server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(80);
    bcopy((char *) server->h_addr, (char *) &server_addr.sin_addr.s_addr, server->h_length);

    site_sock = socket(PF_INET, SOCK_STREAM, 0);
    if (site_sock < 0) {
        printf("Error opening proxy site socket\n");
        close(site_sock);
        return -1;
    }

    if (connect(site_sock, (struct sockaddr *) &server_addr, sizeof(server_addr)) == -1) {
        printf("Cannot connect to site\n");
        close(site_sock);
        return -1;
    }

    return site_sock;
}

int send_from_site(char *header, char *buffer, int site_sock, int clt_sock) {
    cnode_t *c = cache_add(cache, header);
    set_filled(c, 0);
    printf("add to cache\n");

    memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
    int isReading = read(site_sock, buffer, BUFFER_SIZE);
    while (isReading > 0) {
        node_fill(c, buffer, isReading);

        int isWriting = write(clt_sock, buffer, isReading);
        if (isWriting < 0) {
            printf("Error sending to client\n");
            set_filled(c, 1);
            return -1;
        }

        memset(buffer, 0, sizeof(char) * BUFFER_SIZE);
        isReading = read(site_sock, buffer, BUFFER_SIZE);
    }
    if (isReading < 0) {
        printf("Error reading from socket\n");
        set_filled(c, 1);
        return -1;
    }
    set_filled(c, 1);
    return 1;
}
