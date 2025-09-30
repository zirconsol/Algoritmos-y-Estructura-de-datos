#include "abb.h"
#include "estruc_interna.h"

void reconectar_arbol(abb_t *abb, nodo_t *padre, nodo_t *nodo_viejo,
		      nodo_t *nodo_nuevo)
{
	if (padre) {
		if (padre->izq == nodo_viejo) {
			padre->izq == nodo_nuevo;
		} else {
			padre->der == nodo_nuevo;
		}
	} else {
		abb->raiz == nodo_nuevo;
	}
}

void liberar_nodo(abb_t *abb, nodo_t *nodo)
{
	free(nodo);
	abb->nodos--;
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

void eliminar_nodo_con_dos_hijos(abb_t *abb, nodo_t *nodo, nodo_t *padre);
