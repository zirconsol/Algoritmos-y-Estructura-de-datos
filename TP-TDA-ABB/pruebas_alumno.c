#include "pa2m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "src/abb.h"

static int cmp_int(const void *primer_elemento, const void *segundo_elemento)
{
	return *(int *)primer_elemento - *(int *)segundo_elemento;
}

void crear_abb_no_devuelve_null(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	pa2m_afirmar(arbol != NULL, "ABB creado correctamente");

	if (arbol) {
		abb_destruir(arbol);
	}
}

void crear_abb_devuelve_abb_vacio(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	pa2m_afirmar(abb_vacio(arbol), "Arbol creado comienza vacio");

	if (arbol) {
		abb_destruir(arbol);
	}
}

void abb_con_comparador_invalido_devuelve_NULL(void)
{
	abb_t *arbol = abb_crear(NULL);

	pa2m_afirmar(arbol == NULL,
		     "ABB con comparador invalido devuelve NULL");
}

void abb_vacio_con_null_devuelve_true(void)
{
	pa2m_afirmar(abb_vacio(NULL) == true, "ABB invalido devuelve true");
}

void insertar_en_arbol_nulo_devuelve_invalido(void)
{
	int primer_elemento = 3;

	pa2m_afirmar(!abb_insertar(NULL, &primer_elemento),
		     "No se puede insertar elemento en un ABB nulo");
}

void insertar_elemento_en_abb_devuelve_abb_no_vacio(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int primer_elemento = 3;

	pa2m_afirmar(abb_insertar(arbol, &primer_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_vacio(arbol) == false, "El arbol ya no esta vacio");

	pa2m_afirmar(abb_cantidad(arbol) == 1, "El arbol tiene 1 elemento\n");

	abb_destruir(arbol);
}

void insertar_varios_elementos_cuenta_correctamente(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int primer_elemento = 3;

	int segundo_elemento = 1;

	int tercer_elemento = 5;

	int cuarto_elemento = 7;

	pa2m_afirmar(abb_insertar(arbol, &primer_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_existe(arbol, &primer_elemento),
		     "El numero 3 existe en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 1, "El arbol tiene 1 elemento");

	pa2m_afirmar(abb_insertar(arbol, &segundo_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_existe(arbol, &segundo_elemento),
		     "El numero 1 existe en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 2, "El arbol tiene 2 elementos");

	pa2m_afirmar(abb_insertar(arbol, &tercer_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_existe(arbol, &tercer_elemento),
		     "El numero 5 existe en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 3, "El arbol tiene 3 elementos");

	pa2m_afirmar(abb_insertar(arbol, &cuarto_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_existe(arbol, &primer_elemento),
		     "El numero 7 existe en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 4, "El arbol tiene 4 elementos");

	pa2m_afirmar(!abb_insertar(arbol, NULL),
		     "No se puede insertar un elemento NULL");

	pa2m_afirmar(abb_cantidad(arbol) == 4, "El arbol tiene 4 elementos");

	abb_destruir(arbol);
}

int main(void)
{
	pa2m_nuevo_grupo("Pruebas de creacion y chequeo de validez del arbol");
	crear_abb_no_devuelve_null();
	crear_abb_devuelve_abb_vacio();
	abb_con_comparador_invalido_devuelve_NULL();
	abb_vacio_con_null_devuelve_true();

	pa2m_nuevo_grupo("Pruebas de insercion de elementos en arbol");
	insertar_en_arbol_nulo_devuelve_invalido();
	insertar_elemento_en_abb_devuelve_abb_no_vacio();
	insertar_varios_elementos_cuenta_correctamente();

	return pa2m_mostrar_reporte();
}
