CC=gcc
CFLAGS=-Wall -Wextra -Wpedantic -std=c99

c6502: main.o bus.o cpu.o
	$(CC) -o c6502 main.o bus.o cpu.o $(CFLAGS)

main.o: main.c bus.h
	$(CC) -c main.c $(CFLAGS)

bus.o: bus.c bus.h cpu.h
	$(CC) -c bus.c $(CFLAGS)

cpu.o: cpu.c cpu.h
	$(CC) -c cpu.c $(CFLAGS)