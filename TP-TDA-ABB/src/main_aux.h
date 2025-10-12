#ifndef MAIN_AUX_H
#define MAIN_AUX_H

#include "abb.h"
#include "tp1.h"
#include <stdbool.h>

typedef struct {
	abb_t *por_id;
	abb_t *por_nombre;
} indices_t;

void imprimir_pokemon(const struct pokemon *pokemon);
bool imprimir_visitante(void *dato, void *extra);
void mostrar_pokemon_o_error(struct pokemon *pokemon);

int cmp_pokemon_por_id(const void *a, const void *b);
int cmp_pokemon_por_nombre(const void *a, const void *b);

bool indexar_pokemon(struct pokemon *pokemon, void *extra);

struct pokemon *buscar_por_id(abb_t *abb_por_id, int id);
struct pokemon *buscar_por_nombre(abb_t *abb_por_nombre, const char *nombre);

#endif
