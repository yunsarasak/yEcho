#include "../lib/yEcho.h"

void* sendFunc(void* _sock_fd)
{
		int socket_fd = *(int*)_sock_fd;

		while(1){

				SEND_MSG(socket_fd, "hello, TCP! %d speaking", socket_fd);
				poll(NULL, 0, 1000);
		}
}

void* recvFunc(void* _sock_fd)
{
		int socket_fd = *(int*)_sock_fd;

		PrintWhenRecv(socket_fd);

}

int main()
{
		int socket_fd;
		socket_fd = InitClient("127.0.0.1", PORT);

		pthread_t thread_send, thread_recv;

		pthread_create(&thread_send, NULL, sendFunc, (void*)&socket_fd);
		pthread_create(&thread_recv, NULL, recvFunc, (void*)&socket_fd);

		while(1){
				poll(NULL, 0, 1000 * 10);
		}

		close(socket_fd);

		return 0;
}



