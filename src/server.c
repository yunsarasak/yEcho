#include "../lib/yEcho.h"

int main(void){
		int sock, client_sock;
		struct sockaddr_in addr, client_addr;
		char buffer[1024];
		int len, addr_len, recv_len;

		sock = InitServer(7777);

		addr_len = sizeof(client_addr);

		printf("waiting for client..\n");
		

		while((client_sock = accept(sock, (struct sockaddr *)&client_addr, &addr_len)) > 0){
				printf("client ip : %s\n", inet_ntoa(client_addr.sin_addr));
				
				while(1){
						if((recv_len = recv(client_sock, buffer, 1024, 0)) < 0){
								printf("recv : %d, %s\n", errno, strerror(errno));
								return -4;
						}

						buffer[recv_len] = '\0';
						printf("received data : %s\n", buffer);
						send(client_sock, buffer, strlen(buffer), 0);
				}

				close(client_sock);
		}

		close(sock);


		return 0;
}
