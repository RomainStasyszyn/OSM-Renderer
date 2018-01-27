LIBXML = `xml2-config --cflags --libs`
CC = gcc
CFLAGS = -W -Wall -g
LDFLAGS =

SRC = $(wildcard *.c)
OBJS = $(SRC:.c=.o)
AOUT = parseur

all : $(AOUT)

parseur : $(OBJS)
	$(CC) $(LDFLAGS) -o $@ $^ $(LIBXML)

%.o : %.c
	$(CC) $(CFLAGS) -o $@ -c $< $(LIBXML)

clean :
	@rm *.o

mrproper:
	rm -rf $(AOUT)

cleaner : clean
	@rm $(AOUT)
