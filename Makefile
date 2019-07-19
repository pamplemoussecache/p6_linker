elfmaker:
	gcc -o elfmaker main.c

linker:
	gcc -O3 -o hello attempt2.c hello_world

clean: 
	rm -f *.o elfmaker
