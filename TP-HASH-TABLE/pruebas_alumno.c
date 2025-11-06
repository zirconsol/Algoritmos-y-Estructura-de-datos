#include "pa2m.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <stddef.h>
#include "src/hash.h"

static bool contar_cb(char *clave, void *valor, void *ctx)
{
	(void)clave;
	(void)valor;
	size_t *c = ctx;
	(*c)++;
	return true;
}

struct corte_ctx {
	size_t limite;
	size_t llamadas;
};

static bool cortar_cb(char *clave, void *valor, void *ctx)
{
	(void)clave;
	(void)valor;
	struct corte_ctx *c = ctx;
	c->llamadas++;
	return c->llamadas < c->limite;
}

static void destruir_int(void *p)
{
	free(p);
}

void crear_hash_no_devuelve_null(void)
{
	hash_t *h = hash_crear(10);
	pa2m_afirmar(h != NULL, "Hash creado correctamente");
	if (h)
		hash_destruir(h);
}

void crear_hash_devuelve_vacio(void)
{
	hash_t *h = hash_crear(1);
	pa2m_afirmar(hash_cantidad(h) == 0, "Hash comienza vacio");
	if (h)
		hash_destruir(h);
}

void insertar_en_hash_nulo_devuelve_false(void)
{
	int v = 3;
	pa2m_afirmar(!hash_insertar(NULL, "a", &v, NULL),
		     "No inserta en hash nulo");
}

void insertar_con_clave_nula_devuelve_false(void)
{
	hash_t *h = hash_crear(5);
	int v = 1;
	pa2m_afirmar(!hash_insertar(h, NULL, &v, NULL),
		     "No inserta clave nula");
	hash_destruir(h);
}

void insertar_un_elemento_y_buscar(void)
{
	hash_t *h = hash_crear(3);
	int v = 42;
	pa2m_afirmar(hash_insertar(h, "k", &v, NULL), "Inserta elemento");
	pa2m_afirmar(hash_cantidad(h) == 1, "Cantidad es 1");
	pa2m_afirmar(hash_contiene(h, "k"), "Contiene clave insertada");
	pa2m_afirmar(hash_buscar(h, "k") == &v, "Buscar devuelve el valor");
	hash_destruir(h);
}

void insertar_valor_null_es_valido(void)
{
	hash_t *h = hash_crear(3);
	pa2m_afirmar(hash_insertar(h, "nulo", NULL, NULL),
		     "Inserta valor NULL");
	pa2m_afirmar(hash_cantidad(h) == 1, "Cantidad es 1");
	pa2m_afirmar(hash_contiene(h, "nulo"), "Contiene clave con valor NULL");
	pa2m_afirmar(hash_buscar(h, "nulo") == NULL, "Buscar devuelve NULL");
	hash_destruir(h);
}

void reemplazo_devuelve_anterior_y_mantiene_cantidad(void)
{
	hash_t *h = hash_crear(3);
	int a = 10, b = 20;
	void *anterior = (void *)0x1;
	pa2m_afirmar(hash_insertar(h, "x", &a, &anterior), "Inserta x");
	pa2m_afirmar(anterior == NULL, "Anterior es NULL al insertar nuevo");
	pa2m_afirmar(hash_insertar(h, "x", &b, &anterior), "Reemplaza x");
	pa2m_afirmar(anterior == &a, "Anterior devuelve el valor reemplazado");
	pa2m_afirmar(hash_cantidad(h) == 1, "Cantidad sigue siendo 1");
	pa2m_afirmar(hash_buscar(h, "x") == &b, "Buscar devuelve nuevo valor");
	hash_destruir(h);
}

void buscar_inexistente_devuelve_null_y_contiene_false(void)
{
	hash_t *h = hash_crear(3);
	pa2m_afirmar(hash_buscar(h, "zzz") == NULL,
		     "Buscar inexistente devuelve NULL");
	pa2m_afirmar(!hash_contiene(h, "zzz"),
		     "Contiene inexistente devuelve false");
	hash_destruir(h);
}

void quitar_existente_devuelve_valor_y_decrementa(void)
{
	hash_t *h = hash_crear(3);
	int v1 = 1, v2 = 2;
	hash_insertar(h, "a", &v1, NULL);
	hash_insertar(h, "b", &v2, NULL);
	pa2m_afirmar(hash_cantidad(h) == 2, "Cantidad es 2");
	void *ret = hash_quitar(h, "a");
	pa2m_afirmar(ret == &v1, "Quitar devuelve el valor");
	pa2m_afirmar(hash_cantidad(h) == 1, "Cantidad es 1");
	pa2m_afirmar(!hash_contiene(h, "a"), "Clave quitada ya no existe");
	hash_destruir(h);
}

void quitar_inexistente_devuelve_null(void)
{
	hash_t *h = hash_crear(3);
	int v = 1;
	hash_insertar(h, "a", &v, NULL);
	pa2m_afirmar(hash_quitar(h, "b") == NULL,
		     "Quitar inexistente devuelve NULL");
	pa2m_afirmar(hash_cantidad(h) == 1, "Cantidad no cambia");
	hash_destruir(h);
}

void iterar_aplica_a_todos(void)
{
	hash_t *h = hash_crear(7);
	int v1 = 1, v2 = 2, v3 = 3, v4 = 4;
	hash_insertar(h, "a", &v1, NULL);
	hash_insertar(h, "b", &v2, NULL);
	hash_insertar(h, "c", &v3, NULL);
	hash_insertar(h, "d", &v4, NULL);
	size_t c = 0;
	size_t llamadas = hash_iterar(h, contar_cb, &c);
	pa2m_afirmar(llamadas == 4, "Itera sobre 4 elementos");
	pa2m_afirmar(c == 4, "Callback aplicada 4 veces");
	hash_destruir(h);
}

void iterar_corta_cuando_callback_devuelve_false(void)
{
	hash_t *h = hash_crear(5);
	int v[10];
	for (int i = 0; i < 10; i++) {
		v[i] = i;
		char k[8];
		snprintf(k, sizeof k, "k%d", i);
		hash_insertar(h, k, &v[i], NULL);
	}
	struct corte_ctx ctx = { .limite = 3, .llamadas = 0 };
	size_t llamadas = hash_iterar(h, cortar_cb, &ctx);
	pa2m_afirmar(llamadas >= 3,
		     "Iteracion corta a los 3 o mas segun distribucion");
	pa2m_afirmar(ctx.llamadas == 3, "Callback se llamo 3 veces");
	hash_destruir(h);
}

void insercion_masiva_y_busqueda(void)
{
	hash_t *h = hash_crear(3);
	const size_t n = 1000;
	int *vals = malloc(sizeof(int) * n);
	bool ok = true;
	for (size_t i = 0; i < n; i++) {
		vals[i] = (int)i;
		char k[32];
		snprintf(k, sizeof k, "clave_%zu", i);
		ok = ok && hash_insertar(h, k, &vals[i], NULL);
	}
	pa2m_afirmar(ok, "Insercion masiva ok");
	pa2m_afirmar(hash_cantidad(h) == n, "Cantidad coincide");
	bool all = true;
	for (size_t i = 0; i < n; i++) {
		char k[32];
		snprintf(k, sizeof k, "clave_%zu", i);
		void *p = hash_buscar(h, k);
		if (p != &vals[i]) {
			all = false;
			break;
		}
	}
	pa2m_afirmar(all, "Todas las busquedas devuelven su valor");
	free(vals);
	hash_destruir(h);
}

void insercion_masiva_y_quitado_total(void)
{
	hash_t *h = hash_crear(3);
	const size_t n = 500;
	int *vals = malloc(sizeof(int) * n);
	for (size_t i = 0; i < n; i++) {
		vals[i] = (int)(i * 2 + 1);
		char k[32];
		snprintf(k, sizeof k, "k%zu", i);
		hash_insertar(h, k, &vals[i], NULL);
	}
	for (size_t i = 0; i < n; i++) {
		char k[32];
		snprintf(k, sizeof k, "k%zu", i);
		void *p = hash_quitar(h, k);
		if (p == NULL) {
			pa2m_afirmar(false, "Quitar debe devolver valor");
			break;
		}
	}
	pa2m_afirmar(hash_cantidad(h) == 0,
		     "Cantidad vuelve a 0 tras quitar todo");
	free(vals);
	hash_destruir(h);
}

void destruir_todo_aplicando_destructor(void)
{
	hash_t *h = hash_crear(7);
	for (int i = 0; i < 20; i++) {
		int *p = malloc(sizeof(int));
		*p = i;
		char k[16];
		snprintf(k, sizeof k, "n%d", i);
		hash_insertar(h, k, p, NULL);
	}
	hash_destruir_todo(h, destruir_int);
	pa2m_afirmar(true, "Destruir todo finalizo sin errores");
}

int main(void)
{
	pa2m_nuevo_grupo("Creacion y estado inicial");
	crear_hash_no_devuelve_null();
	crear_hash_devuelve_vacio();

	pa2m_nuevo_grupo("Insercion basica y validaciones");
	insertar_en_hash_nulo_devuelve_false();
	insertar_con_clave_nula_devuelve_false();
	insertar_un_elemento_y_buscar();
	insertar_valor_null_es_valido();
	reemplazo_devuelve_anterior_y_mantiene_cantidad();

	pa2m_nuevo_grupo("Busqueda y existencia");
	buscar_inexistente_devuelve_null_y_contiene_false();

	pa2m_nuevo_grupo("Quitar elementos");
	quitar_existente_devuelve_valor_y_decrementa();
	quitar_inexistente_devuelve_null();

	pa2m_nuevo_grupo("Iterador interno");
	iterar_aplica_a_todos();
	iterar_corta_cuando_callback_devuelve_false();

	pa2m_nuevo_grupo("Escenarios masivos y redimension");
	insercion_masiva_y_busqueda();
	insercion_masiva_y_quitado_total();

	pa2m_nuevo_grupo("Destruccion con destructor");
	destruir_todo_aplicando_destructor();

	return pa2m_mostrar_reporte();
}
