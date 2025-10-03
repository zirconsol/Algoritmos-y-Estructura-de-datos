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

bool abb_vacio(const abb_t *abb)
{
	if (!abb) {
		return true;
	}

	return abb->raiz == NULL;
}

void *abb_raiz(abb_t *abb)
{
	if (!abb || !abb->raiz) {
		return NULL;
	}

	return abb->raiz;
}

void comparar_recursivamente(abb_t *abb, nodo_t nodo, void *elemento)
{
	return;
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
	if (abb == NULL || abb->comparador == NULL)
		return false;

	bool insertado = false;

	abb->raiz = abb_insertar_nodo_rec(abb->raiz, (void *)elemento,
					  abb->comparador, &insertado);

	if (insertado)
		abb->nodos++;

	return insertado;
}

void *abb_buscar(const abb_t *abb, const void *elemento)
{
	if (abb != NULL && abb->comparador != NULL) {
		return abb_buscar_nodo_rec(abb->raiz, elemento,
					   abb->comparador);
	}
	return NULL;
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
