CC=gcc
CFLAGS=-Wall -std=c11 -pedantic -Wno-unused-variable
CLIBS=-lSDL2 -lSDL2_image -lSDL2_ttf -lm

main: *.c *.h
	$(CC) main.c -o main $(CFLAGS) $(CLIBS)


debug: *.c *.h
	$(CC) main.c -o main -g $(CFLAGS) $(CLIBS)


concept: *.c *.h
	$(CC) concept.c -o concept $(CFLAGS) $(CLIBS)

concept_debug: *.c *.h
	$(CC) concept.c -o concept -g $(CFLAGS) $(CLIBS)