CC = gcc
SRC = sdl.c
OUT = hello_sdl
CFLAGS = $(shell sdl2-config --cflags)
LIBS = $(shell sdl2-config --libs) -lSDL2_image -lm

$(OUT): $(SRC)
	$(CC) -o $(OUT) $(SRC) $(CFLAGS) $(LIBS)

run: $(OUT)
	./$(OUT)

clean:
	rm -f $(OUT)
