#include "pa2m.h"
#include "src/tp1.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include <limits.h>

#define ARCHIVO_PRUEBA_INEXISTENTE "ejemplos/asdkasjhfskladjhfksdfhksdf.csv"
#define ARCHIVO_VACIO "ejemplos/vacio.txt"
#define ARCHIVO_NL "ejemplos/archivo_con_linea_final.txt"
#define ARCHIVO_SIN_NL "ejemplos/archivo_sin_linea_final.txt"
#define ARCHIVO_VARIAS_LINEAS "ejemplos/archivo_con_varias_lineas.txt"
#define ARCHIVO_NORMAL "ejemplos/normal.csv"
#define ARCHIVO_LARGO "ejemplos/largo.csv"
#define ARCHIVO_REPETIDOS "ejemplos/archivo_con_repetidos.csv"
#define ARCHIVO_MAL_SEPARADO "ejemplos/archivo_campos_incorrectos.csv"
#define ARCHIVO_POKETIPO_INCORRECTO "ejemplos/archivo_tipo_incorrecto.csv"

struct tp1;
struct pokemon *tp1_buscar_id(tp1_t *pokedex, int id);
struct pokemon *tp1_buscar_nombre(tp1_t *pokedex, const char *nombre);

int buscar_indice_por_id(const tp1_t *tp, int id);
int copiar_pokemon_parseado(const struct pokemon *src, struct pokemon *dst);
void tp1_destruir(struct tp1 *);
size_t tp1_cantidad(tp1_t *tp1);
size_t tp1_total_registros(tp1_t *tp1);
size_t tp1_con_cada_pokemon(tp1_t *pokedex, bool (*f)(struct pokemon *, void *),
			    void *extra);
tp1_t *tp1_leer_archivo(const char *ruta);
char *leer_linea(FILE *f);
size_t split_por_comas(char *linea, char **campos, size_t max_campos);
bool mapeo_tipo_pokemones(const char *tipo_leido, enum tipo_pokemon *ptipo);
tp1_t *tp1_guardar_archivo(tp1_t *tp1, const char *ruta);

void tp1_leer_archivo_devuelve_null_cuando_el_archivo_no_existe(void)
{
	tp1_t *tp1 = tp1_leer_archivo(ARCHIVO_PRUEBA_INEXISTENTE);
	pa2m_afirmar(tp1 == NULL, "Abrir un archivo inexistente devuelve NULL");
	tp1_destruir(tp1);
}

void tp1_leer_archivo_devuelve_estructura(void)
{
	struct tp1 *tp = tp1_leer_archivo(ARCHIVO_VACIO);
	pa2m_afirmar(tp != NULL,
		     "Se devuelve puntero válido si el archivo existe");
	pa2m_afirmar(
		tp1_cantidad(tp) == 0,
		"La estructura comienza con cantidad 0 (es valido, el archivo esta vacio)");
	tp1_destruir(tp);
}

void leer_linea_archivo_vacio_da_NULL(void)
{
	FILE *f = fopen(ARCHIVO_VACIO, "r");
	pa2m_afirmar(f != NULL, "Se pudo abrir archivo vacio");
	char *linea = leer_linea(f);
	pa2m_afirmar(linea == NULL, "Como esta vacio leer_linea devuelve NULL");
	fclose(f);
}

void leer_linea_con_newline(void)
{
	FILE *f = fopen(ARCHIVO_NL, "r");
	pa2m_afirmar(f != NULL,
		     "Se pudo abrir archivo con salto de linea al final");
	char *linea = leer_linea(f);
	pa2m_afirmar(linea != NULL, "Devuelve una linea no nula");
	pa2m_afirmar(strcmp(linea, "Pikachu") == 0,
		     "La linea no incluye el \\n final");
	free(linea);
	fclose(f);
}

void leer_linea_sin_newline(void)
{
	FILE *f = fopen(ARCHIVO_SIN_NL, "r");
	pa2m_afirmar(f != NULL,
		     "Se pudo abrir archivo sin salto de linea al final");
	char *linea = leer_linea(f);
	pa2m_afirmar(linea != NULL, "Devuelve una linea no nula");
	pa2m_afirmar(strcmp(linea, "Charizard") == 0,
		     "La linea no incluye el \\n final");
	free(linea);
	char *otra = leer_linea(f);
	pa2m_afirmar(
		otra == NULL,
		"Segunda lectura da NULL (es valido, es archivo solo tiene una linea)");
	fclose(f);
}

void leer_varias_lineas(void)
{
	FILE *f = fopen(ARCHIVO_VARIAS_LINEAS, "r");
	char *linea1 = leer_linea(f);
	char *linea2 = leer_linea(f);
	char *linea3 = leer_linea(f);
	char *linea4 = leer_linea(f);

	pa2m_afirmar(linea1 && linea2 && linea3,
		     "Primeras tres lecturas son no nulas");
	pa2m_afirmar(strcmp(linea1, "Bulbasaur") == 0 &&
			     strcmp(linea2, "Squirtle") == 0 &&
			     strcmp(linea3, "Jigglypuff") == 0,
		     "El contenido de las lineas es es valido");
	pa2m_afirmar(
		linea4 == NULL,
		"La cuarta lectura da NULL (es valido, se llego al final del archivo)");
	free(linea1);
	free(linea2);
	free(linea3);
	fclose(f);
}

void spliteo_linea_por_comas(void)
{
	char linea[] = "25,Pikachu,ELEC,55,40,90";
	char *campos[6] = { 0 };
	size_t n = split_por_comas(linea, campos, 6);
	pa2m_afirmar(n == 6,
		     "Cantidad de campos separados correcta, hay 6 campos");
	pa2m_afirmar(strcmp(campos[0], "25") == 0, "Campo 0 es valido");
	pa2m_afirmar(strcmp(campos[1], "Pikachu") == 0, "Campo 1 es valido");
	pa2m_afirmar(strcmp(campos[2], "ELEC") == 0, "Campo 2 es valido");
	pa2m_afirmar(strcmp(campos[3], "55") == 0, "Campo 3 es valido");
	pa2m_afirmar(strcmp(campos[4], "40") == 0, "Campo 4 es valido");
	pa2m_afirmar(strcmp(campos[5], "90") == 0, "Campo 5 es valido");
}

void leer_archivo_verificando_cantidad_campos_correcta(void)
{
	FILE *f = fopen(ARCHIVO_NORMAL, "r");
	char *linea = leer_linea(f);
	char *campos[6] = { 0 };
	size_t n = split_por_comas(linea, campos, 6);

	pa2m_afirmar(n == 6,
		     "La línea tiene exactamente 6 campos separados por coma");
	free(linea);
	fclose(f);
}

void leer_archivo_verificando_separacion_incorrecta(void)
{
	FILE *f = fopen(ARCHIVO_MAL_SEPARADO, "r");
	char *linea = leer_linea(f);
	char *campos[6] = { 0 };
	size_t cant_campos = split_por_comas(linea, campos, 6);

	pa2m_afirmar(
		cant_campos != 6,
		"La línea esta mal separada, se usa ';' como separador, no produce 6 campos validos (Hay %zu campos correctamente separados)",
		campos);
	free(linea);
	fclose(f);
}

void leer_archivo_poketipo_valido(void)
{
	FILE *f = fopen(ARCHIVO_NORMAL, "r");
	char *linea = leer_linea(f);
	char *campos[6] = { 0 };
	split_por_comas(linea, campos, 6);
	pa2m_afirmar(f != NULL, "Linea leida del archivo");
	enum tipo_pokemon tipo = 0;
	pa2m_afirmar(
		mapeo_tipo_pokemones(campos[2], &tipo),
		"Poketipo recibido valido. El mapeo devuelve true (poketipo numero %d)",
		tipo);

	free(linea);
	fclose(f);
}

void leer_archivo_poketipo_invalido(void)
{
	FILE *f = fopen(ARCHIVO_POKETIPO_INCORRECTO, "r");
	char *linea = leer_linea(f);
	char *campos[6] = { 0 };
	split_por_comas(linea, campos, 6);

	enum tipo_pokemon tipo;
	pa2m_afirmar(!mapeo_tipo_pokemones(campos[2], &tipo),
		     "Tipo inválido, el mapeo resulta incorrecto");

	free(linea);
	fclose(f);
}

void leer_archivo_parseado_con_multiples_lineas(void)
{
	tp1_t *tp = tp1_leer_archivo(ARCHIVO_NORMAL);
	pa2m_afirmar(tp != NULL, "Se devolvio una estructura valida");
	pa2m_afirmar(tp1_cantidad(tp) >= 1, "Cantidad de pokemon validos: %zu",
		     tp1_cantidad(tp));
	tp1_destruir(tp);
}

void cargar_cinco_pokemons_cuenta_cinco(void)
{
	tp1_t *tp = tp1_leer_archivo(ARCHIVO_NORMAL);
	size_t cant = tp1_cantidad(tp);
	pa2m_afirmar(
		cant == 5,
		"%zu pokemons unicos leidos correctamente (5 eran los esperados)",
		cant);
	tp1_destruir(tp);
}

void mostrar_cantidad_total_pokemons(void)
{
	tp1_t *tp = tp1_leer_archivo(ARCHIVO_REPETIDOS);
	size_t unicos = tp1_cantidad(tp);
	size_t total = tp1_total_registros(tp);

	pa2m_afirmar(unicos == 5, "%zu pokemons unicos (5 eran los esperados)",
		     unicos);
	pa2m_afirmar(
		total == 6,
		" %zu pokemons leidos incluyendo repetidos(6 eran los esperados)",
		total);

	tp1_destruir(tp);
}

void guardar_vector_dinamico_en_archivo(void)
{
	const char *entrada = ARCHIVO_REPETIDOS;
	const char *salida = "salida_guardado.csv";

	tp1_t *lector = tp1_leer_archivo(entrada);
	size_t unicos_antes = tp1_cantidad(lector);
	size_t total_antes = tp1_total_registros(lector);

	pa2m_afirmar(
		total_antes >= unicos_antes,
		"%zu pokemons totales y %zu indices unicos antes de guardar",
		total_antes, unicos_antes);

	pa2m_afirmar(tp1_guardar_archivo(lector, salida) == lector,
		     "Al guardar se devuelve el mismo tp");

	tp1_t *segundo_lector = tp1_leer_archivo(salida);
	pa2m_afirmar(segundo_lector != NULL,
		     "Se pudo volver a leer el archivo luego de ser guardado");

	size_t unicos_despues = tp1_cantidad(segundo_lector);
	size_t total_despues = tp1_total_registros(segundo_lector);

	pa2m_afirmar(unicos_despues == unicos_antes,
		     "%zu pokemons unicos luego de guardar y releer",
		     unicos_despues);
	pa2m_afirmar(
		total_despues == unicos_antes,
		"El archivo guardado solo contiene ids unicos, con %zu despues de releer",
		total_despues);

	tp1_destruir(segundo_lector);
	tp1_destruir(lector);
}

void guardar_en_ruta_invalida_devuelve_NULL(void)
{
	const char *entrada = ARCHIVO_REPETIDOS;
	const char *ruta_invalida = "no_existe/no_existe.csv";

	tp1_t *lector = tp1_leer_archivo(entrada);

	pa2m_afirmar(tp1_guardar_archivo(lector, ruta_invalida) == NULL,
		     "Intento de guardar en una ruta inválida devuelve NULL");

	tp1_destruir(lector);
}

void buscar_id_existente_devuelve_puntero_valido(void)
{
	tp1_t *pokedex = tp1_leer_archivo(ARCHIVO_NORMAL);

	struct pokemon *poke_buscado = tp1_buscar_id(pokedex, 3);
	pa2m_afirmar(poke_buscado != NULL,
		     "Buscar id=3 devuelve puntero válido");
	if (poke_buscado) {
		pa2m_afirmar(strcmp(poke_buscado->nombre, "Bulbasaur") == 0,
			     "El nombre del id=3 es Bulbasaur");
		pa2m_afirmar(poke_buscado->tipo == TIPO_PLAN,
			     "El tipo del id=3 es PLAN");
	}

	tp1_destruir(pokedex);
}

void union_los_dos_null_da_null(void)
{
	pa2m_afirmar(tp1_union(NULL, NULL) == NULL,
		     "union entre dos NULL devuelve NULL");
}

static void union_vacio_con_normal(void)
{
	tp1_t *un_tp = tp1_leer_archivo(ARCHIVO_VACIO);
	tp1_t *otro_tp = tp1_leer_archivo(ARCHIVO_NORMAL);
	tp1_t *union_arch = tp1_union(un_tp, otro_tp);

	pa2m_afirmar(
		union_arch != NULL,
		"La union con un archivo vacio y uno normal devuelve una estructura valida");
	pa2m_afirmar(
		tp1_cantidad(union_arch) == tp1_cantidad(otro_tp),
		"La cantidad de pokemons en la union es igual a la del archivo normal");
	pa2m_afirmar(tp1_total_registros(union_arch) ==
			     tp1_cantidad(union_arch),
		     "Se conservaron todos los pokemons del archivo normal");

	tp1_destruir(union_arch);
	tp1_destruir(otro_tp);
	tp1_destruir(un_tp);
}

int main(void)
{
	pa2m_nuevo_grupo("Pruebas de apertura de archivos");
	tp1_leer_archivo_devuelve_null_cuando_el_archivo_no_existe();
	tp1_leer_archivo_devuelve_estructura();

	pa2m_nuevo_grupo("Pruebas lectura de lineas");
	leer_linea_archivo_vacio_da_NULL();
	leer_linea_con_newline();
	leer_linea_sin_newline();
	leer_varias_lineas();

	pa2m_nuevo_grupo("Pruebas split por comas");
	spliteo_linea_por_comas();
	leer_archivo_verificando_cantidad_campos_correcta();
	leer_archivo_verificando_separacion_incorrecta();

	pa2m_nuevo_grupo("Pruebas lectura y verificacion tipo");
	leer_archivo_poketipo_valido();
	leer_archivo_poketipo_invalido();

	pa2m_nuevo_grupo("Pruebas parseo");
	leer_archivo_parseado_con_multiples_lineas();

	pa2m_nuevo_grupo("Pruebas vector dinamico");
	cargar_cinco_pokemons_cuenta_cinco();
	mostrar_cantidad_total_pokemons();

	pa2m_nuevo_grupo("Prueba guardado de archivo");
	guardar_vector_dinamico_en_archivo();
	guardar_en_ruta_invalida_devuelve_NULL();

	pa2m_nuevo_grupo("Pruebas de busqueda en archivo");
	buscar_id_existente_devuelve_puntero_valido();

	pa2m_nuevo_grupo("Pruebas de union de archivos");
	union_los_dos_null_da_null();
	union_vacio_con_normal();

	return pa2m_mostrar_reporte();
}
