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
	srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
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
	struct sockaddr_in clt_sockaddr;
	socklen_t len = sizeof(struct sockaddr);
	while (1) {
		int text_len;
		char buf[64];
		text_len = recvfrom(srv_sock, buf, sizeof(buf), 0, (struct sockaddr*)&clt_sockaddr, &len);
		text_len = sendto(srv_sock, buf, text_len, 0, (struct sockaddr*)&clt_sockaddr, len);
	}
	close(srv_sock);
	return 0;
}