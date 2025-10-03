#include <stdbool.h>
#include "estruc_interna.h"

nodo_t *abb_insertar_nodo_rec(nodo_t *nodo, const void *dato,
			      int (*comparador)(const void *, const void *),
			      bool *insertado)
{
	if (nodo == NULL) {
		nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
		if (nuevo_nodo != NULL) {
			nuevo_nodo->dato = (void *)dato;
			*insertado = true;
		}
		return nuevo_nodo;
	}
	int cmp = comparador(dato, nodo->dato);

	if (cmp <= 0) {
		nodo->izq = abb_insertar_nodo_rec(nodo->izq, dato, comparador,
						  insertado);
	} else {
		nodo->der = abb_insertar_nodo_rec(nodo->der, dato, comparador,
						  insertado);
	}

	return nodo;
}

void *abb_buscar_nodo_rec(nodo_t *nodo, const void *dato,
			  int (*comparador)(const void *, const void *))
{
	if (nodo == NULL) {
		return NULL;
	}

	int cmp = comparador(dato, nodo->dato);

	if (cmp == 0) {
		return nodo->dato;
	} else if (cmp < 0) {
		return abb_buscar_nodo_rec(nodo->izq, dato, comparador);
	} else {
		return abb_buscar_nodo_rec(nodo->der, dato, comparador);
	}
}
