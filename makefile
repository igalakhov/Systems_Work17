all: main.o header.h init.o
	gcc main.o init.o

main.o: main.c header.h
	gcc -c main.c

init.o: init.c
	gcc -c init.c

run:
	./a.out

clean:
	rm *.o
