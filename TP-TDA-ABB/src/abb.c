#include "abb.h"
#include "estruc_interna.h"
#include "abb_eliminar.c"
#include "abb_aux_rec.c"

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

bool abb_insertar(abb_t *abb, const void *elemento)
{
	if (abb == NULL || abb->comparador == NULL || elemento == NULL)
		return false;

	bool insertado = false;

	abb->raiz = abb_insertar_nodo_rec(abb->raiz, (void *)elemento,
					  abb->comparador, &insertado);

	if (insertado)
		abb->cantidad++;

	return insertado;
}

bool abb_existe(const abb_t *abb, const void *elemento)
{
	return abb_buscar(abb, elemento) != NULL;
}

void *abb_buscar(const abb_t *abb, const void *elemento)
{
	if (abb != NULL && abb->comparador != NULL) {
		return abb_buscar_nodo_rec(abb->raiz, elemento,
					   abb->comparador);
	}
	return NULL;
}

void *abb_raiz(abb_t *abb)
{
	if (!abb || !abb->raiz) {
		return NULL;
	}

	return abb->raiz;
}

void *abb_eliminar(abb_t *abb, const void *elemento)
{
	if (!abb || !abb->raiz) {
		return NULL;
	}

	nodo_t *padre = NULL;
	nodo_t *actual = abb->raiz;
	int cmp = 0;

	while (actual != NULL &&
	       (cmp = abb->comparador(elemento, actual->dato) != 0)) {
		padre = actual;

		if (cmp < 0) {
			actual = actual->izq;
		} else {
			actual = actual->der;
		}
	}

	if (actual == NULL) {
		return NULL;
	}

	void *dato_eliminado = actual->dato;

	if (!actual->izq && !actual->der)
		eliminar_nodo_hoja(abb, actual, padre);
	else if (!actual->izq || !actual->der)
		eliminar_nodo_con_un_hijo(abb, actual, padre);
	else
		eliminar_nodo_con_dos_hijos(abb, actual, padre);

	return dato_eliminado;
}

size_t abb_cantidad(const abb_t *abb)
{
	if (!abb) {
		return 0;
	}

	return abb->cantidad;
}

bool abb_vacio(const abb_t *abb)
{
	if (!abb) {
		return true;
	}

	return abb->raiz == NULL;
}

size_t abb_con_cada_elemento(const abb_t *abb, enum abb_recorrido modo,
			     bool (*f)(void *, void *), void *extra)
{
	if (abb == NULL || f == NULL)
		return 0;

	size_t visitados = 0;

	switch (modo) {
	case ABB_INORDEN:
		visitados = abb_inorden_rec(abb->raiz, f, extra);
		break;
	case ABB_PREORDEN:
		visitados = abb_preorden_rec(abb->raiz, f, extra);
		break;
	case ABB_POSTORDEN:
		visitados = abb_postorden_rec(abb->raiz, f, extra);
		break;
	default:
		visitados = 0;
		break;
	}

	return visitados;
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
