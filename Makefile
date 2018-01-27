CC = gcc
CFLAGS = -W -Wall -g

LFLAGS = `pkg-config --cflags --libs glib-2.0` `xml2-config --cflags --libs` `sdl2-config --cflags --libs` -lSDL2_gfx

SRC_PATH = src/
HEADER_PATH = header/
BIN_PATH = bin/

EXEC = main

all: $(EXEC)

$(EXEC): $(BIN_PATH)main.o $(BIN_PATH)graphics.o $(BIN_PATH)parser.o
	$(CC) $(CFLAGS) -o $(EXEC) $(BIN_PATH)main.o $(BIN_PATH)graphics.o $(BIN_PATH)parser.o $(LFLAGS)

main.o : $(HEADER_PATH)main.h
graphics.o : $(HEADER_PATH)graphics.h
parser.o : $(HEADER_PATH)parser.h

$(BIN_PATH)%.o: $(SRC_PATH)%.c
	$(CC) -o $@ -c $< $(CFLAGS) $(LFLAGS)

clean:
	@rm -f $(BIN_PATH)*.o

mrproper: clean
	rm -rf $(EXEC)
