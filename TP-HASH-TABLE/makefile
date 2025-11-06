CC = gcc
CFLAGS = -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g -Wno-strict-prototypes
VALGRIND_FLAGS = --leak-check=full --track-origins=yes --show-reachable=yes --error-exitcode=2 --show-leak-kinds=all --trace-children=yes

SRC := $(wildcard src/*.c)

.PHONY: all run valgrind clean

all: pruebas_alumno

pruebas_alumno: $(SRC) pruebas_alumno.c
	$(CC) $(CFLAGS) $^ -o $@

run: pruebas_alumno
	./pruebas_alumno

valgrind: pruebas_alumno
	valgrind $(VALGRIND_FLAGS) ./pruebas_alumno

clean:
	rm -f pruebas_alumno