#include "yEcho.h"


//summary : return socket fd when success
//param : destination ipv4 address, port
//return : socket fd on success, negative number on fail
int InitClient(char* _str_dest_address, int _port)
{
		int socket_fd;
		struct sockaddr_in st_address;

		if((socket_fd = socket(AF_INET, SOCK_STREAM, 0)) < 0 ){
				perror("socket :");
				return -1;
		}

		SetAddress(&st_address, _str_dest_address, _port);

		if(connect(socket_fd, (struct sockaddr *)&st_address, sizeof(struct sockaddr_in)) < 0){
				printf("errno : %d, %s\n", errno, strerror(errno));
				return -2;
		}

		return socket_fd;
}

//summary : return socket fd when success
//param :  port
//return : socket fd on success, negative number on fail
int InitServer(int _port)
{
		int socket_fd;
		struct sockaddr_in addr;
		if((socket_fd = socket(AF_INET, SOCK_STREAM, 0))<0){
			   	perror("socket_fdet "); return 1;
	   	} 
		
		SetAddress(&addr, NULL, 7777);


		if(bind(socket_fd, (struct sockaddr *)&addr, sizeof(addr)) < 0){ 
				perror("bind "); return 1;
	   	} 

		if(listen(socket_fd, 5) < 0){
			   	perror("listen "); 
				return 1; 
		}

		return socket_fd;
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
