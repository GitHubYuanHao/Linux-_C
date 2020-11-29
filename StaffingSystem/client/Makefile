client:main.o protocal.o client.o
	gcc main.o protocal.o client.o -l sqlite3 -o client
main.o:main.c client.h
	gcc -c main.c -o main.o -Wall
client.o:client.c client.h
	gcc -c client.c -o client.o -Wall
ptotocal.o:protoacl.c protocal.h
	gcc -c protocal.c -o protocal.o -Wall
clean:
	rm *.o
