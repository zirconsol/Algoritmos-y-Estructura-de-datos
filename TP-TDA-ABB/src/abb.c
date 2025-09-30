#include "abb.h"
#include "estruc_interna.h"

abb_t *abb_crear(int (*cmp)(const void *, const void *))
{
	if (!cmp) {
		return NULL;
	}

	abb_t *arbol = calloc(1, sizeof(abb_t));

	if (!arbol) {
		return NULL;
	}

	arbol->comparador = cmp;

	return arbol;
}

bool abb_vacio(const abb_t *abb)
{
	if (!abb) {
		return true;
	}

	return abb->raiz == NULL;
}

size_t abb_cantidad(const abb_t *abb)
{
	if (!abb) {
		return 0;
	}

	return abb->nodos;
}

bool abb_insertar(abb_t *abb, const void *elemento)
{
	if (!abb) {
		return NULL;
	}

	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));

	if (!nuevo_nodo) {
		return false;
	}

	nuevo_nodo->dato = (void *)elemento;

	if (!abb->raiz) {
		abb->raiz = nuevo_nodo;
		abb->nodos++;
		return true;
	}

	nodo_t *actual = abb->raiz;
	nodo_t *padre = NULL;
	int cmp = 0;

	while (actual) {
		padre = actual;
		cmp = abb->comparador(elemento, actual->dato);
		if (cmp < 0) {
			actual = actual->izq;
		} else {
			actual = actual->der;
		}
	}

	if (cmp < 0) {
		padre->izq = nuevo_nodo;
	} else {
		padre->der = nuevo_nodo;
	}

	abb->nodos++;
	return true;
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

void abb_destruir(abb_t *abb)
{
	if (!abb) {
		return;
	}
	destruir_nodo_rec(abb->raiz, NULL);
	free(abb);
	abb = NULL;
}
