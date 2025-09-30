correr_pruebas: compilar_pruebas
	./pruebas_abb

compilar_pruebas:
	gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g \
	    pruebas_alumno.c src/abb.c -o pruebas_abb

correr_tp: compilar_tp
	./abb

compilar_tp:
	gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g \
	    main.c src/abb.c -o abb