#include "pa2m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "src/abb.h"

struct recolector {
	char *buf;
	size_t cap;
	size_t len;
};

bool imprimir_elemento(void *dato, void *extra)
{
	(void)extra;
	printf("%d ", *(int *)dato);
	return true;
}

bool concatenar_ints_en_string(void *dato, void *extra)
{
	struct recolector *recolector = extra;
	int v = *(int *)dato;
	int escritos = snprintf(recolector->buf + recolector->len,
				recolector->cap - recolector->len,
				(recolector->len == 0) ? "%d" : " %d", v);

	if (escritos < 0)
		return false;
	if ((size_t)escritos >= recolector->cap - recolector->len)
		return false;
	recolector->len += (size_t)escritos;
	return true;
}

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

	pa2m_afirmar(abb_esta_vacio(arbol), "Arbol creado comienza vacio");

	if (arbol) {
		abb_destruir(arbol);
	}
}

void abb_con_comparador_invalido_devuelve_NULL(void)
{
	abb_t *arbol = abb_crear(NULL);

	pa2m_afirmar(arbol == NULL,
		     "ABB con comparador invalido devuelve NULL");

	abb_destruir(arbol);
}

void abb_vacio_con_null_devuelve_true(void)
{
	pa2m_afirmar(abb_esta_vacio(NULL) == true,
		     "ABB invalido devuelve true");
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

	pa2m_afirmar(abb_esta_vacio(arbol) == false,
		     "El arbol ya no esta vacio");

	pa2m_afirmar(abb_cantidad(arbol) == 1, "El arbol tiene 1 elemento\n");

	abb_destruir(arbol);
}

void insertar_null_no_inserta_en_arbol(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int primer_elemento = 3;

	pa2m_afirmar(abb_insertar(arbol, &primer_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(abb_existe(arbol, &primer_elemento),
		     "El numero 3 existe en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 1, "El arbol tiene 1 elemento");

	pa2m_afirmar(abb_insertar(arbol, NULL),
		     "Se puede insertar un elemento NULL");

	pa2m_afirmar(abb_cantidad(arbol) == 2, "El arbol tiene 2 elementos");
}

void insertar_varios_elementos_correctamente(void)
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

	pa2m_afirmar(abb_cantidad(arbol) == 4, "El arbol tiene 4 elementos\n");

	abb_destruir(arbol);
}

void chequear_raiz_al_insertar_varios_elementos(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int primer_elemento = 3;

	int segundo_elemento = 1;

	int tercer_elemento = 5;

	int cuarto_elemento = 7;

	abb_insertar(arbol, &primer_elemento);
	pa2m_afirmar(abb_raiz(arbol) == &primer_elemento,
		     "La raiz del arbol es el numero 3");
	pa2m_afirmar(abb_insertar(arbol, &segundo_elemento),
		     "Se inserto el elemento en el arbol");
	pa2m_afirmar(abb_existe(arbol, &segundo_elemento),
		     "El numero 1 existe en el arbol");
	pa2m_afirmar(abb_raiz(arbol) == &primer_elemento,
		     "La raiz del arbol es el numero 3");
	pa2m_afirmar(abb_cantidad(arbol) == 2, "El arbol tiene 2 elementos");
	pa2m_afirmar(abb_insertar(arbol, &tercer_elemento),
		     "Se inserto el elemento en el arbol");
	pa2m_afirmar(abb_existe(arbol, &tercer_elemento),
		     "El numero 5 existe en el arbol");
	pa2m_afirmar(abb_raiz(arbol) == &primer_elemento,
		     "La raiz del arbol es el numero 3");
	pa2m_afirmar(abb_cantidad(arbol) == 3, "El arbol tiene 3 elementos");
	pa2m_afirmar(abb_insertar(arbol, &cuarto_elemento),
		     "Se inserto el elemento en el arbol");
	pa2m_afirmar(abb_existe(arbol, &primer_elemento),
		     "El numero 7 existe en el arbol");
	pa2m_afirmar(abb_cantidad(arbol) == 4, "El arbol tiene 4 elementos");
	abb_destruir(arbol);
}

void eliminar_un_nodo_hoja_del_abb(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int vector[] = { 4, 2, 6 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	pa2m_afirmar(abb_cantidad(arbol) == 3, "El arbol tiene 3 elementos");

	pa2m_afirmar(abb_eliminar(arbol, &vector[2]) == &vector[2],
		     "Se elimino correctamente el numero 6");
	pa2m_afirmar(!abb_existe(arbol, &vector[2]),
		     "El numero 6 no existe en el arbol");
	pa2m_afirmar(abb_cantidad(arbol) == 2, "El arbol tiene 2 elementos");

	pa2m_afirmar(abb_existe(arbol, &vector[0]),
		     "El numero 4 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[1]),
		     "El numero 2 esta en el arbol");
	abb_destruir(arbol);
}

void eliminar_un_nodo_con_un_hijo_del_abb(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int vector[] = { 4, 2, 6, 1 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;

	pa2m_afirmar(abb_cantidad(arbol) == 4, "El arbol tiene 4 elementos");

	pa2m_afirmar(abb_eliminar(arbol, &vector[1]) == &vector[1],
		     "Se elimino correctamente el numero 2");

	abb_con_cada_elemento(arbol, ABB_PREORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "4 1 6") == 0, "Preorden correcto");

	pa2m_afirmar(abb_existe(arbol, &vector[0]),
		     "El numero 4 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[2]),
		     "El numero 1 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[3]),
		     "El numero 6 esta en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 3, "El arbol tiene 3 elementos");
	abb_destruir(arbol);
}

void eliminar_un_nodo_con_dos_hijos_del_abb(void)
{
	abb_t *arbol = abb_crear(cmp_int);

	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;

	pa2m_afirmar(abb_cantidad(arbol) == 7, "El arbol tiene 7 elementos");

	pa2m_afirmar(abb_eliminar(arbol, &vector[2]) == &vector[2],
		     "Se elimino correctamente el numero 6");

	abb_con_cada_elemento(arbol, ABB_PREORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "4 2 1 3 5 7") == 0, "Preorden correcto");

	pa2m_afirmar(abb_existe(arbol, &vector[0]),
		     "El numero 4 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[1]),
		     "El numero 2 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[3]),
		     "El numero 1 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[4]),
		     "El numero 3 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[5]),
		     "El numero 5 esta en el arbol");
	pa2m_afirmar(abb_existe(arbol, &vector[6]),
		     "El numero 7 esta en el arbol");

	pa2m_afirmar(abb_cantidad(arbol) == 6, "El arbol tiene 6 elementos");

	abb_destruir(arbol);
}

void abb_existe_con_inexistente_devuelve_false(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int elemento_sin_insertar = 10;
	int primer_elemento = 3;
	pa2m_afirmar(abb_insertar(arbol, &primer_elemento),
		     "Se inserto el elemento en el arbol");

	pa2m_afirmar(!abb_existe(arbol, &elemento_sin_insertar),
		     "El elemento no existe en el arbol");

	abb_destruir(arbol);
}

void buscar_con_inexistente_devuelve_null(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int elemento_sin_insertar = 10;

	pa2m_afirmar(abb_buscar(arbol, &elemento_sin_insertar) == NULL,
		     "El elemento no se encontro al buscar");

	abb_destruir(arbol);
}

void buscar_elemento_del_arbol_lo_devuelve(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int elemento = 10;
	abb_insertar(arbol, &elemento);
	pa2m_afirmar(abb_buscar(arbol, &elemento) != NULL,
		     "El elemento fue encontrado en el arbol");
	abb_destruir(arbol);
}

void prueba_recorridos_in_orden_con_string(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_INORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "1 2 3 4 5 6 7") == 0, "Inorden correcto");
	abb_destruir(arbol);
}

void abb_desordenado_comparado_con_inorden_da_falso(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);
	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_PREORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "1 2 3 4 5 6 7") != 0,
		     "El array no esta inorden");
	abb_destruir(arbol);
}

void prueba_recorridos_pre_orden_con_string(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_PREORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "4 2 1 3 6 5 7") == 0, "Preorden correcto");
	abb_destruir(arbol);
}

void prueba_recorridos_post_orden_con_string(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_POSTORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "1 3 2 5 7 6 4") == 0, "Postorden correcto");

	abb_destruir(arbol);
}

void abb_desordenado_comparado_con_postorden_da_falso(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_INORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "1 3 2 5 7 6 4") != 0,
		     "El array no esta en postorden");

	abb_destruir(arbol);
}

void abb_desordenado_comparado_con_preorden_da_falso(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	char buf[128];
	struct recolector recolector = { .buf = buf,
					 .cap = sizeof buf,
					 .len = 0 };

	recolector.len = 0;
	abb_con_cada_elemento(arbol, ABB_INORDEN, concatenar_ints_en_string,
			      &recolector);
	pa2m_afirmar(strcmp(buf, "4 2 1 3 6 5 7") != 0,
		     "El array no esta en preorden");

	abb_destruir(arbol);
}

void vectorizar_tres_elem_de_arbol_inorden(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	void *vec[3];
	size_t cantidad = 0;

	cantidad = abb_vectorizar(arbol, ABB_INORDEN, 3, vec);
	pa2m_afirmar(cantidad == 3,
		     "Se vectorizaron 3 elementos del arbol en inorden");
	pa2m_afirmar(*(int *)vec[0] == 1 && *(int *)vec[1] == 2 &&
			     *(int *)vec[2] == 3,
		     "Orden inorden correcto: 1,2,3");
	abb_destruir(arbol);
}

void vectorizar_tres_elem_de_arbol_preorden(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	void *vec[3];
	size_t cantidad = 0;

	cantidad = abb_vectorizar(arbol, ABB_PREORDEN, 3, vec);
	pa2m_afirmar(cantidad == 3,
		     "Se vectorizaron 3 elementos del arbol en preorden");
	pa2m_afirmar(*(int *)vec[0] == 4 && *(int *)vec[1] == 2 &&
			     *(int *)vec[2] == 1,
		     "Orden inorden correcto: 4,2,1");
	abb_destruir(arbol);
}

void vectorizar_tres_elem_de_arbol_postorden(void)
{
	abb_t *arbol = abb_crear(cmp_int);
	int vector[] = { 4, 2, 6, 1, 3, 5, 7 };
	for (size_t i = 0; i < sizeof vector / sizeof vector[0]; i++)
		abb_insertar(arbol, &vector[i]);

	void *vec[3];
	size_t cantidad = 0;

	cantidad = abb_vectorizar(arbol, ABB_POSTORDEN, 3, vec);
	pa2m_afirmar(cantidad == 3,
		     "Se vectorizaron 3 elementos del arbol en preorden");
	pa2m_afirmar(*(int *)vec[0] == 1 && *(int *)vec[1] == 3 &&
			     *(int *)vec[2] == 2,
		     "Orden inorden correcto: 1,3,2");
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
	insertar_null_no_inserta_en_arbol();
	insertar_varios_elementos_correctamente();
	chequear_raiz_al_insertar_varios_elementos();

	pa2m_nuevo_grupo("Pruebas de eliminacion de elementos de un arbol");
	eliminar_un_nodo_hoja_del_abb();
	eliminar_un_nodo_con_un_hijo_del_abb();
	eliminar_un_nodo_con_dos_hijos_del_abb();

	pa2m_nuevo_grupo("Pruebas de busqueda de elementos en arbol");
	abb_existe_con_inexistente_devuelve_false();
	buscar_con_inexistente_devuelve_null();
	buscar_elemento_del_arbol_lo_devuelve();

	pa2m_nuevo_grupo("Pruebas de busqueda de elementos en arbol");
	prueba_recorridos_in_orden_con_string();
	prueba_recorridos_pre_orden_con_string();
	prueba_recorridos_post_orden_con_string();
	abb_desordenado_comparado_con_inorden_da_falso();
	abb_desordenado_comparado_con_postorden_da_falso();
	abb_desordenado_comparado_con_preorden_da_falso();

	pa2m_nuevo_grupo("Pruebas de vectorizacion de elementos del arbol");
	vectorizar_tres_elem_de_arbol_inorden();
	vectorizar_tres_elem_de_arbol_preorden();
	vectorizar_tres_elem_de_arbol_postorden();

	return pa2m_mostrar_reporte();
}
