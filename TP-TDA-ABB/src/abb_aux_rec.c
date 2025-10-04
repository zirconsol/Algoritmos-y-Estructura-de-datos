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

size_t abb_inorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return 0;
	}
	size_t nodos_visitados = 0;

	if (abb_inorden_rec(nodo->izq, f, extra)) {
		nodos_visitados++;
	}

	if (!f(nodo->dato, extra)) {
		return nodos_visitados + 1;
	}

	nodos_visitados++;

	if (abb_inorden_rec(nodo->der, f, extra)) {
		nodos_visitados++;
	}

	return nodos_visitados;
}

size_t abb_preorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return 0;
	}

	size_t nodos_visitados = 0;

	if (!f(nodo->dato, extra)) {
		return 1;
	}
	nodos_visitados++;

	if (abb_preorden_rec(nodo->izq, f, extra)) {
		nodos_visitados++;
	}

	if (abb_preorden_rec(nodo->der, f, extra)) {
		nodos_visitados++;
	}

	return nodos_visitados;
}

size_t abb_postorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra)
{
	if (nodo == NULL) {
		return 0;
	}

	size_t nodos_visitados = 0;

	if (abb_postorden_rec(nodo->izq, f, extra)) {
		nodos_visitados++;
	}

	if (abb_postorden_rec(nodo->der, f, extra)) {
		nodos_visitados++;
	}

	if (!f(nodo->dato, extra)) {
		return nodos_visitados + 1;
	}

	nodos_visitados++;

	return nodos_visitados;
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
