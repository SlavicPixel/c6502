UNAME_S := $(shell uname -s)

# Compiler selection
ifeq ($(UNAME_S),Linux)
    CC = gcc
    LDFLAGS = -lraylib -lm -ldl -lpthread -lGL
endif

ifeq ($(UNAME_S),Darwin)
    CC = clang
    LDFLAGS = -lraylib -lm -ldl -framework OpenGL
endif

CFLAGS = -Wall -Wextra -Wpedantic -std=c99 -Iinclude -Ilib

c6502: main.o bus.o cpu.o debugger_gui.o cpu_runner.o
	$(CC) -o c6502 main.o bus.o cpu.o debugger_gui.o cpu_runner.o $(CFLAGS) $(LDFLAGS)

main.o: src/main.c include/bus.h include/disassembler_ht.h lib/raylib.h lib/raygui.h
	$(CC) -c src/main.c $(CFLAGS)

debugger_gui.o: src/debugger_gui.c include/debugger_gui.h include/cpu.h
	$(CC) -c src/debugger_gui.c $(CFLAGS)

cpu_runner.o: src/cpu_runner.c include/cpu_runner.h include/cpu.h
	$(CC) -c src/cpu_runner.c $(CFLAGS)

cpu.o: src/cpu.c include/cpu.h include/bus.h include/disassembler_ht.h
	$(CC) -c src/cpu.c $(CFLAGS)

bus.o: src/bus.c include/bus.h
	$(CC) -c src/bus.c $(CFLAGS)

clean:
	rm -f *.o c6502
