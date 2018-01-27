all: test0

test0: test0.o
		gcc -o test0 `sdl2-config --cflags` test0.c `sdl2-config --libs`

test0.o: test0.c
		gcc -c test0.c

clean: 
		rm -rf *.o

mrproper: clean
		rm -rf test0