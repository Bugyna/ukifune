CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -Wno-unused-variable
CLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf

main: *.c *.h
	$(CC) main.c -o main $(CFLAGS) $(CLIBS)