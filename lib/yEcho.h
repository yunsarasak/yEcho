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


int InitClient(char* _str_dest_address, int _port);
int InitServer(int _port);


//inner function
int SetAddress(struct sockaddr_in *_st_address, char* _str_address, int _i_port_number);

