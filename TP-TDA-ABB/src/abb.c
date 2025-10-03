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
	if (abb == NULL || abb->comparador == NULL)
		return false;

	bool insertado = false;

	abb->raiz = abb_insertar_nodo_rec(abb->raiz, (void *)elemento,
					  abb->comparador, &insertado);

	if (insertado)
		abb->nodos++;

	return insertado;
}

bool abb_existe(const abb_t *abb, const void *elemento)
{
	return false;
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

size_t abb_cantidad(const abb_t *abb)
{
	if (!abb) {
		return 0;
	}

	return abb->nodos;
}

bool abb_vacio(const abb_t *abb)
{
	if (!abb) {
		return true;
	}

	return abb->raiz == NULL;
}

void abb_recorrer(const struct abb_t *abb, enum abb_recorrido orden,
		  void (*f)(void *, void *), void *extra)
{
	if (abb == NULL || f == NULL)
		return;

	switch (orden) {
	case ABB_INORDEN:
		abb_inorden_rec(abb->raiz, f, extra);
		break;
	case ABB_PREORDEN:
		abb_preorden_rec(abb->raiz, f, extra);
		break;
	case ABB_POSTORDEN:
		abb_postorden_rec(abb->raiz, f, extra);
		break;
	default:
		break;
	}
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
