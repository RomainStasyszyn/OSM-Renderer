LIBXML = `xml2-config --cflags --libs`
GLIB = `pkg-config --cflags --libs glib-2.0`
SDL = `sdl2-config --cflags --libs`
GFX = '-lSDL2_gfx'
CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS =

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
AOUT = parseur

all : $(AOUT)

parseur : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBXML) $(GLIB) $(SDL) $(GFX)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBXML) $(GLIB) $(SDL) $(GFX)

clean :
	@rm *.o

mrproper:
	rm -rf $(AOUT)

cleaner : clean
	@rm $(AOUT)
