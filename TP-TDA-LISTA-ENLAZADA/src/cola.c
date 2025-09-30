#include "lista.h"
#include <stdlib.h>

typedef struct cola {
	lista_t *lista;
} cola_t;

cola_t *cola_crear(void)
{
	cola_t *cola = malloc(sizeof(cola_t));

	if (!cola) {
		return NULL;
	}
	cola->lista = lista_crear();

	if (!cola->lista) {
		free(cola);
		return NULL;
	}
	return cola;
}

bool cola_encolar(cola_t *cola, void *elemento)
{
	if (!cola) {
		return false;
	}

	return lista_agregar(cola->lista, elemento);
}

void *cola_desencolar(cola_t *cola)
{
	if (!cola) {
		return NULL;
	}

	if (lista_vacia(cola->lista)) {
		return NULL;
	}
	return lista_eliminar_elemento(cola->lista, 0);
}

void *cola_ver_primero(cola_t *cola)
{
	if (!cola) {
		return NULL;
	}

	if (lista_vacia(cola->lista)) {
		return NULL;
	}
	return lista_buscar_elemento(cola->lista, 0);
}

size_t cola_cantidad(cola_t *cola)
{
	return lista_cantidad(cola->lista);
}

void cola_destruir(cola_t *cola)
{
	if (!cola) {
		return;
	}
	lista_destruir(cola->lista);
	free(cola);
}
