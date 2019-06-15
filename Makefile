all:
	gcc -o linker main.c

clean: 
	rm -f *.o linker
