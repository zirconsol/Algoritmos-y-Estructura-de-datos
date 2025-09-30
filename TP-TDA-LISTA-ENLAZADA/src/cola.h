#ifndef COLA_H
#define COLA_H

#include "lista.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct cola {
	lista_t *lista;
} cola_t;

cola_t *cola_crear(void);

bool cola_encolar(cola_t *cola, void *elemento);

void *cola_desencolar(cola_t *cola);

void *cola_ver_primero(cola_t *cola);

size_t cola_cantidad(cola_t *cola);

void cola_destruir(cola_t *cola);

#endif
