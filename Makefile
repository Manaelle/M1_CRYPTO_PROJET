CC=gcc
CFLAGS= -Wall -Werror
LFLAGS= -std=c99

all: main

main: main.o cbc.o tczero.o birthday_attack.o
	$(CC) -o main main.o cbc.o tczero.o birthday_attack.o $(CFLAGS) $(LFLAGS)

main.o: main.c cbc.h tczero.h
	$(CC) -o main.o -c main.c $(CFLAGS) $(LFLAGS)

birthday_attack.o: birthday_attack.c cbc.h tczero.h
	$(CC) -o birthday_attack.o -c birthday_attack.c $(CFLAGS) $(LFLAGS)

cbc.o: cbc.c tczero.h
	$(CC) -o cbc.o -c cbc.c $(CFLAGS) $(LFLAGS)

tczero.o: tczero.c tczero.h
	$(CC) -o tczero.o -c tczero.c $(CFLAGS) $(LFLAGS)

clean:
	rm -rf *.o