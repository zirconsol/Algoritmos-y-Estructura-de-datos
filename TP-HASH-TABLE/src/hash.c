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
	nodo_t **buckets;
	size_t (*funcion_hash)(const char *);
};

static size_t hash_djb2(const char *clave)
{
	size_t h = 5381u;
	if (clave != NULL) {
		const unsigned char *s = (const unsigned char *)clave;
		size_t i = 0;
		while (s[i] != 0) {
			h = ((h << 5) + h) + (size_t)s[i];
			i = i + 1;
		}
	}
	return h;
}

static char *copiar_clave(const char *clave)
{
	char *copia = NULL;
	size_t n = 0;
	if (clave != NULL) {
		n = strlen(clave) + 1;
		copia = (char *)malloc(n);
		if (copia != NULL) {
			memcpy(copia, clave, n);
		}
	}
	return copia;
}

static size_t obtener_posicion_hash(size_t hv, size_t capacidad)
{
	size_t pos = 0;
	if (capacidad != 0) {
		pos = hv % capacidad;
	}
	return pos;
}

static nodo_t *lista_buscar(nodo_t *primero, const char *clave,
			    nodo_t **out_ant)
{
	nodo_t *ant = NULL;
	nodo_t *act = primero;
	nodo_t *encontrado = NULL;
	bool listo = false;

	if (out_ant != NULL)
		*out_ant = NULL;

	while (act != NULL && !listo) {
		if (strcmp(act->clave, clave) == 0) {
			encontrado = act;
			if (out_ant != NULL)
				*out_ant = ant;
			listo = true;
		} else {
			ant = act;
			act = act->sig;
		}
	}
	return encontrado;
}

static bool redimensionar(hash_t *h, size_t nueva_cap)
{
	bool ok = false;
	nodo_t **nuevos = NULL;
	size_t i = 0;

	if (h != NULL) {
		if (nueva_cap < CAPACIDAD_MINIMA) {
			nueva_cap = CAPACIDAD_MINIMA;
		}
		nuevos = (nodo_t **)calloc(nueva_cap, sizeof(nodo_t *));
		if (nuevos != NULL) {
			while (i < h->capacidad) {
				nodo_t *act = h->buckets[i];
				while (act != NULL) {
					nodo_t *prox = act->sig;
					size_t hv = h->funcion_hash(act->clave);
					size_t pos = obtener_posicion_hash(
						hv, nueva_cap);
					act->sig = nuevos[pos];
					nuevos[pos] = act;
					act = prox;
				}
				i = i + 1;
			}
			free(h->buckets);
			h->buckets = nuevos;
			h->capacidad = nueva_cap;
			ok = true;
		}
	}
	return ok;
}

hash_t *hash_crear(size_t capacidad_inicial)
{
	hash_t *h = NULL;
	size_t cap = capacidad_inicial;

	if (cap < CAPACIDAD_MINIMA) {
		cap = CAPACIDAD_MINIMA;
	}

	h = (hash_t *)calloc(1, sizeof(hash_t));
	if (h != NULL) {
		h->buckets = (nodo_t **)calloc(cap, sizeof(nodo_t *));
		if (h->buckets == NULL) {
			free(h);
			h = NULL;
		} else {
			h->capacidad = cap;
			h->cantidad = 0;
			h->funcion_hash = hash_djb2; /* hash por defecto */
		}
	}
	return h;
}

size_t hash_cantidad(hash_t *hash)
{
	size_t c = 0;
	if (hash != NULL) {
		c = hash->cantidad;
	}
	return c;
}

bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
{
	bool ok = false;
	if (encontrado != NULL) {
		*encontrado = NULL;
	}
	if (hash != NULL && clave != NULL) {
		/* Check factor de carga y crecer si es necesario */
		double fc = (double)(hash->cantidad + 1) /
			    (double)(hash->capacidad);
		if (fc > MAX_FACTOR_DE_CARGA) {
			size_t nueva = hash->capacidad * FACTOR_REDIM;
			if (!redimensionar(hash, nueva)) {
				/* Si no se pudo redimensionar, seguimos intentando insertar igual */
			}
		}

		size_t hv = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(hv, hash->capacidad);

		/* Buscar si la clave existe en la lista del bucket */
		nodo_t *ant = NULL;
		nodo_t *nodo = lista_buscar(hash->buckets[pos], clave, &ant);
		if (nodo != NULL) {
			if (encontrado != NULL) {
				*encontrado = nodo->valor;
			}
			nodo->valor = valor;
			ok = true;
		} else {
			/* Crear nodo nuevo al frente */
			nodo_t *nuevo = (nodo_t *)malloc(sizeof(nodo_t));
			if (nuevo != NULL) {
				char *kdup = copiar_clave(clave);
				if (kdup == NULL) {
					free(nuevo);
				} else {
					nuevo->clave = kdup;
					nuevo->valor = valor;
					nuevo->sig = hash->buckets[pos];
					hash->buckets[pos] = nuevo;
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
		size_t hv = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(hv, hash->capacidad);
		nodo_t *nodo = lista_buscar(hash->buckets[pos], clave, NULL);
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
		size_t hv = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(hv, hash->capacidad);
		nodo_t *nodo = lista_buscar(hash->buckets[pos], clave, NULL);
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
		size_t hv = hash->funcion_hash(clave);
		size_t pos = obtener_posicion_hash(hv, hash->capacidad);
		nodo_t *ant = NULL;
		nodo_t *nodo = lista_buscar(hash->buckets[pos], clave, &ant);
		if (nodo != NULL) {
			valor = nodo->valor;
			if (ant == NULL) {
				hash->buckets[pos] = nodo->sig;
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
			nodo_t *act = hash->buckets[i];
			while (act != NULL && seguir) {
				seguir = f(act->clave, act->valor, ctx);
				llamadas =
					llamadas +
					1; /* se cuenta esta llamada aunque seguir sea false */
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
			nodo_t *act = hash->buckets[i];
			while (act != NULL) {
				nodo_t *prox = act->sig;
				act->sig = NULL;
				free(act->clave);
				free(act);
				act = prox;
			}
			i = i + 1;
		}
		free(hash->buckets);
		free(hash);
	}
}

void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))
{
	size_t i = 0;
	if (hash != NULL) {
		while (i < hash->capacidad) {
			nodo_t *act = hash->buckets[i];
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
		free(hash->buckets);
		free(hash);
	}
}