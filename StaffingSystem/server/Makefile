server:main.o protocal.o server.o
	gcc main.o protocal.o server.o -l pthread -l sqlite3 -o server
main.o:main.c server.h
	gcc -c main.c -o main.o -Wall
server.o:server.c server.h
	gcc -c server.c -o server.o -Wall
protocal:protoacl.c protocal.h
	gcc -c protoal.c -o protocal -Wall
clean:
	rm *o
