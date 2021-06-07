#include "../lib/yEcho.h"

void *SendRecvFunc(void* _socket_fd)
{
		int socket_fd = *(int*)_socket_fd;
		char msg[1024];
		char buffer[1024];
		int recv_len;
		while(1){
				//send
				if( fgets(msg, sizeof(msg), stdin) != NULL){
						if(send(socket_fd, msg, strlen(msg), 0) < 0){
								perror("send :");
								return NULL;
						}
				}
				//recv
				if((recv_len = recv(socket_fd, buffer, 1024, 0)) < 0){
						perror("receive :");
						return (void*)-1;
				}

				buffer[recv_len] = '\0';

				printf("received msg : %s\n", buffer);
				memset(buffer, 0x00, 1024);
				memset(msg, 0x00, 1024);
		}

		return NULL;
}

int main(void)
{
		
		int sock;
		char buffer[1024];
		const char *msg = "hellow, world! from client2\n";
		int recv_len;

		pthread_t thread_send_recv;
		
		if((sock = InitClient("127.0.0.1", 7777)) < 0){
				printf("client init fail : %d, %s\n", errno, strerror(errno));
				return -1;
		}

		if(pthread_create(&thread_send_recv, NULL, SendRecvFunc, (void*)&sock) != 0){
				printf("socket : %d, %s\n", errno, strerror(errno));
				return -2;
		}

		while(1){
			poll(NULL, 0, 1000);
		}

		if((recv_len = recv(sock, buffer, 1024, 0)) < 0){
				perror("receive :");
				return -4;
		}

		buffer[recv_len] = '\0';

		printf("received msg : %s\n", buffer);

		close(sock);


		return 0;
}

