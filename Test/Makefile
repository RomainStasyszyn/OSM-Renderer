CC = cc
CFLAGS = -Wall
CPPFLAGS = `xml2-config --cflags`
LDFLAGS = `xml2-config --libs`
SDLFLAGS = `sdl2-config --cflags --libs`
EXEC = parseur
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

parseur: parseur.o file.o
	$(CC) $^ -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SDLFLAGS)

testSDL:
	gcc -Wall -o testSDL testSDL.c $(SDLFLAGS)

%.o: %.c
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS) $(SDLFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
