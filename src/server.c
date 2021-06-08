#include "yEcho.h"

int main(int argc, char* argv[]){
		int serv_sock, clnt_sock;
		struct sockaddr_in clnt_addr;
		struct timeval timeout;
		fd_set reads, cpy_reads;

		socklen_t addr_size;
		int fd_max, str_len, fd_num, i;

		if((serv_sock = InitServer(PORT)) < 0){
				printf("InitServer %d %s", errno, strerror(errno));
				return serv_sock;
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
										addr_size = sizeof(clnt_addr);
										clnt_sock = accept(serv_sock, (struct sockaddr*)&clnt_addr, &addr_size);
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
										str_len = recv(i, buf, BUF_SIZE, 0);
										buf[str_len] = '\0';
										if(str_len == 0) //close request
										{
												FD_CLR(i, &reads);
												close(i);
												printf("closed client %d\n", i);
										}
										else
										{
												SEND_MSG(i, "%d:%s", i, buf);		//echo
												printf("%d: recv : %s\n", i, buf);
										}
								}// if other socket is changed, read data.
						}
				}
		}

		close(serv_sock);
		return 0;



}
