#include "abb.h"
#include "tp1.h"
#include <stdio.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>

typedef struct {
	abb_t *por_id;
	abb_t *por_nombre;
} indices_t;

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

bool imprimir_visitante(void *dato, void *extra)
{
	(void)extra;
	imprimir_pokemon((struct pokemon *)dato);
	return true;
}

void mostrar_pokemon_o_error(struct pokemon *pokemon)
{
	if (pokemon) {
		imprimir_pokemon(pokemon);
	} else {
		printf("No se encontro el pokemon\n");
	}
}

int cmp_pokemon_por_id(const void *primer_poke, const void *segundo_poke)
{
	const struct pokemon *poke_uno = (const struct pokemon *)primer_poke;
	const struct pokemon *poke_dos = (const struct pokemon *)segundo_poke;
	if (poke_uno->id == poke_dos->id) {
		return 0;
	}
	if (poke_uno->id < poke_dos->id) {
		return -1;
	} else {
		return 1;
	}
}

int cmp_pokemon_por_nombre(const void *primer_poke, const void *segundo_poke)
{
	const struct pokemon *poke_uno = (const struct pokemon *)primer_poke;
	const struct pokemon *poke_dos = (const struct pokemon *)segundo_poke;

	int cmp = strcmp(poke_uno->nombre, poke_dos->nombre);

	if (cmp == 0) {
		if (poke_uno->id < poke_dos->id) {
			return -1;
		}
		if (poke_uno->id > poke_dos->id) {
			return 1;
		}
		return 0;
	}
	return cmp;
}

bool indexar_pokemon(struct pokemon *pokemon, void *extra)
{
	indices_t *indice = (indices_t *)extra;
	if (!pokemon || !indice)
		return false;

	bool indexar = true;
	indexar = indexar && abb_insertar(indice->por_id, pokemon);
	indexar = indexar && abb_insertar(indice->por_nombre, pokemon);

	return indexar;
}

struct pokemon *buscar_por_id(abb_t *abb_por_id, int id)
{
	struct pokemon clave = { 0 };
	clave.id = id;
	return (struct pokemon *)abb_buscar(abb_por_id, &clave);
}

struct pokemon *buscar_por_nombre(abb_t *abb_por_nombre, const char *nombre)
{
	struct pokemon clave = { 0 };
	clave.nombre = (char *)nombre;
	return (struct pokemon *)abb_buscar(abb_por_nombre, &clave);
}
