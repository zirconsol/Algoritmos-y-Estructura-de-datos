#ifndef ABB_FUNCIONES_H
#define ABB_FUNCIONES_H

#include <stdbool.h>
#include <stddef.h>
#include "abb.h"
#include "estruc_interna.h"

struct vector_contenedor {
	void **destino;
	size_t cap;
	size_t insertados;
};

bool insertar_dato_en_vector(void *dato, void *extra);

void reconectar_arbol(abb_t *abb, nodo_t *padre, nodo_t *nodo_viejo,
		      nodo_t *nodo_nuevo);

void liberar_nodo(abb_t *abb, nodo_t *nodo);

void eliminar_nodo_hoja(abb_t *abb, nodo_t *nodo, nodo_t *padre);

void eliminar_nodo_con_un_hijo(abb_t *abb, nodo_t *nodo, nodo_t *padre);

nodo_t *buscar_minimo(nodo_t *nodo, nodo_t **padre);

void eliminar_nodo_con_dos_hijos(abb_t *abb, nodo_t *nodo, nodo_t *padre);

nodo_t *abb_insertar_nodo_rec(nodo_t *nodo, const void *dato,
			      int (*comparador)(const void *, const void *),
			      bool *insertado);

void *abb_buscar_nodo_rec(nodo_t *nodo, const void *dato,
			  int (*comparador)(const void *, const void *));

size_t abb_inorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra);

size_t abb_preorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra);

size_t abb_postorden_rec(nodo_t *nodo, bool (*f)(void *, void *), void *extra);

void destruir_nodo_rec(nodo_t *nodo, void (*destructor)(void *));

#endif
