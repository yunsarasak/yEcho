#include "yEcho.h"


//summary : return socket fd when success
//param : destination ipv4 address, port
//return : socket fd on success, negative number on fail
int InitClient(char* _str_dest_address, int _port)
{
		int socket_fd;
		int iDSize;
		int iRet;

		pthread_mutex_init(&mutex_fd, NULL);

		struct sockaddr_in stAddr;

		iDSize = sizeof(struct sockaddr_in);
		bzero(&stAddr, iDSize);
		socket_fd = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
		if(socket_fd < 0)
		{
				printf("socket %d %s", errno, strerror(errno));
				return -1;
		}
		stAddr.sin_family = AF_INET;
		iRet = inet_pton(AF_INET, _str_dest_address, &stAddr.sin_addr.s_addr);
		if(iRet == 0)
		{
				printf("pton %d %s", errno, strerror(errno));
				return -2;
		}

		printf("IP : %s\n", inet_ntoa(stAddr.sin_addr));

		stAddr.sin_port = htons(_port);

		if( 0 > connect(socket_fd, (struct sockaddr *)&stAddr, iDSize))
		{
				printf("socket %d %s", errno, strerror(errno));
				close(socket_fd);
				return -3;
		}

		return socket_fd;

}

//summary : return socket fd when success
//param :  port
//return : socket fd on success, negative number on fail
int InitServer(int _port)
{
		int serv_sock;
		struct sockaddr_in serv_addr;
		pthread_mutex_init(&mutex_fd, NULL);

		socklen_t adr_sz;
		int fd_max, str_len, fd_num, i;

		//src address init. ipv4, pass (0.0.0.0) get any lancard address, use 7777 port
		serv_addr.sin_family = AF_INET;
		serv_addr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_addr.sin_port = htons(_port);
		//if socket stream type is 0, automatically apply 
		serv_sock = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);

		//bind src addr to server socket
		if(bind(serv_sock, (struct sockaddr*)&serv_addr, sizeof(serv_addr)) == -1){
				printf("bind %d %s", errno, strerror(errno));
				return -1;
		}

		if(listen(serv_sock, 5) == -1){
				printf("listen %d %s", errno, strerror(errno));
				return -2;
		}

		return serv_sock;

}


//summary : send string with format to dest socket. 
//param : socket descriptor, format specifier , args
//return : return 0 on suceess, -1 on error
int SendMsg(int _dest_fd, char *_msg)
{
	int send_result = -1;
	pthread_mutex_lock(&mutex_fd);			
	send_result = send(_dest_fd, _msg, strlen(_msg), 0);
	pthread_mutex_unlock(&mutex_fd);

	return send_result;

}

void PrintWhenRecv(int _socket_fd_to_monitor)
{
		char buff[BUF_SIZE];
		fd_set rfds, copy_rfds;
		struct timeval tv;
		int retval;
		int recv_length;

		FD_ZERO(&rfds);
		FD_SET(_socket_fd_to_monitor, &rfds);

		tv.tv_sec = 5;
		tv.tv_usec = 0;

		while(1){
				copy_rfds = rfds;
				retval = select(_socket_fd_to_monitor+1, &copy_rfds, NULL, NULL, &tv);

				if(retval == -1){
						continue;
				}else if(retval == 0){
						continue;
				}else{
						memset(buff, 0x00, sizeof(buff));
						pthread_mutex_lock(&mutex_fd);
						recv_length = recv(_socket_fd_to_monitor , buff, sizeof(buff), 0);
						pthread_mutex_unlock(&mutex_fd);
						buff[recv_length] = '\0';

						printf("%s\n", buff);	
				}
		}

		return;
}


//inner fucntion
int SetAddress(struct sockaddr_in *_st_address, char* _str_address, int _i_port_number)
{
		memset(_st_address, 0x00, sizeof(struct sockaddr_in));
		_st_address->sin_family = AF_INET;
		if(_str_address == NULL){
				_st_address->sin_addr.s_addr = htonl(INADDR_ANY);
		}else{
				_st_address->sin_addr.s_addr = inet_addr(_str_address);
		}
		_st_address->sin_port = htons(_i_port_number);

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
