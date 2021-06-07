libyEcho.a : yEcho.c yEcho.h
	gcc -c -g -o yEcho.o yEcho.c -lpthread
	ar rc $@ yEcho.o

clean:
	rm yEcho.o
	rm libyEcho.a


