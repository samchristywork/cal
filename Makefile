CC=gcc
CFLAGS=-Wall -Wextra -Werror -pedantic

.PHONY: all
all: build/cal

build/cal: build/cal.o
	mkdir -p build
	$(CC) $(CFLAGS) -o build/cal build/cal.o

build/%.o: src/%.c
	mkdir -p build
	$(CC) $(CFLAGS) -c $< -o $@
