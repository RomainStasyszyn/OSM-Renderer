CC = gcc
CFLAGS = -g -Wall -Wextra -Werror

LIBXML = `xml2-config --cflags --libs`
GLIB = `pkg-config --cflags --libs glib-2.0`
SDL = `sdl2-config --cflags --libs`
GFX = -lSDL2_gfx

SRC_DIR = src
INC_DIR = inc
OBJ_DIR = obj
BIN_DIR = bin

EXEC = Render

all: $(EXEC)

$(OBJ_DIR)/parser.o: $(SRC_DIR)/parser.c $(INC_DIR)/parser.h $(INC_DIR)/error.h $(INC_DIR)/dataStructures.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/parser.c $(LIBXML) $(GLIB)

$(OBJ_DIR)/graphics.o: $(SRC_DIR)/graphics.c $(INC_DIR)/graphics.h $(INC_DIR)/dataStructures.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/graphics.c $(GLIB) $(SDL) $(GFX)

$(OBJ_DIR)/main.o: $(SRC_DIR)/main.c $(INC_DIR)/parser.h $(INC_DIR)/graphics.h
	$(CC) $(CFLAGS) -c $(SRC_DIR)/main.c $(LIBXML) $(GLIB) $(SDL) $(GFX)

$(BIN_DIR)/$(EXEC): $(OBJ_DIR)/main.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/parser.o
	$(CC) $(CFLAGS) -o $(BIN_DIR)/$(EXEC) $(OBJ_DIR)/main.o $(OBJ_DIR)/graphics.o $(OBJ_DIR)/parser.o $(LIBXML) $(GLIB) $(SDL) $(GFX)

clean:
	@rm -f $(OBJ_DIR)/*.o

mrproper: clean
	rm -rf $(BIN_DIR)/$(EXEC)
