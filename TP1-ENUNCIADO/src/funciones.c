#include "funciones.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define CAP_INICIAL 8
#define FACTOR_CRECIMIENTO 2

void ordenar_indices_por_pokeid(const tp1_t *pokedex, size_t *indices);

const char *tipo_str(enum tipo_pokemon poketipo)
{
	switch (poketipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	}
	return "INVALIDO";
}

void imprimir_pokemon(const struct pokemon *pokemon)
{
	if (!pokemon)
		return;
	printf("%d,%s,%s,%d,%d,%d\n", pokemon->id,
	       pokemon->nombre ? pokemon->nombre : "", tipo_str(pokemon->tipo),
	       pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}

int buscar_nombre(const char *ruta, const char *nombre)
{
	tp1_t *tp = tp1_leer_archivo(ruta);
	if (!tp)
		return -1;

	struct pokemon *pokemon = tp1_buscar_nombre(tp, nombre);
	if (pokemon)
		imprimir_pokemon(pokemon);

	tp1_destruir(tp);
	return pokemon ? 0 : -2;
}

int buscar_id(const char *ruta, int id)
{
	tp1_t *tp = tp1_leer_archivo(ruta);
	if (!tp)
		return -1;

	struct pokemon *pokemon = tp1_buscar_id(tp, id);
	if (pokemon)
		imprimir_pokemon(pokemon);

	tp1_destruir(tp);
	return pokemon ? 0 : -2;
}

int union_arch(const char *un_tp, const char *otro_tp, const char *salida_tp)
{
	tp1_t *un_arch = tp1_leer_archivo(un_tp);
	tp1_t *otro_arch = tp1_leer_archivo(otro_tp);
	if (!un_arch || !otro_arch) {
		tp1_destruir(un_arch);
		tp1_destruir(otro_arch);
		return -1;
	}

	tp1_t *arch_unificado = tp1_union(un_arch, otro_arch);
	tp1_destruir(un_arch);
	tp1_destruir(otro_arch);
	if (!arch_unificado)
		return -1;

	int arch_guardado = (tp1_guardar_archivo(arch_unificado, salida_tp) ==
			     arch_unificado) ?
				    0 :
				    -1;
	tp1_destruir(arch_unificado);
	return arch_guardado;
}

int intersec_arch(const char *un_tp, const char *otro_tp, const char *salida_tp)
{
	tp1_t *un_arch = tp1_leer_archivo(un_tp);
	tp1_t *otro_arch = tp1_leer_archivo(otro_tp);
	if (!un_arch || !otro_arch) {
		tp1_destruir(un_arch);
		tp1_destruir(otro_arch);
		return -1;
	}

	tp1_t *arch_intersec = tp1_interseccion(un_arch, otro_arch);
	tp1_destruir(un_arch);
	tp1_destruir(otro_arch);
	if (!arch_intersec)
		return -1;

	int arch_guardado = (tp1_guardar_archivo(arch_intersec, salida_tp) ==
			     arch_intersec) ?
				    0 :
				    -1;
	tp1_destruir(arch_intersec);
	return arch_guardado;
}

int dif_arch(const char *un_tp, const char *otro_tp, const char *salida_tp)
{
	tp1_t *primer_tp = tp1_leer_archivo(un_tp);
	tp1_t *segundo_tp = tp1_leer_archivo(otro_tp);
	if (!primer_tp) {
		tp1_destruir(segundo_tp);
		return -1;
	}

	tp1_t *tercer_tp = tp1_diferencia(primer_tp, segundo_tp);
	tp1_destruir(primer_tp);
	tp1_destruir(segundo_tp);
	if (!tercer_tp)
		return -1;

	int dif_valida =
		(tp1_guardar_archivo(tercer_tp, salida_tp) == tercer_tp) ? 0 :
									   1;
	tp1_destruir(tercer_tp);
	return dif_valida;
}

bool imprimir(struct pokemon *pokemon, void *extra)
{
	(void)extra;
	imprimir_pokemon(pokemon);
	return true;
}

int mostrar_por_id(const char *ruta)
{
	tp1_t *tp = tp1_leer_archivo(ruta);
	if (!tp)
		return -1;

	(void)tp1_con_cada_pokemon(tp, imprimir, NULL);

	tp1_destruir(tp);
	return 0;
}

struct vec_pokemons {
	const struct pokemon **poke;
	size_t cant, cap;
};

bool agregar_pokemon_al_vector(struct pokemon *pokemon, void *extra)
{
	struct vec_pokemons *pokemons = (struct vec_pokemons *)extra;

	if (pokemons->cant == pokemons->cap) {
		size_t nueva_cap = pokemons->cap ?
					   pokemons->cap * FACTOR_CRECIMIENTO :
					   CAP_INICIAL;
		const struct pokemon **buffer_temp = realloc(
			pokemons->poke, nueva_cap * sizeof(*buffer_temp));
		if (!buffer_temp) {
			return false;
		}
		pokemons->poke = buffer_temp;
		pokemons->cap = nueva_cap;
	}
	pokemons->poke[pokemons->cant++] = pokemon;
	return true;
}

void ordenar_pokemons_por_nombre(struct vec_pokemons *pokemons)
{
	if (!pokemons || pokemons->cant < 2) {
		return;
	}

	for (size_t i = 0; i + 1 < pokemons->cant; i++) {
		size_t indice_menor = i;

		for (size_t j = i + 1; j < pokemons->cant; j++) {
			char *poke_actual = pokemons->poke[j]->nombre;
			char *poke_menor = pokemons->poke[indice_menor]->nombre;

			if (!poke_actual) {
				poke_actual = "";
			}

			if (!poke_menor) {
				poke_menor = "";
			}

			if (strcmp(poke_actual, poke_menor) < 0) {
				indice_menor = j;
			}
		}

		if (indice_menor != i) {
			const struct pokemon *poke_aux = pokemons->poke[i];
			pokemons->poke[i] = pokemons->poke[indice_menor];
			pokemons->poke[indice_menor] = poke_aux;
		}
	}
}

int mostrar_por_nombre(const char *ruta)
{
	tp1_t *tp = tp1_leer_archivo(ruta);
	if (!tp) {
		return -1;
	}

	struct vec_pokemons pokemons = { 0 };
	if (tp1_con_cada_pokemon(tp, agregar_pokemon_al_vector, &pokemons) !=
	    tp1_cantidad(tp)) {
		free(pokemons.poke);
		tp1_destruir(tp);
		return -1;
	}

	ordenar_pokemons_por_nombre(&pokemons);
	for (size_t i = 0; i < pokemons.cant; i++) {
		imprimir_pokemon(pokemons.poke[i]);
	}

	free(pokemons.poke);
	tp1_destruir(tp);
	return 0;
}