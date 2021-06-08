#include "../lib/yEcho.h"

#define TOTALFORK 30

void* sendFunc(void* _sock_fd)
{
		int socket_fd = *(int*)_sock_fd;

		while(1){

				SEND_MSG(socket_fd, "hello, TCP! %d speaking", socket_fd);
				//poll(NULL, 0, 1);
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
		pthread_t thread_send, thread_recv;


		pid_t pids[TOTALFORK], pid;
		int runProcess = 0; //생성한 프로세스 수
		int state;

		while(runProcess < TOTALFORK) {
				//30개의 프로세스를 loop 를 이용하여 생성
				//자식 프로세스 종료 대기 (각 프로세스가 따로 동작하고,
				//종료를 기다려야 할 경우에 사용
				pids[runProcess] = fork();
				//fork 생성
				//0보다 작을 경우 에러 (-1)
				if(pids[runProcess] < 0) {
						return -1;
				} else if(pids[runProcess] == 0) {//자식 프로세스
						socket_fd = InitClient("127.0.0.1", PORT);

						pthread_create(&thread_send, NULL, sendFunc, (void*)&socket_fd);
						pthread_create(&thread_recv, NULL, recvFunc, (void*)&socket_fd);

						break;

				} else { //부모 프로세스
						;
				}
				runProcess++;
		}


		while(1){
				poll(NULL, 0, 1000 * 10);
		}

		close(socket_fd);

		return 0;
}



