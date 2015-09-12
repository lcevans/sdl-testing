all: src/main.c
	gcc -g -o output src/main.c -lSDL2 -lm
