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

char* make_message(const char *fmt, ...);

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

		while(1){
				char mybuff[256];
				char myrbuff[256];
				memset(mybuff, 0x00, sizeof(mybuff));
				char key1;
//				scanf("%c", &key1);
//				while(getchar != '\n');

				//char* msg = make_message("%s", "this is the test");

				fgets(mybuff, sizeof(mybuff), stdin);

				send(iDs, mybuff, strlen(mybuff), 0);
				recv(iDs, myrbuff, sizeof(myrbuff), 0);
				//write(iDs, mybuff, mylen);
				//iRet = read(iDs, ucBuf, sizeof(ucBuf));
				//ucBuf[iRet] = 0;
				printf("%s\n", myrbuff);
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

