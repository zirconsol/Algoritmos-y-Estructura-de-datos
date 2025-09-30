#ifndef PILA_H
#define PILA_H

#include "lista.h"
#include <stdbool.h>
#include <stddef.h>

typedef struct pila {
	lista_t *lista;
} pila_t;

pila_t *pila_crear(void);

bool pila_apilar(pila_t *pila, void *dato);

void *pila_desapilar(pila_t *pila);

void *pila_ver_primero(pila_t *pila);

bool pila_cantidad(pila_t *pila);

void pila_destruir(pila_t *pila);

#endif
