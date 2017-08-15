all: vfsdemo

vfsdemo: main.o sqlite3.o
	gcc main.o sqlite3.o -o vfsdemo -lpthread -ldl

main.o: main.c
	gcc -c main.c

sqlite3.o: sqlite3.c
	gcc -c sqlite3.c

clean:
	rm -f *.o vfsdemo
