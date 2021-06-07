all : client server
	make client
	make server	

client : client.c
	gcc -g -o $@ $^ -I../lib -L../lib -lpthread -lyEcho

server : server.c
	gcc -g -o $@ $^ -I../lib -L../lib -lpthread -lyEcho

ctags : 
	ctags -R ../*/*.[ch]

clean :
	rm client
	rm server
