correr-pruebas: compilar-pruebas
	valgrind --track-origin=yes --leak-check=full ./pruebas

compilar-tp:
	gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g main.c src/tp1.c src/funciones.c -g -o tp1

correr-valgrind: compilar-tp
	valgrind --track-origin=yes --leak-check=full ./tp1 ejemplos/normal.csv

compilar-pruebas:
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g pruebas_alumno.c src/tp1.c -o pruebas

