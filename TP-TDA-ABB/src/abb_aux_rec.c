#include <stdbool.h>
#include "abb.h"
#include "estruc_interna.h"

struct vector_contenedor {
	void **destino;
	size_t cap;
	size_t insertados;
};

void reconectar_arbol(abb_t *abb, nodo_t *padre, nodo_t *nodo_viejo,
		      nodo_t *nodo_nuevo)
{
	if (padre) {
		if (padre->izq == nodo_viejo) {
			padre->izq = nodo_nuevo;
		} else {
			padre->der = nodo_nuevo;
		}
	} else {
		abb->raiz = nodo_nuevo;
	}
}

void liberar_nodo(abb_t *abb, nodo_t *nodo)
{
	free(nodo);
	abb->cantidad--;
}

void eliminar_nodo_hoja(abb_t *abb, nodo_t *nodo, nodo_t *padre)
{
	reconectar_arbol(abb, padre, nodo, NULL);
	liberar_nodo(abb, nodo);
}

void eliminar_nodo_con_un_hijo(abb_t *abb, nodo_t *nodo, nodo_t *padre)
{
	nodo_t *hijo;

	if (nodo->izq) {
		hijo = nodo->izq;
	} else {
		hijo = nodo->der;
	}
	reconectar_arbol(abb, padre, nodo, hijo);
	liberar_nodo(abb, nodo);
}

nodo_t *buscar_maximo(nodo_t *nodo, nodo_t **padre)
{
	if (!nodo)
		return NULL;

	while (nodo->der) {
		if (padre)
			*padre = nodo;
		nodo = nodo->der;
	}
	return nodo;
}

void eliminar_nodo_con_dos_hijos(abb_t *abb, nodo_t *nodo, nodo_t *padre)
{
	(void)padre;
	nodo_t *padre_pred = nodo;
	nodo_t *pred = buscar_maximo(nodo->izq, &padre_pred);
	nodo->dato = pred->dato;

	if (!pred->izq && !pred->der) {
		eliminar_nodo_hoja(abb, pred, padre_pred);
	} else {
		eliminar_nodo_con_un_hijo(abb, pred, padre_pred);
	}
}

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
	if (dato == NULL && nodo->dato == NULL) {
		nodo->izq = abb_insertar_nodo_rec(nodo->izq, dato, comparador,
						  insertado);
	} else if (dato == NULL && nodo->dato != NULL) {
		nodo->izq = abb_insertar_nodo_rec(nodo->izq, dato, comparador,
						  insertado);
	} else if (dato != NULL && nodo->dato == NULL) {
		nodo->der = abb_insertar_nodo_rec(nodo->der, dato, comparador,
						  insertado);
	} else {
		int cmp = comparador(dato, nodo->dato);
		if (cmp <= 0)
			nodo->izq = abb_insertar_nodo_rec(
				nodo->izq, dato, comparador, insertado);
		else
			nodo->der = abb_insertar_nodo_rec(
				nodo->der, dato, comparador, insertado);
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
	static bool parar = false;
	static int nivel = 0;
	size_t nodos_visitados = 0;

	if (nivel == 0) {
		parar = false;
	}

	if (!nodo || !f) {
		return 0;
	}

	nivel++;

	if (!parar) {
		nodos_visitados += abb_inorden_rec(nodo->izq, f, extra);

		if (!parar) {
			nodos_visitados += 1;
			if (!f(nodo->dato, extra)) {
				parar = true;
			} else {
				nodos_visitados +=
					abb_inorden_rec(nodo->der, f, extra);
			}
		}
	}

	nivel--;

	if (nivel == 0) {
		parar = false;
	}

	return nodos_visitados;
}

size_t abb_preorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra)
{
	static bool parar = false;
	static int nivel = 0;
	size_t nodos_visitados = 0;

	if (nivel == 0) {
		parar = false;
	}
	if (!nodo || !f)
		return 0;
	nivel++;

	if (!parar) {
		nodos_visitados += 1;
		if (!f(nodo->dato, extra)) {
			parar = true;
		} else {
			nodos_visitados +=
				abb_preorden_rec(nodo->izq, f, extra);
			if (!parar)
				nodos_visitados +=
					abb_preorden_rec(nodo->der, f, extra);
		}
	}

	nivel--;
	if (nivel == 0) {
		parar = false;
	}

	return nodos_visitados;
}

size_t abb_postorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra)
{
	static bool parar = false;
	static int nivel = 0;
	size_t nodos_visitados = 0;

	if (nivel == 0) {
		parar = false;
	}

	if (!nodo || !f)
		return 0;
	nivel++;

	if (!parar)
		nodos_visitados += abb_postorden_rec(nodo->izq, f, extra);
	if (!parar)
		nodos_visitados += abb_postorden_rec(nodo->der, f, extra);

	if (!parar) {
		nodos_visitados += 1;
		if (!f(nodo->dato, extra)) {
			parar = true;
		}
	}

	nivel--;
	if (nivel == 0) {
		parar = false;
	}
	return nodos_visitados;
}

bool insertar_dato_en_vector(void *dato, void *extra)
{
	struct vector_contenedor *vector = extra;

	if (vector->insertados >= vector->cap) {
		return false;
	}
	vector->destino[vector->insertados] = dato;
	vector->insertados++;
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
