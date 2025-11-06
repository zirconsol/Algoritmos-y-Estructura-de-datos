#include "src/abb.h"
#include "src/tp1.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include "src/main_aux.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		return 1;
	}

	tp1_t *archivo = tp1_leer_archivo(argv[1]);
	if (!archivo) {
		fprintf(stderr, "No se pudo leer el archivo: %s\n", argv[1]);
		return 1;
	}

	abb_t *abb_por_id = abb_crear(cmp_pokemon_por_id);
	abb_t *abb_por_nombre = abb_crear(cmp_pokemon_por_nombre);

	if (!abb_por_id || !abb_por_nombre) {
		abb_destruir(abb_por_id);
		abb_destruir(abb_por_nombre);
		tp1_destruir(archivo);
		fprintf(stderr, "Error creando los ABB de índices\n");
		return 1;
	}

	indices_t indice = { .por_id = abb_por_id,
			     .por_nombre = abb_por_nombre };

	if (!tp1_con_cada_pokemon(archivo, indexar_pokemon, &indice)) {
		fprintf(stderr, "Error indexando pokemones\n");
		abb_destruir(abb_por_nombre);
		abb_destruir(abb_por_id);
		tp1_destruir(archivo);
		return 1;
	}

	if (argc == 2) {
		(void)abb_con_cada_elemento(abb_por_nombre, ABB_INORDEN,
					    imprimir_visitante, NULL);

		abb_destruir(abb_por_nombre);
		abb_destruir(abb_por_id);
		tp1_destruir(archivo);
		return 0;

	} else if (argc == 5 && strcmp(argv[2], "buscar") == 0) {
		if (strcmp(argv[3], "id") == 0) {
			char *aux = NULL;
			long id = strtol(argv[4], &aux, 10);
			if (!argv[4][0] || (aux && *aux)) {
				fprintf(stderr, "ID inválido: %s\n", argv[4]);
				abb_destruir(abb_por_nombre);
				abb_destruir(abb_por_id);
				tp1_destruir(archivo);
				return 1;
			}
			struct pokemon *pokemon =
				buscar_por_id(abb_por_id, (int)id);
			mostrar_pokemon_o_error(pokemon);

		} else if (strcmp(argv[3], "nombre") == 0) {
			struct pokemon *pokemon =
				buscar_por_nombre(abb_por_nombre, argv[4]);
			mostrar_pokemon_o_error(pokemon);

		} else {
			fprintf(stderr,
				"Clave desconocida, use 'id' o 'nombre'\n");
			abb_destruir(abb_por_nombre);
			abb_destruir(abb_por_id);
			tp1_destruir(archivo);
			return 1;
		}

		abb_destruir(abb_por_nombre);
		abb_destruir(abb_por_id);
		tp1_destruir(archivo);
		return 0;

	} else {
		abb_destruir(abb_por_nombre);
		abb_destruir(abb_por_id);
		tp1_destruir(archivo);
		return 1;
	}
}
