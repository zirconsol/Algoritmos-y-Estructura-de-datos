#include "lista.h"
#include <stdlib.h>

typedef struct nodo {
	void *dato;
	struct nodo *siguiente;
} nodo_t;

struct lista {
	nodo_t *primero;
	size_t cantidad;
	nodo_t *ultimo;
};

typedef struct lista_iterador {
	lista_t *lista;
	nodo_t *actual;
} lista_iterador;

lista_t *lista_crear(void)
{
	lista_t *lista = malloc(sizeof(lista_t));

	if (!lista) {
		return NULL;
	}

	lista->primero = NULL;
	lista->ultimo = NULL;
	lista->cantidad = 0;

	return lista;
}

bool lista_vacia(lista_t *lista)
{
	if (!lista) {
		return true;
	}
	return lista->primero == NULL;
}

size_t lista_cantidad(lista_t *lista)
{
	if (!lista) {
		return 0;
	}
	return lista->cantidad;
}

bool lista_agregar(lista_t *lista, void *dato)
{
	if (!lista) {
		return false;
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));
	if (!nuevo_nodo) {
		return false;
	}

	nuevo_nodo->dato = dato;
	nuevo_nodo->siguiente = NULL;

	if (lista->cantidad == 0) {
		lista->primero = lista->ultimo = nuevo_nodo;
	} else {
		lista->ultimo->siguiente = nuevo_nodo;
		lista->ultimo = nuevo_nodo;
	}

	lista->cantidad++;
	return true;
}

bool lista_insertar(lista_t *lista, void *elemento, size_t posicion)
{
	if (!lista) {
		return false;
	}

	if (lista->cantidad == 0) {
		return false;
	}

	if (posicion > lista->cantidad) {
		return false;
	}

	nodo_t *nuevo_nodo = malloc(sizeof(nodo_t));

	if (!nuevo_nodo) {
		return false;
	}

	nuevo_nodo->dato = elemento;
	nuevo_nodo->siguiente = NULL;

	if (posicion == 0) {
		nuevo_nodo->siguiente = lista->primero;
		lista->primero = nuevo_nodo;
		if (lista->cantidad == 0) {
			lista->ultimo = nuevo_nodo;
		}
	} else if (posicion == lista->cantidad) {
		lista->ultimo->siguiente = nuevo_nodo;
		lista->ultimo = nuevo_nodo;

	} else {
		nodo_t *actual = lista->primero;
		for (size_t i = 0; i < posicion - 1; i++) {
			actual = actual->siguiente;
		}
		nuevo_nodo->siguiente = actual->siguiente;
		actual->siguiente = nuevo_nodo;
	}
	lista->cantidad++;
	return true;
}

void *lista_eliminar_primer_elemento(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}
	nodo_t *borrar_nodo = NULL;
	void *valor = NULL;

	borrar_nodo = lista->primero;
	valor = borrar_nodo->dato;
	lista->primero = borrar_nodo->siguiente;

	if (lista->cantidad == 1) {
		lista->ultimo = NULL;
	}
	free(borrar_nodo);
	lista->cantidad--;
	return valor;
}

void *lista_eliminar_ultimo_elemento(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	if (lista->cantidad == 0) {
		return NULL;
	}

	nodo_t *borrar_nodo = NULL;
	void *valor = NULL;

	if (lista->cantidad == 1) {
		borrar_nodo = lista->primero;
		valor = borrar_nodo->dato;
		lista->primero = NULL;
		lista->ultimo = NULL;
	} else {
		nodo_t *anterior = lista->primero;
		while (anterior->siguiente != lista->ultimo) {
			anterior = anterior->siguiente;
		}
		borrar_nodo = lista->ultimo;
		valor = borrar_nodo->dato;
		anterior->siguiente = NULL;
		lista->ultimo = anterior;
	}
	free(borrar_nodo);
	lista->cantidad--;
	return valor;
}

void *lista_eliminar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista) {
		return NULL;
	}
	if (posicion >= lista->cantidad) {
		return NULL;
	}

	if (posicion == 0) {
		return lista_eliminar_primer_elemento(lista);
	}
	if (posicion == lista->cantidad - 1) {
		return lista_eliminar_ultimo_elemento(lista);
	}

	nodo_t *anterior = lista->primero;
	for (size_t i = 0; i < posicion - 1; i++) {
		anterior = anterior->siguiente;
	}

	nodo_t *borrar_nodo = anterior->siguiente;
	void *valor = borrar_nodo->dato;

	anterior->siguiente = borrar_nodo->siguiente;

	free(borrar_nodo);
	lista->cantidad--;
	return valor;
}

int lista_buscar_posicion(lista_t *lista, void *dato,
			  int (*comparador)(const void *, const void *))
{
	if (!lista) {
		return -1;
	}

	if (!comparador) {
		return -1;
	}

	struct nodo *actual = lista->primero;

	int posicion = 0;

	while (actual) {
		if (comparador(actual->dato, dato) == 0) {
			return posicion;
		}
		actual = actual->siguiente;
		posicion++;
	}
	return -1;
}

void *lista_buscar_elemento(lista_t *lista, size_t posicion)
{
	if (!lista) {
		return NULL;
	}

	if (posicion >= lista->cantidad) {
		return NULL;
	}

	struct nodo *actual = lista->primero;
	size_t pos_lista = 0;

	while (actual) {
		if (pos_lista == posicion) {
			return actual->dato;
		}
		actual = actual->siguiente;
		pos_lista++;
	}
	return NULL;
}

size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *),
			       void *extra)
{
	size_t nodos_visitados = 0;

	if (lista && f) {
		nodo_t *actual = lista->primero;
		bool cortar_ciclo = false;

		while (actual && !cortar_ciclo) {
			bool seguir_iterando = f(actual->dato, extra);
			nodos_visitados++;
			if (!seguir_iterando) {
				cortar_ciclo = true;
			}
			actual = actual->siguiente;
		}
	}
	return nodos_visitados;
}

void lista_destruir_todo(lista_t *lista, void (*destructor)(void *))
{
	if (!lista) {
		return;
	}

	nodo_t *actual = lista->primero;
	while (actual) {
		nodo_t *proximo = actual->siguiente;
		if (destructor) {
			destructor(actual->dato);
		}
		free(actual);
		actual = proximo;
	}
	free(lista);
}

void lista_destruir(lista_t *lista)
{
	if (!lista) {
		return;
	}

	nodo_t *actual = lista->primero;
	while (actual) {
		nodo_t *prox = actual->siguiente;
		free(actual);
		actual = prox;
	}
	free(lista);
}

lista_iterador_t *lista_iterador_crear(lista_t *lista)
{
	if (!lista) {
		return NULL;
	}

	lista_iterador_t *it = malloc(sizeof(lista_iterador_t));

	if (!it) {
		return NULL;
	}

	it->lista = lista;
	it->actual = lista->primero;
	return it;
}

bool lista_iterador_hay_mas_elementos(lista_iterador_t *it)
{
	return it && it->actual != NULL;
}

void lista_iterador_siguiente(lista_iterador_t *it)
{
	if (!it || !it->actual) {
		return;
	}
	it->actual = it->actual->siguiente;
}

void *lista_iterador_obtener_actual(lista_iterador_t *it)
{
	if (!it || !it->actual) {
		return NULL;
	}
	return it->actual->dato;
}

void lista_iterador_destruir(lista_iterador_t *it)
{
	free(it);
}
