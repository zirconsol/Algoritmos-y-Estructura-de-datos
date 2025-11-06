#include "hash.h"
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

static const double MAX_FACTOR_DE_CARGA = 0.75;
static const size_t CAPACIDAD_MINIMA = 3;
static const size_t FACTOR_REDIM = 2;

typedef struct nodo {
	char *clave;
	void *valor;
	struct nodo *sig;
} nodo_t;

struct hash {
	size_t capacidad;
	size_t cantidad;
	nodo_t **contenedores;
	size_t (*funcion_hash)(const char *);
};

static size_t hash_djb2(const char *clave)
{
	size_t tabla_hash = 5381u;
	if (clave != NULL) {
		const unsigned char *s = (const unsigned char *)clave;
		size_t i = 0;
		while (s[i] != 0) {
			tabla_hash =
				((tabla_hash << 5) + tabla_hash) + (size_t)s[i];
			i = i + 1;
		}
	}
	return tabla_hash;
}

static char *copiar_clave(const char *clave)
{
	char *copia = NULL;
	size_t tamaño_clave = 0;
	if (clave != NULL) {
		tamaño_clave = strlen(clave) + 1;
		copia = (char *)malloc(tamaño_clave);
		if (copia != NULL) {
			memcpy(copia, clave, tamaño_clave);
		}
	}
	return copia;
}

static size_t obtener_posicion_hash(size_t valor_hash, size_t capacidad)
{
	size_t pos = 0;
	if (capacidad != 0) {
		pos = valor_hash % capacidad;
	}
	return pos;
}

static nodo_t *lista_buscar(nodo_t *primero, const char *clave,
			    nodo_t **salida_ant)
{
	nodo_t *ant = NULL;
	nodo_t *act = primero;
	nodo_t *encontrado = NULL;
	bool listo = false;

	if (salida_ant != NULL)
		*salida_ant = NULL;

	while (act != NULL && !listo) {
		if (strcmp(act->clave, clave) == 0) {
			encontrado = act;
			if (salida_ant != NULL)
				*salida_ant = ant;
			listo = true;
		} else {
			ant = act;
			act = act->sig;
		}
	}
	return encontrado;
}

static bool redimensionar(hash_t *tabla_hash, size_t nueva_cap)
{
	bool ok = false;
	nodo_t **nuevos_contenedores = NULL;
	size_t i = 0;

	if (tabla_hash != NULL) {
		if (nueva_cap < CAPACIDAD_MINIMA) {
			nueva_cap = CAPACIDAD_MINIMA;
		}
		nuevos_contenedores =
			(nodo_t **)calloc(nueva_cap, sizeof(nodo_t *));
		if (nuevos_contenedores != NULL) {
			while (i < tabla_hash->capacidad) {
				nodo_t *act = tabla_hash->contenedores[i];
				while (act != NULL) {
					nodo_t *prox = act->sig;
					size_t valor_hash =
						tabla_hash->funcion_hash(
							act->clave);
					size_t pos = obtener_posicion_hash(
						valor_hash, nueva_cap);
					act->sig = nuevos_contenedores[pos];
					nuevos_contenedores[pos] = act;
					act = prox;
				}
				i = i + 1;
			}
			free(tabla_hash->contenedores);
			tabla_hash->contenedores = nuevos_contenedores;
			tabla_hash->capacidad = nueva_cap;
			ok = true;
		}
	}
	return ok;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *tabla_hash = NULL;
	size_t cap = capacidad_inicial;

	if (cap < CAPACIDAD_MINIMA) {
		cap = CAPACIDAD_MINIMA;
	}

	tabla_hash = (hash_t *)calloc(1, sizeof(hash_t));
	if (tabla_hash != NULL) {
		tabla_hash->contenedores =
			(nodo_t **)calloc(cap, sizeof(nodo_t *));
		if (tabla_hash->contenedores == NULL) {
			free(tabla_hash);
			tabla_hash = NULL;
		} else {
			tabla_hash->capacidad = cap;
			tabla_hash->cantidad = 0;
			tabla_hash->funcion_hash =
				hash_djb2; /* hash por defecto */
		}
	}
	return tabla_hash;
}

size_t hash_cantidad(hash_t *hash)
{
	size_t cantidad = 0;
	if (hash != NULL) {
		cantidad = hash->cantidad;
	}
	return cantidad;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	bool ok = false;
	if (encontrado != NULL) {
		*encontrado = NULL;
	}
	if (hash != NULL && clave != NULL) {
		double factor_carga = (double)(hash->cantidad + 1) /
				      (double)(hash->capacidad);
		if (factor_carga > MAX_FACTOR_DE_CARGA) {
			size_t nueva = hash->capacidad * FACTOR_REDIM;
			if (!redimensionar(hash, nueva)) {
			}
		}

		size_t valor_hash = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(valor_hash, hash->capacidad);

		nodo_t *ant = NULL;
		nodo_t *nodo =
			lista_buscar(hash->contenedores[pos], clave, &ant);
		if (nodo != NULL) {
			if (encontrado != NULL) {
				*encontrado = nodo->valor;
			}
			nodo->valor = valor;
			ok = true;
		} else {
			nodo_t *nuevo = (nodo_t *)malloc(sizeof(nodo_t));
			if (nuevo != NULL) {
				char *clave_duplicada = copiar_clave(clave);
				if (clave_duplicada == NULL) {
					free(nuevo);
				} else {
					nuevo->clave = clave_duplicada;
					nuevo->valor = valor;
					nuevo->sig = hash->contenedores[pos];
					hash->contenedores[pos] = nuevo;
					hash->cantidad = hash->cantidad + 1;
					ok = true;
				}
			}
		}
	}
	return ok;
}

void *hash_buscar(hash_t *hash, char *clave)
{
	void *valor = NULL;
	if (hash != NULL && clave != NULL) {
		size_t valor_hash = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(valor_hash, hash->capacidad);
		nodo_t *nodo =
			lista_buscar(hash->contenedores[pos], clave, NULL);
		if (nodo != NULL) {
			valor = nodo->valor;
		}
	}
	return valor;
}

bool hash_contiene(hash_t *hash, char *clave)
{
	bool contiene = false;
	if (hash != NULL && clave != NULL) {
		size_t valor_hash = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(valor_hash, hash->capacidad);
		nodo_t *nodo =
			lista_buscar(hash->contenedores[pos], clave, NULL);
		if (nodo != NULL) {
			contiene = true;
		}
	}
	return contiene;
}

void *hash_quitar(hash_t *hash, char *clave)
{
	void *valor = NULL;
	if (hash != NULL && clave != NULL && hash->cantidad > 0) {
		size_t valor_hash = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(valor_hash, hash->capacidad);
		nodo_t *ant = NULL;
		nodo_t *nodo =
			lista_buscar(hash->contenedores[pos], clave, &ant);
		if (nodo != NULL) {
			valor = nodo->valor;
			if (ant == NULL) {
				hash->contenedores[pos] = nodo->sig;
			} else {
				ant->sig = nodo->sig;
			}
			nodo->sig = NULL;
			free(nodo->clave);
			free(nodo);
			hash->cantidad = hash->cantidad - 1;
		}
	}
	return valor;
}

size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx)
{
	size_t llamadas = 0;
	if (hash != NULL && f != NULL) {
		size_t i = 0;
		bool seguir = true;
		while (i < hash->capacidad && seguir) {
			nodo_t *act = hash->contenedores[i];
			while (act != NULL && seguir) {
				seguir = f(act->clave, act->valor, ctx);
				llamadas = llamadas + 1;
				if (seguir) {
					act = act->sig;
				}
			}
			if (seguir) {
				i = i + 1;
			}
		}
	}
	return llamadas;
}

void hash_destruir(hash_t *hash)
{
	size_t i = 0;
	if (hash != NULL) {
		while (i < hash->capacidad) {
			nodo_t *act = hash->contenedores[i];
			while (act != NULL) {
				nodo_t *prox = act->sig;
				act->sig = NULL;
				free(act->clave);
				free(act);
				act = prox;
			}
			i = i + 1;
		}
		free(hash->contenedores);
		free(hash);
	}
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	size_t i = 0;
	if (hash != NULL) {
		while (i < hash->capacidad) {
			nodo_t *act = hash->contenedores[i];
			while (act != NULL) {
				nodo_t *prox = act->sig;
				act->sig = NULL;
				if (destructor != NULL) {
					destructor(act->valor);
				}
				free(act->clave);
				free(act);
				act = prox;
			}
			i = i + 1;
		}
		free(hash->contenedores);
		free(hash);
	}
}
