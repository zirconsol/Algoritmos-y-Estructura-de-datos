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

void abb_inorden_rec(nodo_t *nodo, void (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return NULL;
	}
	abb_inorden_rec(nodo->izq, f, extra);
	f(nodo->dato, extra);
	abb_inorden_rec(nodo->der, f, extra);
}

void abb_preorden_rec(nodo_t *nodo, void (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return NULL;
	}
	f(nodo->dato, extra);
	abb_preorden_rec(nodo->izq, f, extra);
	abb_preorden_rec(nodo->der, f, extra);
}

void abb_postorden_rec(nodo_t *nodo, void (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return NULL;
	}
	abb_postorden_rec(nodo->izq, f, extra);
	abb_postorden_rec(nodo->der, f, extra);
	f(nodo->dato, extra);
}

void destruir_nodo_rec(nodo_t *nodo, void (*destructor)(void *))
{
	if (!nodo) {
		return;
	}

	destruir_nodo_rec(nodo->izq, destructor);
	destruir_nodo_rec(nodo->der, destructor);

	if (destructor) {
		destructor(nodo->dato);
	}
	free(nodo);
}