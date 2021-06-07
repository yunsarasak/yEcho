#include <stdio.h>
#include <unistd.h>
#include <sys/select.h>
#include <sys/time.h>
#include <sys/socket.h>
#include <netinet/in.h>

#define BUF_SIZE 1024
#define PORT 7777

int main(int argc, char* argv[]){
		int serv_sock, clnt_sock;
		struct sockaddr_in serv_adr, clnt_adr;
		struct timeval timeout;
		fd_set reads, cpy_reads;

		socklen_t adr_sz;
		int fd_max, str_len, fd_num, i;

		//codes
		serv_adr.sin_family = AF_INET;
		serv_adr.sin_addr.s_addr = htonl(INADDR_ANY);
		serv_adr.sin_port = htons(PORT);

		serv_sock = socket(AF_INET, SOCK_STREAM, 0);

		//codes

		if(bind(serv_sock, (struct sockaddr*)&serv_adr, sizeof(serv_adr)) == -1){
				printf("bind err\n");
		}

		if(listen(serv_sock, 5) == -1){
				printf("listen err\n");
		}

		FD_ZERO(&reads);// initialize fd_set
		FD_SET(serv_sock, &reads);// add server socket into read fd set
		fd_max = serv_sock;//max file descriptor

		while(1)
		{
				cpy_reads = reads;		//copy origin
				timeout.tv_sec = 5;
				timeout.tv_usec = 5000;	//set timeout

				if((fd_num = select(fd_max+1, &cpy_reads, 0, 0, &timeout)) == -1){
						break;
				}//there is only server socket in the set yet, so when receive connect request, data insert into server socket

				if(fd_num == 0){
						continue;
				}// if time out, continue

				for(i = 0; i < fd_max+1; i++){
						if(FD_ISSET(i, &cpy_reads))
						{
								if( i == serv_sock)
								{
										adr_sz = sizeof(clnt_adr);
										clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_adr, &adr_sz);
										FD_SET(clnt_sock, &reads);
										if(fd_max < clnt_sock)
										{
												fd_max = clnt_sock;
										}
										printf("connect client %d\n", clnt_sock);
								}// if server socket is changed , it means there is connect request
								else
								{
										char buf[BUF_SIZE];
										str_len = read(i, buf, BUF_SIZE);
										if(str_len == 0) //close request
										{
												FD_CLR(i, &reads);
												close(i);
												printf("closed client %d\n", i);
										}
										else
										{
												write(i, buf, str_len);	//echo
										}
								}// if other socket is changed, read data.
						}
				}
		}

		close(serv_sock);
		return 0;



}
