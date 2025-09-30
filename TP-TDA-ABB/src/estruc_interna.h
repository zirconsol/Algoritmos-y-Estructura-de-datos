#ifndef ESTRUC_INTERNA_H_
#define ESTRUC_INTERNA_H_

#include <stdio.h>

typedef struct nodo {
	void *dato;
	struct nodo *izquierda;
	struct nodo *derecha;
} nodo_t;

struct abb_t {
	size_t nodos;
	nodo_t *raiz;
	int (*comparador)(const void *, const void *);
};

#endif
