#include "abb.h"
#include "estruc_interna.h"
#include "abb_eliminar.c"

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

bool abb_existe(const abb_t *abb, const void *elemento)
{
	bool nodo_existe = false;

	if (abb && abb->raiz) {
		nodo_t *nodo = abb->raiz;
		while (nodo && !nodo_existe) {
			int comparacion = abb->comparador(elemento, nodo->dato);

			if (comparacion == 0) {
				nodo_existe = true;
			} else if (comparacion < 0) {
				nodo = nodo->izq;
			} else {
				nodo = nodo->der;
			}
		}
		return nodo_existe;
	}

	/*primero comparo contra la raiz para ver que rama tomar

si el comparador da <0 agarramos comparamos contra hijo derecho

si el comparador da >0, comparamos contra el hijo izquierdo

si el comparador da 0, encontramos el elemento.

llamada recursiva  para repetir con el padre.

se repite hasta que comparador = 0 o caso invalido*/
}

void comparar_recursivamente(abb_t *abb, nodo_t nodo, void *elemento)
{
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
