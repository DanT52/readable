all: readable

readable: readable.c main.c
	gcc readable.c main.c -o readable

read:
	cat README.md

run: readable
	./readable

clean: 
	rm -f readable
