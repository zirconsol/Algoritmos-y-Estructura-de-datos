#include "lista.h"
#include <stdlib.h>

typedef struct pila {
	lista_t *lista;
} pila_t;

pila_t *pila_crear(void)
{
	pila_t *pila = malloc(sizeof(pila_t));

	if (!pila) {
		return NULL;
	}
	pila->lista = lista_crear();

	if (!pila->lista) {
		free(pila);
		return NULL;
	}
	return pila;
}

bool pila_apilar(pila_t *pila, void *dato)
{
	if (!pila) {
		return false;
	}

	if (lista_vacia(pila->lista)) {
		return lista_agregar(pila->lista, dato);
	}

	return lista_insertar(pila->lista, dato, 0);
}

void *pila_desapilar(pila_t *pila)
{
	if (lista_vacia(pila->lista)) {
		return NULL;
	}
	return lista_eliminar_elemento(pila->lista, 0);
}

void *pila_ver_primero(pila_t *pila)
{
	size_t cant = lista_cantidad(pila->lista);

	if (cant == 0) {
		return NULL;
	}

	return lista_buscar_elemento(pila->lista, 0);
}

size_t pila_cantidad(pila_t *pila)
{
	return lista_cantidad(pila->lista);
}

void pila_destruir(pila_t *pila)
{
	if (!pila) {
		return;
	}
	lista_destruir(pila->lista);
	free(pila);
}
