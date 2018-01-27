LIBXML = `xml2-config --cflags --libs`
GLIB = `pkg-config --cflags --libs glib-2.0`
SDL = `sdl2-config --cflags --libs`
CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS =

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
AOUT = protoRenderer

all : $(AOUT)

protoRenderer : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBXML) $(GLIB) $(SDL)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBXML) $(GLIB) $(SDL)

clean :
	@rm *.o

mrproper:
	rm -rf $(AOUT)

cleaner : clean
	@rm $(AOUT)
