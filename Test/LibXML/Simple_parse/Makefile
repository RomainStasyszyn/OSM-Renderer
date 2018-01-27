CC = cc
CFLAGS = -Wall
CPPFLAGS = `xml2-config --cflags`
LDFLAGS = `xml2-config --libs`
EXEC = parseur
SRC = $(wildcard *.c)
OBJ = $(SRC:.c=.o)

all: $(EXEC)

parseur: parseur.o file.o
	$(CC) $^ -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

%.o: %.c 
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LDFLAGS)

clean:
	rm -rf *.o

mrproper:
	rm -rf $(EXEC)
