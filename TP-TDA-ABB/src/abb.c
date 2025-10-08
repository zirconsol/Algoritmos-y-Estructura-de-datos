#include "abb.h"
#include "estruc_interna.h"
#include "abb_aux_rec.h"

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

bool abb_insertar(abb_t *abb, void *dato)
{
	if (abb == NULL || abb->comparador == NULL)
		return false;

	bool insertado = false;

	abb->raiz = abb_insertar_nodo_rec(abb->raiz, (void *)dato,
					  abb->comparador, &insertado);

	if (insertado)
		abb->cantidad++;

	return insertado;
}

bool abb_existe(abb_t *abb, void *dato)
{
	return abb_buscar(abb, dato) != NULL;
}

void *abb_buscar(abb_t *abb, void *dato)
{
	if (abb != NULL && abb->comparador != NULL) {
		return abb_buscar_nodo_rec(abb->raiz, dato, abb->comparador);
	}
	return NULL;
}

void *abb_raiz(abb_t *abb)
{
	if (!abb || !abb->raiz) {
		return NULL;
	}

	return abb->raiz->dato;
}

void *abb_eliminar(abb_t *abb, void *dato)
{
	if (!abb || !abb->raiz) {
		return NULL;
	}

	nodo_t *padre = NULL;
	nodo_t *actual = abb->raiz;
	int cmp = 0;

	while (actual && (cmp = abb->comparador(dato, actual->dato)) != 0) {
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

size_t abb_cantidad(abb_t *abb)
{
	if (!abb) {
		return 0;
	}

	return abb->cantidad;
}

bool abb_esta_vacio(abb_t *abb)
{
	if (!abb) {
		return true;
	}

	return abb->raiz == NULL;
}

size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido modo,
			     bool (*f)(void *, void *), void *extra)
{
	if (!abb || !f)
		return 0;

	switch (modo) {
	case ABB_INORDEN:
		return abb_inorden_rec(abb->raiz, f, extra);
	case ABB_PREORDEN:
		return abb_preorden_rec(abb->raiz, f, extra);
	case ABB_POSTORDEN:
		return abb_postorden_rec(abb->raiz, f, extra);
	default:
		return 0;
	}
}

size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido,
		      size_t cant, void **vector)
{
	if (!abb || !vector || cant == 0) {
		return 0;
	}
	struct vector_contenedor vec_recibido = { .destino = vector,
						  .cap = cant,
						  .insertados = 0 };

	(void)abb_con_cada_elemento(abb, tipo_recorrido,
				    insertar_dato_en_vector, &vec_recibido);

	return vec_recibido.insertados;
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

void abb_destruir_todo(abb_t *abb, void (*destruir)(void *))
{
	if (!abb) {
		return;
	}
	destruir_nodo_rec(abb->raiz, destruir);
	free(abb);
}
