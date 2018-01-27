CC = cc
CFLAGS = -Wall
SDLFLAGS = `sdl2-config --cflags --libs`

testSDL:
	gcc -Wall -o testSDL testSDL.c $(SDLFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)