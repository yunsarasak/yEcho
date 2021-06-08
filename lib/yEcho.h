#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <pthread.h>
#include <poll.h>
#include <stdarg.h>
#include <stdlib.h>

#define BUF_SIZE 1024
#define PORT 7777

#define SEND_MSG(X, Y, ...) \
		SendMsg(X, make_message(Y, __VA_ARGS__))

pthread_mutex_t mutex_fd;

int InitClient(char* _str_dest_address, int _port);
int InitServer(int _port);
int SendMsg(int _dest_fd, char* _msg);
void PrintWhenRecv(int _socket_fd_to_monitor);

//inner function
int SetAddress(struct sockaddr_in *_st_address, char* _str_address, int _i_port_number);

char* make_message(const char *fmt, ...);
