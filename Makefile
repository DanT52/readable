all: readable

readable: assignment3.c main.c
	gcc assignment3.c main.c -o readable

read:
	cat README.md

run: readable
	./readable

clean: 
	rm -f readable