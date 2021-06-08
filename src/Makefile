server : server.c
	gcc -g -o $@ $^ -I../lib -L../lib -lpthread -lyEcho

client : client.c
	gcc -g -o $@ $^ -I../lib -L../lib -lpthread -lyEcho

ctags :
	ctags ../*/*.[ch]

clean :
	rm server
