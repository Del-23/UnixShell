
all: main.c 
	gcc -Wall *.c -o main

clean: 
	$(RM) main