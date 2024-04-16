#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <string.h>

#define PORT 25625

int main() {
	int srv_sock;
	int clt_sock;
	int pid;
	srv_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (srv_sock == -1) {
		perror("socket");
		exit(0);
	}
	struct sockaddr_in srv_sockaddr;
	memset(&srv_sockaddr, 0, sizeof(srv_sockaddr));
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_port = htons(PORT);
	srv_sockaddr.sin_addr.s_addr = INADDR_ANY;
	if (bind(srv_sock, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		perror("bind");
		close(srv_sock);
		exit(0);
	}
	if (listen(srv_sock, 5) == -1) {
		perror("listen");
		close(srv_sock);
		exit(0);
	}
	struct sockaddr_in clt_sockaddr;
	socklen_t len = sizeof(struct sockaddr);
	while (1) {
		memset(&clt_sockaddr, 0, sizeof(clt_sockaddr));
		clt_sock = accept(srv_sock, (struct sockaddr*)&clt_sockaddr, &len);
		if (clt_sock == -1) {
			perror("accept");
			close(clt_sock);
			exit(0);
		}
		else {
			printf("Client - %s:%d\n", inet_ntoa(clt_sockaddr.sin_addr), clt_sockaddr.sin_port);
		}
		pid = fork();
		if (!pid) {
			break;
		}
		close(clt_sock);
	}
	close(srv_sock);
	while (1) {
		int len;
		char buf[64];
		len = read(clt_sock, buf, sizeof(buf));
		len = write(clt_sock, buf, len);
	}
	return 0;
}