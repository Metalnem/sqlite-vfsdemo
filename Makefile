all: vfsdemo

vfsdemo: main.o sqlite3.o
	gcc -pthread main.o sqlite3.o -o vfsdemo

main.o: main.c
	gcc -c main.c

sqlite3.o: sqlite3.c
	gcc -c sqlite3.c

clean:
	rm *o vfsdemo
