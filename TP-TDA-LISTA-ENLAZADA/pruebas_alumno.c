#include "pa2m.h"
#include "src/lista.h"
#include "src/pila.h"
#include "src/cola.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>

bool iterar_siempre(void *dato, void *extra)
{
	(void)dato;
	(void)extra;
	return true;
}

bool imprimir_menor_a_diez(void *dato, void *extra)
{
	(void)extra;
	int valor = *(int *)dato;

	if (valor < 10) {
		printf("%d < 10\n", valor);
	} else {
		printf("Valor invalido, %d < 10\n", valor);
	}

	return valor < 10;
}

void lista_recien_creada_vacia_devuelve_true(void)
{
	lista_t *lista = lista_crear();

	pa2m_afirmar(lista_vacia(lista), "Lista recien creada, comienza vacia");

	lista_destruir(lista);
}

void lista_recien_creada_con_un_elemento_devuelve_false(void)
{
	lista_t *lista = lista_crear();

	int valor = 5;

	lista_agregar(lista, &valor);

	pa2m_afirmar(!lista_vacia(lista),
		     "La lista creada tiene un elemento, no esta vacia");

	lista_destruir(lista);
}

void lista_con_un_elemento_devuelve_cantidad(void)
{
	lista_t *lista = lista_crear();

	int valor = 5;

	lista_agregar(lista, &valor);

	pa2m_afirmar(lista_cantidad(lista) == 1, "La lista tiene 1 elemento");

	printf("Lista tiene %zu elementos", lista_cantidad(lista));

	lista_destruir(lista);
}

void agregar_elem_funciona_correctamente_y_aumenta_cantidad(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;

	int seg_elem = 8;

	pa2m_afirmar(lista_vacia(lista), "La lista comienza vacia");

	pa2m_afirmar(lista_agregar(lista, &primer_elem), "Se agrego el 5");
	pa2m_afirmar(lista_cantidad(lista) == 1, "La lista tiene 1 elemento");

	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_cantidad(lista) == 2, "La lista tiene 2 elementos");

	lista_destruir(lista);
}

void agregar_elemento_nulo(void)
{
	lista_t *lista = lista_crear();

	pa2m_afirmar(lista_agregar(lista, NULL),
		     "Se pudo agregar un elemento NULL en la lista vacia");

	pa2m_afirmar(lista_cantidad(lista) == 1,
		     "La lista tiene 1 elemento después de agregar NULL");
	lista_destruir(lista);
}

void insertar_en_posicion_cero_correctamente(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 12;
	int cuarto_elem = 21;

	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &tercer_elem), "Se agrego el 12");
	pa2m_afirmar(lista_insertar(lista, &primer_elem, 0),
		     "Se inserto el 5 en la posicion 0");
	pa2m_afirmar(lista_agregar(lista, &cuarto_elem), "Se agrego el 21");

	int *pos_cero = (int *)lista_buscar_elemento(lista, 0);
	pa2m_afirmar(pos_cero && *pos_cero == 5,
		     "El numero 5 sen encuentra en la posicion 0");
	lista_destruir(lista);
}

void insertar_en_ultima_posicion_correctamente(void)
{
	lista_t *lista = lista_crear();

	int seg_elem = 8;
	int tercer_elem = 12;
	int cuarto_elem = 21;

	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &tercer_elem), "Se agrego el 12");
	pa2m_afirmar(lista_cantidad(lista) == 2, "La lista tiene 2 elementos");

	int *ultima_pos = (int *)lista_buscar_elemento(lista, 1);
	pa2m_afirmar(ultima_pos && *ultima_pos == 12,
		     "El numero 12 se encuentra en ultima posicion (indice 1)");

	pa2m_afirmar(lista_insertar(lista, &cuarto_elem, lista_cantidad(lista)),
		     "Se inserto el 21 en la ultima posicion (indice 2)");
	pa2m_afirmar(lista_cantidad(lista) == 3, "La lista tiene 3 elementos");

	int *nuevo_ultimo = (int *)lista_buscar_elemento(lista, 2);
	pa2m_afirmar(nuevo_ultimo && *nuevo_ultimo == 21,
		     "El nuevo ultimo es 21 (indice 2)");
	lista_destruir(lista);
}

void insertar_en_posicion_intermedia_correctamente(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 12;
	int cuarto_elem = 21;

	pa2m_afirmar(lista_agregar(lista, &primer_elem), "Se agrego el 5");
	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &cuarto_elem), "Se agrego el 21");
	pa2m_afirmar(lista_insertar(lista, &tercer_elem, 2),
		     "Se inserto el 12 en la posicion 2");

	int *segunda_pos = (int *)lista_buscar_elemento(lista, 2);
	pa2m_afirmar(segunda_pos && *segunda_pos == 12,
		     "El elemento en la posicion 2 es el 12");
	lista_destruir(lista);
}

void insertar_fuera_de_rango_devuelve_false(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 12;
	int cuarto_elem = 21;

	bool en_rango = lista_insertar(lista, &tercer_elem, 5);

	pa2m_afirmar(lista_agregar(lista, &primer_elem), "Se agrego el 5");
	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &cuarto_elem), "Se agrego el 21");

	pa2m_afirmar(!en_rango,
		     "Insertar en posicion fuera de rango devuelve false");

	pa2m_afirmar(lista_cantidad(lista) == 3,
		     "La lista sigue teniendo 3 elementos");
}

void eliminar_elemento_de_lista_devuelve_lista_sin_ese_elemento(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 25;

	pa2m_afirmar(lista_agregar(lista, &primer_elem), "Se agrego el 5");
	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &tercer_elem), "Se agrego el 25");

	pa2m_afirmar(lista_cantidad(lista) == 3,
		     "La lista tiene tres elementos");

	int *eliminado = (int *)lista_eliminar_elemento(lista, 1);
	pa2m_afirmar(eliminado && *eliminado == 8, "Se elimino el 8");

	pa2m_afirmar(lista_cantidad(lista) == 2,
		     "La lista tiene tiene 2 elementos\n");

	lista_destruir(lista);
}

void eliminar_ultimo_elemento_de_lista_y_actualizar_lista(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 25;

	pa2m_afirmar(lista_agregar(lista, &primer_elem), "Se agrego el 5");
	pa2m_afirmar(lista_agregar(lista, &seg_elem), "Se agrego el 8");
	pa2m_afirmar(lista_agregar(lista, &tercer_elem), "Se agrego el 25");

	pa2m_afirmar(lista_cantidad(lista) == 3,
		     "La lista tiene tres elementos");

	int *eliminado = (int *)lista_eliminar_elemento(
		lista, lista_cantidad(lista) - 1);

	pa2m_afirmar(eliminado && *eliminado == 25, "Se elimino el 25");

	int *nuevo_ultimo =
		(int *)lista_buscar_elemento(lista, lista_cantidad(lista) - 1);
	pa2m_afirmar(
		nuevo_ultimo && *nuevo_ultimo == 8,
		"El ultimo elemento de la lista luego de eliminar un elemento es 8\n");
	lista_destruir(lista);
}

void iterador_interno_devuelve_todos_los_elementos(void)
{
	lista_t *lista = lista_crear();
	int primer_elem = 5;
	int seg_elem = 8;

	lista_agregar(lista, &primer_elem);
	lista_agregar(lista, &seg_elem);

	size_t recorridos =
		lista_con_cada_elemento(lista, iterar_siempre, NULL);

	pa2m_afirmar(
		recorridos == 2,
		"El iterador interno visitó los 3 elementos de la lista\n");

	lista_destruir(lista);
}

void iterador_interno_se_detiene_con_elem_mayor_a_diez(void)
{
	lista_t *lista = lista_crear();

	int primer_elem = 5;
	int seg_elem = 8;
	int tercer_elem = 25;

	lista_agregar(lista, &primer_elem);
	lista_agregar(lista, &seg_elem);
	lista_agregar(lista, &tercer_elem);

	size_t recorridos =
		lista_con_cada_elemento(lista, imprimir_menor_a_diez, NULL);

	pa2m_afirmar(
		recorridos == 3,
		"El iterador se detuvo en el tercer elemento porque 25 > 10");

	lista_destruir(lista);
}

void lista_destruir_no_rompe(void)
{
	lista_destruir(NULL);
	pa2m_afirmar(true, "lista_destruir(NULL) no rompe");
}

//Pila

void crear_pila_con_elemento_null(void)
{
	pila_t *pila = pila_crear();

	pa2m_afirmar(pila != NULL, "Se pudo crear la pila");

	pa2m_afirmar(pila_ver_primero(pila) == NULL, "La pila inicia vacia");

	pila_destruir(pila);
}

void apilar_null(void)
{
	pila_t *pila = pila_crear();

	pa2m_afirmar(pila_apilar(pila, NULL),
		     "Se puede apilar un elemento NULL");

	pa2m_afirmar(pila_cantidad(pila) == 1, "La pila tiene 1 elemento");

	pila_destruir(pila);
}

void apilar_elemento(void)
{
	pila_t *pila = pila_crear();

	int primer_elem = 25;
	int seg_elem = 18;
	int tercer_elem = 5;

	pa2m_afirmar(pila_apilar(pila, &primer_elem), "Se apilo 25");
	pa2m_afirmar(pila_apilar(pila, &seg_elem), "Se apilo 8");
	pa2m_afirmar(pila_apilar(pila, &tercer_elem), "Se apilo 5");

	int *tope = pila_ver_primero(pila);

	pa2m_afirmar(tope && *tope == 5,
		     "Se apilaron correctamente, el tope es 5");

	pila_destruir(pila);
}

void desapilar_elemento_de_pila(void)
{
	pila_t *pila = pila_crear();

	int primer_elem = 25;
	int seg_elem = 18;
	int tercer_elem = 5;

	pa2m_afirmar(pila_apilar(pila, &primer_elem), "Se apilo 25");
	pa2m_afirmar(pila_apilar(pila, &seg_elem), "Se apilo 18");
	pa2m_afirmar(pila_apilar(pila, &tercer_elem), "Se apilo 5");

	int *tope = pila_ver_primero(pila);

	pa2m_afirmar(tope && *tope == 5, "El tope es 5");

	int *desapilado = (int *)pila_desapilar(pila);
	pa2m_afirmar(desapilado && *desapilado == 5, "Se desapilo el 5");

	int *nuevo_tope = pila_ver_primero(pila);

	pa2m_afirmar(nuevo_tope && *nuevo_tope == 18,
		     "El tope actualizado es 18");
	pila_destruir(pila);
}

//Cola

void crear_cola_con_elemento_null(void)
{
	cola_t *cola = cola_crear();

	pa2m_afirmar(cola != NULL, "Se pudo crear la cola");

	pa2m_afirmar(cola_ver_primero(cola) == NULL, "La cola inicia vacia");

	cola_destruir(cola);
}

void encolar_null(void)
{
	cola_t *cola = cola_crear();

	pa2m_afirmar(cola_encolar(cola, NULL),
		     "Se puede encolar un elemento NULL");

	pa2m_afirmar(cola_cantidad(cola) == 1, "La pila tiene 1 elemento");

	cola_destruir(cola);
}

void encolar_elemento(void)
{
	cola_t *cola = cola_crear();

	int primer_elem = 25;
	int seg_elem = 18;
	int tercer_elem = 5;

	pa2m_afirmar(cola_encolar(cola, &primer_elem), "Se encolo 25");
	pa2m_afirmar(cola_encolar(cola, &seg_elem), "Se encolo 8");
	pa2m_afirmar(cola_encolar(cola, &tercer_elem), "Se encolo 5");

	int *frente = cola_ver_primero(cola);

	pa2m_afirmar(frente && *frente == 25,
		     "Se encolaron correctamente, el frente es 25");

	cola_destruir(cola);
}

void desencolar_elemento(void)
{
	cola_t *cola = cola_crear();

	int primer_elem = 25;
	int seg_elem = 18;
	int tercer_elem = 5;

	pa2m_afirmar(cola_encolar(cola, &primer_elem), "Se encolo 25");
	pa2m_afirmar(cola_encolar(cola, &seg_elem), "Se encolo 18");
	pa2m_afirmar(cola_encolar(cola, &tercer_elem), "Se encolo 5");

	int *frente = cola_ver_primero(cola);

	pa2m_afirmar(frente && *frente == 25,
		     "Se encolaron correctamente, el frente es 25");

	int *desencolado = (int *)cola_desencolar(cola);
	pa2m_afirmar(desencolado && *desencolado == 25, "Se desencolo el 25");

	int *nuevo_frente = cola_ver_primero(cola);

	pa2m_afirmar(nuevo_frente && *nuevo_frente == 18,
		     "El frente actualizado es 8");

	cola_destruir(cola);
}

int main(void)
{
	pa2m_nuevo_grupo("Pruebas de creacion y TDAs vacios");
	lista_recien_creada_vacia_devuelve_true();
	lista_recien_creada_con_un_elemento_devuelve_false();
	crear_pila_con_elemento_null();
	crear_cola_con_elemento_null();

	pa2m_nuevo_grupo("Pruebas de agregar elementos en lista");
	agregar_elemento_nulo();
	agregar_elem_funciona_correctamente_y_aumenta_cantidad();
	insertar_en_posicion_cero_correctamente();
	insertar_en_ultima_posicion_correctamente();
	insertar_en_posicion_intermedia_correctamente();

	pa2m_nuevo_grupo("Pruebas de agregar elementos en cola-pila");
	apilar_null();
	encolar_null();
	apilar_elemento();
	encolar_elemento();

	pa2m_nuevo_grupo("Pruebas iterador interno");
	iterador_interno_devuelve_todos_los_elementos();
	iterador_interno_se_detiene_con_elem_mayor_a_diez();

	pa2m_nuevo_grupo("Pruebas eliminacion de elementos de lista");
	eliminar_elemento_de_lista_devuelve_lista_sin_ese_elemento();
	eliminar_ultimo_elemento_de_lista_y_actualizar_lista();

	pa2m_nuevo_grupo("Pruebas eliminacion de elementos de cola-pila");
	desapilar_elemento_de_pila();
	desencolar_elemento();

	pa2m_nuevo_grupo("Pruebas de destructores");
	lista_destruir_no_rompe();

	return pa2m_mostrar_reporte();
}
