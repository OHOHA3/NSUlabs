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
	struct sockaddr_in srv_sockaddr;
	memset(&srv_sockaddr, 0, sizeof(srv_sockaddr));
	srv_sockaddr.sin_family = AF_INET;
	srv_sockaddr.sin_port = htons(PORT);
	srv_sockaddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (connect(srv_sock, (struct sockaddr*)&srv_sockaddr, sizeof(srv_sockaddr)) == -1) {
		perror("connect");
		exit(0);
	}
	struct sockaddr_in clt_sockaddr;
	socklen_t len = sizeof(struct sockaddr);
	while (1) {
		char buf[64];
		scanf("%s", buf);
		sendto(srv_sock, buf, strlen(buf), 0, (struct sockaddr*)&srv_sockaddr, len);
		recvfrom(srv_sock, buf, sizeof(buf), 0, (struct sockaddr*)&srv_sockaddr, &len);
		printf("%s\n", buf);
	}
	close(srv_sock);
	return 0;
}