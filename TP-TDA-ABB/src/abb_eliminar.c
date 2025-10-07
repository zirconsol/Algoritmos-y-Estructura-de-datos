#include "abb.h"
#include "estruc_interna.h"

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

nodo_t *buscar_minimo(nodo_t *nodo, nodo_t **padre)
{
	if (!nodo)
		return NULL;

	while (nodo->izq) {
		if (padre)
			*padre = nodo;
		nodo = nodo->izq;
	}
	return nodo;
}

void eliminar_nodo_con_dos_hijos(abb_t *abb, nodo_t *nodo, nodo_t *padre)
{
	(void)padre;
	nodo_t *padre_sucesor = nodo;
	nodo_t *sucesor = buscar_minimo(nodo->der, &padre_sucesor);
	nodo->dato = sucesor->dato;

	if (!sucesor->izq && !sucesor->der) {
		eliminar_nodo_hoja(abb, sucesor, padre_sucesor);
	} else {
		eliminar_nodo_con_un_hijo(abb, sucesor, padre_sucesor);
	}
}
