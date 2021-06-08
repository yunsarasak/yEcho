#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <poll.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>

pthread_mutex_t mutex_fd;

char* make_message(const char *fmt, ...);

void* sendFunc(void* _sock_fd)
{
		int iDs = *(int*)_sock_fd;
		char mybuff[1024];


		while(1){

				memset(mybuff, 0x00, sizeof(mybuff));
				char* msg = make_message("%s, %d", "hello, ", iDs);
				pthread_mutex_lock(&mutex_fd);			
				send(iDs, msg, strlen(msg), 0);
				pthread_mutex_unlock(&mutex_fd);

				printf("send str!\n");
				poll(NULL, 0, 1000);
		}
}

void* recvFunc(void* _sock_fd)
{
		int iDs = *(int*)_sock_fd;
		char myrbuff[1024];
		fd_set rfds, copy_rfds;
		struct timeval tv;
		int retval;
		int recv_length;

		FD_ZERO(&rfds);
		FD_SET(iDs, &rfds);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		while(1){
				copy_rfds = rfds;
				retval = select(iDs+1, &copy_rfds, NULL, NULL, &tv);

				if(retval == -1){
						continue;
				}else if(retval == 0){
						continue;
				}else{
						memset(myrbuff, 0x00, sizeof(myrbuff));
						pthread_mutex_lock(&mutex_fd);
						recv_length = recv(iDs , myrbuff, sizeof(myrbuff), 0);
						myrbuff[recv_length] = '\0';
						pthread_mutex_unlock(&mutex_fd);

						printf("%s\n", myrbuff);	
				}
		}
}

int main()
{
		int iDs;
		int iDSize;
		int iRet;
		unsigned char ucBuf[256];

		struct sockaddr_in stAddr;

		iDSize = sizeof(struct sockaddr_in);
		bzero(&stAddr, iDSize);
		iDs = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(iDs < 0)
		{
				perror("socket failed");
				return -100;
		}
		stAddr.sin_family = AF_INET;
		iRet = inet_pton(AF_INET, "127.0.0.1", &stAddr.sin_addr.s_addr);
		if(iRet == 0)
		{
				perror("");
				return -100;
		}

		printf("IP : %s\n", inet_ntoa(stAddr.sin_addr));

		stAddr.sin_port = htons(7777);

		if( 0 > connect(iDs, (struct sockaddr *)&stAddr, iDSize))
		{
				perror("connect failed");
				close(iDs);
				return -10;
		}


		pthread_mutex_init(&mutex_fd, NULL);

		//pthread_t thread_loop;
		pthread_t thread_send, thread_recv;
		//pthread_create(&thread_loop, NULL, whileFunc, (void*)&iDs);
		pthread_create(&thread_send, NULL, sendFunc, (void*)&iDs);
		pthread_create(&thread_recv, NULL, recvFunc, (void*)&iDs);

		while(1){
				poll(NULL, 0, 1000 * 10);
		}

		close(iDs);

		return 0;
}

char* make_message(const char *fmt, ...)
{
		int n = 0;
		size_t size = 0;
		char *p = NULL;
		va_list ap;

		/* Determine required size. */

		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n < 0)
				return NULL;

		size = (size_t) n + 1;      /* One extra byte for '\0' */
		p = malloc(size);
		if (p == NULL)
				return NULL;

		va_start(ap, fmt);
		n = vsnprintf(p, size, fmt, ap);
		va_end(ap);

		if (n < 0) {
				free(p);
				return NULL;
		}

		return p;
}

