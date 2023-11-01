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

.PHONY: test
test: build/cal
	diff -u --color=always <(cal 2 2023) <(./build/cal 2 2023)
	diff -u --color=always <(cal 2 2024) <(./build/cal 2 2024)
	diff -u --color=always <(cal 2 2025) <(./build/cal 2 2025)
	diff -u --color=always <(cal -y 2023) <(./build/cal -y 2023)
	diff -u --color=always <(cal -3 2 2023) <(./build/cal -3 2 2023)
	diff -u --color=always <(cal 2022) <(./build/cal 2022)
	diff -u --color=always <(cal) <(./build/cal)

.PHONY: run
run: build/cal
	./build/cal

.PHONY: clean
clean:
	rm -f build/*.o build/cal
