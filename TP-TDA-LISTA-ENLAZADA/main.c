#include "src/lista.h"
#include "src/tp1.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
struct buscar_id {
	int id;
	struct pokemon *pokemon_buscado;
};

struct buscar_nombre {
	const char *nombre;
	struct pokemon *pokemon_buscado;
};

const char *convertir_tipo_enum_a_string(enum tipo_pokemon poketipo);

void imprimir_pokemon(const struct pokemon *pokemon)
{
	if (!pokemon)
		return;
	printf("%d,%s,%s,%d,%d,%d\n", pokemon->id,
	       pokemon->nombre ? pokemon->nombre : "(null)",
	       convertir_tipo_enum_a_string(pokemon->tipo), pokemon->ataque,
	       pokemon->defensa, pokemon->velocidad);
}

bool imprimir_pokemon_buscado(void *dato, void *extra)
{
	(void)extra;
	imprimir_pokemon((struct pokemon *)dato);
	return true;
}

bool punteros_a_lista(struct pokemon *pokemon, void *extra)
{
	lista_t *lista = (lista_t *)extra;
	return lista_agregar(lista, pokemon);
}

void mostrar_pokemon_o_error(struct pokemon *pokemon)
{
	if (pokemon) {
		imprimir_pokemon(pokemon);
	} else {
		printf("No se encontro el pokemon\n");
	}
}

bool buscar_por_id(void *dato, void *extra)
{
	struct pokemon *pokemon = dato;
	struct buscar_id *buscador = extra;

	if (!pokemon || !buscador) {
		return true;
	}

	if (pokemon && pokemon->id == buscador->id) {
		buscador->pokemon_buscado = pokemon;
		return false;
	}
	return true;
}

bool buscar_por_nombre(void *dato, void *extra)
{
	struct pokemon *pokemon = (struct pokemon *)dato;
	struct buscar_nombre *buscador = (struct buscar_nombre *)extra;

	if (!pokemon || !buscador || !pokemon->nombre || !buscador->nombre) {
		return true;
	}
	if (strcmp(pokemon->nombre, buscador->nombre) == 0) {
		buscador->pokemon_buscado = pokemon;
		return false;
	}
	return true;
}

void destruir_pokemon(void *dato)
{
	struct pokemon *pokemon = dato;
	if (!pokemon)
		return;
	free(pokemon->nombre); // si fue alocado dinámicamente
	free(pokemon);
}

int main(int argc, char *argv[])
{
	tp1_t *archivo = tp1_leer_archivo(argv[1]);

	if (!archivo) {
		printf("No se pudo leer el archivo");
		return 1;
	}
	lista_t *lista = lista_crear();

	if (!lista) {
		tp1_destruir(archivo);
		return 1;
	}
	(void)tp1_con_cada_pokemon(archivo, punteros_a_lista, lista);

	if (argc == 2) {
		(void)lista_con_cada_elemento(lista, imprimir_pokemon_buscado,
					      NULL);
	} else if (argc == 4 && strcmp(argv[2], "buscar id") == 0) {
		int id = (int)strtol(argv[3], NULL, 10);
		struct buscar_id buscador = { .id = id,
					      .pokemon_buscado = NULL };
		(void)lista_con_cada_elemento(lista, buscar_por_id, &buscador);
		mostrar_pokemon_o_error(buscador.pokemon_buscado);
	} else if (argc == 4 && strcmp(argv[2], "buscar nombre") == 0) {
		struct buscar_nombre buscador = { .nombre = argv[3],
						  .pokemon_buscado = NULL };
		(void)lista_con_cada_elemento(lista, buscar_por_nombre,
					      &buscador);
		mostrar_pokemon_o_error(buscador.pokemon_buscado);
	}
	lista_destruir(lista);

	tp1_destruir(archivo);

	return 0;
}
