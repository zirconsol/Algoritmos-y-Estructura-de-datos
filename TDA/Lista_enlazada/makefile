correr_pruebas: compilar_pruebas
	./pruebas_lista

compilar_pruebas:
	gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g \
	    pruebas_alumno.c src/lista.c src/pila.c src/cola.c -o pruebas_lista

correr_tp: compilar_tp
	./lista

compilar_tp:
	gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g \
	    main.c src/lista.c src/pila.c src/cola.c src/tp1.c -o lista