#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp1.h"
#include <stdbool.h>

#define FACTOR_CRECIMIENTO 2
#define MEMORIA_INICIAL 8
#define CAMPOS_VALIDOS 6
#define BASE_LECT 10
#define POKE_ID 0
#define NOMBRE 1
#define POKETIPO 2
#define ATT 3
#define DEF 4
#define VEL 5

char *leer_linea(FILE *f);
size_t split_por_comas(char *linea, char **campos, size_t max_campos);
bool mapeo_tipo_pokemones(char *tipo_leido, enum tipo_pokemon *ptipo);
int parsear_pokemon(char *linea, struct pokemon *pokemon_parseado);
int buscar_indice_por_id(tp1_t *pokedex, int id);
void ordenar_indices_por_pokeid(const tp1_t *pokedex, size_t *indices);
int copiar_pokemon_parseado(struct pokemon *fuente, struct pokemon *destino);
int ampliar_capacidad(tp1_t *pokedex);

struct tp1 {
	struct pokemon *pokemones;
	unsigned int cantidad_pokemones;
	size_t capacidad_pokemones;
	size_t cantidad_total_registros;
};

char *leer_linea(FILE *f)
{
	if (!f) {
		return NULL;
	}

	char *buffer = NULL;
	char *nuevo_buffer = NULL;
	size_t tamaño = 0;
	size_t usado = 0;

	bool fin = false;
	bool error = false;
	bool linea_vacia = false;

	while (!fin && !error) {
		int caracter = fgetc(f);

		bool añadir_caracter = false;

		if (caracter == EOF) {
			if (usado == 0) {
				linea_vacia = true;
			}
			fin = true;
		} else if (caracter == '\n') {
			fin = true;
		} else if (caracter == '\r') {
		} else {
			añadir_caracter = true;
		}
		if (añadir_caracter) {
			if (usado + 1 > tamaño) {
				size_t nuevo_tamaño =
					(tamaño == 0) ?
						MEMORIA_INICIAL :
						tamaño * FACTOR_CRECIMIENTO;
				nuevo_buffer = realloc(buffer, nuevo_tamaño);
				if (!nuevo_buffer) {
					error = true;
				} else {
					buffer = nuevo_buffer;
					tamaño = nuevo_tamaño;
				}
			}
			if (!error) {
				buffer[usado] = (char)caracter;
				usado++;
			}
		}
	}

	if (error || linea_vacia) {
		free(buffer);
		return NULL;
	}
	if (usado + 1 > tamaño) {
		size_t nuevo_tamaño = (tamaño == 0) ?
					      MEMORIA_INICIAL :
					      tamaño * FACTOR_CRECIMIENTO;
		nuevo_buffer = realloc(buffer, nuevo_tamaño);
		if (!nuevo_buffer) {
			free(buffer);
			return NULL;
		}
		buffer = nuevo_buffer;
		tamaño = nuevo_tamaño;
	}
	buffer[usado] = '\0';
	return buffer;
}

size_t split_por_comas(char *linea, char **campos, size_t max_campos)
{
	if (linea == NULL || campos == NULL || max_campos == 0) {
		return 0;
	}

	size_t cant_campos = 0;
	char *linea_actual = linea;
	campos[cant_campos] = linea_actual;
	cant_campos++;

	for (size_t i = 0; linea[i] != '\0'; i++) {
		if (linea[i] == ',') {
			linea[i] = '\0';
			if (cant_campos < max_campos) {
				campos[cant_campos] = &linea[i + 1];
				cant_campos++;
			}
		}
	}
	return cant_campos;
}

bool mapeo_tipo_pokemones(char *tipo_leido, enum tipo_pokemon *ptipo)
{
	if (!tipo_leido || !ptipo) {
		return false;
	}

	struct tipos_pokemon {
		const char *codigo;
		enum tipo_pokemon valor;
	};

	const struct tipos_pokemon poketipos[] = {
		{ "ELEC", TIPO_ELEC }, { "FUEG", TIPO_FUEG },
		{ "PLAN", TIPO_PLAN }, { "AGUA", TIPO_AGUA },
		{ "NORM", TIPO_NORM }, { "FANT", TIPO_FANT },
		{ "PSI", TIPO_PSI },   { "LUCH", TIPO_LUCH }
	};
	size_t cantidad = sizeof(poketipos) / sizeof(poketipos[0]);
	for (size_t i = 0; i < cantidad; i++) {
		if (strcmp(tipo_leido, poketipos[i].codigo) == 0) {
			*ptipo = poketipos[i].valor;
			return true;
		}
	}
	return false;
}

int parsear_pokemon(char *linea, struct pokemon *pokemon_parseado)
{
	if (!linea) {
		return 0;
	}

	char *campos[CAMPOS_VALIDOS] = { 0 };

	if (split_por_comas(linea, campos, CAMPOS_VALIDOS) != CAMPOS_VALIDOS) {
		return 0;
	}

	char *final_string = NULL;
	long id_parseado;
	id_parseado = strtol(campos[POKE_ID], &final_string, BASE_LECT);
	if (*final_string != '\0') {
		return 0;
	}
	pokemon_parseado->id = (int)id_parseado;

	size_t longitud_nombre = strlen(campos[NOMBRE]);
	pokemon_parseado->nombre = (char *)malloc(longitud_nombre + 1);
	if (pokemon_parseado->nombre == NULL) {
		return 0;
	}
	memcpy(pokemon_parseado->nombre, campos[NOMBRE], longitud_nombre + 1);

	enum tipo_pokemon poketipo_parseado;
	if (!mapeo_tipo_pokemones(campos[POKETIPO], &poketipo_parseado)) {
		free(pokemon_parseado->nombre);
		pokemon_parseado->nombre = NULL;
		return 0;
	}
	pokemon_parseado->tipo = poketipo_parseado;

	final_string = NULL;
	long att_parseado;
	att_parseado = strtol(campos[ATT], &final_string, BASE_LECT);
	if (*final_string != '\0') {
		free(pokemon_parseado->nombre);
		pokemon_parseado->nombre = NULL;
		return 0;
	}
	pokemon_parseado->ataque = (int)att_parseado;

	final_string = NULL;
	long def_parseado;
	def_parseado = strtol(campos[DEF], &final_string, BASE_LECT);
	if (*final_string != '\0') {
		free(pokemon_parseado->nombre);
		pokemon_parseado->nombre = NULL;
		return 0;
	}
	pokemon_parseado->defensa = (int)def_parseado;

	final_string = NULL;
	long vel_parseado;
	vel_parseado = strtol(campos[VEL], &final_string, BASE_LECT);
	if (*final_string != '\0') {
		free(pokemon_parseado->nombre);
		pokemon_parseado->nombre = NULL;
		return 0;
	}
	pokemon_parseado->velocidad = (int)vel_parseado;

	return 1;
}

int buscar_indice_por_id(tp1_t *pokedex, int id)
{
	if (!pokedex) {
		return -1;
	}

	for (size_t i = 0; i < pokedex->cantidad_pokemones; i++) {
		if (pokedex->pokemones[i].id == id) {
			return (int)i;
		}
	}
	return -1;
}

int copiar_pokemon_parseado(struct pokemon *fuente, struct pokemon *destino)
{
	if (!destino || !fuente) {
		return 0;
	}
	if (!fuente->nombre) {
		return 0;
	}
	if (destino->nombre) {
		free(destino->nombre);
		destino->nombre = NULL;
	}

	*destino = *fuente;
	destino->nombre = NULL;
	size_t n = strlen(fuente->nombre);
	destino->nombre = malloc(n + 1);
	if (!destino->nombre) {
		return 0;
	}
	memcpy(destino->nombre, fuente->nombre, n + 1);
	return 1;
}

void ordenar_tp_por_id(tp1_t *tp)
{
	if (!tp) {
		return;
	}

	if (tp->cantidad_pokemones < 2) {
		return;
	}

	size_t cant = tp->cantidad_pokemones;

	size_t *indice = malloc(cant * sizeof(size_t));

	if (!indice) {
		return;
	}

	ordenar_indices_por_pokeid(tp, indice);

	struct pokemon *buffer_temp = malloc(cant * sizeof(struct pokemon));

	if (!buffer_temp) {
		free(indice);
		return;
	}
	for (size_t i = 0; i < cant; i++) {
		buffer_temp[i] = tp->pokemones[indice[i]];
	}
	memcpy(tp->pokemones, buffer_temp, cant * sizeof(struct pokemon));

	free(buffer_temp);
	free(indice);
}

struct tp1 *tp1_leer_archivo(const char *ruta)
{
	if (!ruta) {
		return NULL;
	}

	FILE *archivo = fopen(ruta, "r");
	tp1_t *pokedex = NULL;

	if (!archivo) {
		return NULL;
	}

	pokedex = malloc(sizeof(tp1_t));
	if (pokedex) {
		pokedex->pokemones = NULL;
		pokedex->cantidad_pokemones = 0;
		pokedex->capacidad_pokemones = 0;
		pokedex->cantidad_total_registros = 0;
	}

	char *linea = NULL;

	while ((linea = leer_linea(archivo)) != NULL) {
		struct pokemon pokemon_leido = { 0 };

		if (parsear_pokemon(linea, &pokemon_leido)) {
			pokedex->cantidad_total_registros++;

			int indice_pokemon =
				buscar_indice_por_id(pokedex, pokemon_leido.id);
			if (indice_pokemon == -1) {
				if (ampliar_capacidad(pokedex) &&
				    copiar_pokemon_parseado(
					    &pokemon_leido,
					    &pokedex->pokemones
						     [pokedex->cantidad_pokemones])) {
					pokedex->cantidad_pokemones++;
				}
			}
		}
		free(pokemon_leido.nombre);
		free(linea);
	}
	fclose(archivo);

	ordenar_tp_por_id(pokedex);
	return pokedex;
}

size_t tp1_cantidad(tp1_t *tp)
{
	return tp ? tp->cantidad_pokemones : 0;
}

size_t tp1_total_registros(tp1_t *tp)
{
	return tp ? tp->cantidad_total_registros : 0;
}

void tp1_destruir(struct tp1 *pokedex)
{
	if (!pokedex) {
		return;
	}

	for (size_t i = 0; i < pokedex->cantidad_pokemones; i++) {
		free(pokedex->pokemones[i].nombre);
	}

	free(pokedex->pokemones);
	free(pokedex);
}

int ampliar_capacidad(tp1_t *pokedex)
{
	if (pokedex->capacidad_pokemones > pokedex->cantidad_pokemones) {
		return 1;
	}

	size_t capacidad_actual = pokedex->capacidad_pokemones;
	size_t nueva_capacidad = (capacidad_actual == 0) ?
					 MEMORIA_INICIAL :
					 capacidad_actual * FACTOR_CRECIMIENTO;

	struct pokemon *nuevo_pokemon = realloc(
		pokedex->pokemones, nueva_capacidad * sizeof(struct pokemon));
	if (!nuevo_pokemon) {
		return 0;
	}

	pokedex->pokemones = nuevo_pokemon;

	for (size_t i = capacidad_actual; i < nueva_capacidad; i++) {
		pokedex->pokemones[i] = (struct pokemon){ 0 };
	}
	pokedex->capacidad_pokemones = nueva_capacidad;
	return 1;
}

char *convertir_tipo_enum_a_string(enum tipo_pokemon poketipo)
{
	switch (poketipo) {
	case TIPO_ELEC:
		return "ELEC";
	case TIPO_FUEG:
		return "FUEG";
	case TIPO_PLAN:
		return "PLAN";
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_NORM:
		return "NORM";
	case TIPO_FANT:
		return "FANT";
	case TIPO_PSI:
		return "PSI";
	case TIPO_LUCH:
		return "LUCH";
	default:
		return NULL;
	}
}

struct pokemon *tp1_buscar_id(tp1_t *pokedex, int id)
{
	if (!pokedex) {
		return NULL;
	}
	for (size_t i = 0; i < pokedex->cantidad_pokemones; i++) {
		if (pokedex->pokemones[i].id == id) {
			return &pokedex->pokemones[i];
		}
	}
	return NULL;
}

struct pokemon *tp1_buscar_nombre(tp1_t *pokedex, const char *pokenombre)
{
	if (!pokedex || !pokenombre) {
		return NULL;
	}
	for (size_t i = 0; i < pokedex->cantidad_pokemones; i++) {
		if (strcmp(pokedex->pokemones[i].nombre, pokenombre) == 0) {
			return &pokedex->pokemones[i];
		}
	}
	return NULL;
}

void ordenar_indices_por_pokeid(const tp1_t *pokedex, size_t *indices)
{
	size_t n = pokedex->cantidad_pokemones;

	for (size_t i = 0; i < n; i++) {
		indices[i] = i;
	}

	for (size_t i = 0; i + 1 < n; i++) {
		size_t posicion_minima = i;
		for (size_t j = i + 1; j < n; j++) {
			if (pokedex->pokemones[indices[j]].id <
			    pokedex->pokemones[indices[posicion_minima]].id) {
				posicion_minima = j;
			}
		}
		if (posicion_minima != i) {
			size_t tmp = indices[i];
			indices[i] = indices[posicion_minima];
			indices[posicion_minima] = tmp;
		}
	}
}

size_t tp1_con_cada_pokemon(tp1_t *pokedex, bool (*f)(struct pokemon *, void *),
			    void *extra)
{
	if (!pokedex) {
		return 0;
	}

	if (!f) {
		return 0;
	}

	size_t cant = pokedex->cantidad_pokemones;
	size_t i = 0;
	size_t poke_procesados = 0;
	bool continuar = true;

	while (i < cant && continuar) {
		continuar = f(&pokedex->pokemones[i], extra);
		poke_procesados++;
		if (!continuar) {
			break;
		}
		i++;
	}
	return poke_procesados;
}

tp1_t *tp1_guardar_archivo(tp1_t *estructura_tp1, const char *ruta_archivo)
{
	if (!estructura_tp1 || !ruta_archivo || ruta_archivo[0] == '\0') {
		return NULL;
	}

	FILE *archivo_salida = fopen(ruta_archivo, "w");

	if (!archivo_salida) {
		return NULL;
	}

	for (size_t indice_pokemon = 0;
	     indice_pokemon < estructura_tp1->cantidad_pokemones;
	     indice_pokemon++) {
		struct pokemon *pokemon_actual =
			&estructura_tp1->pokemones[indice_pokemon];

		const char *tipo_str =
			convertir_tipo_enum_a_string(pokemon_actual->tipo);
		if (!tipo_str) {
			continue;
		}

		int resultado = fprintf(
			archivo_salida, "%d,%s,%s,%d,%d,%d\n",
			pokemon_actual->id,
			pokemon_actual->nombre ? pokemon_actual->nombre : "",
			tipo_str, pokemon_actual->ataque,
			pokemon_actual->defensa, pokemon_actual->velocidad);

		if (resultado < 0) {
			fclose(archivo_salida);
			return NULL;
		}
	}
	fclose(archivo_salida);
	return estructura_tp1;
}

size_t *ordenar_indices_por_id(tp1_t *tp)
{
	if (!tp || tp->cantidad_pokemones == 0) {
		return NULL;
	}

	size_t cant = tp->cantidad_pokemones;
	size_t *indices = malloc(cant * sizeof(size_t));
	if (!indices) {
		return NULL;
	}

	ordenar_indices_por_pokeid(tp, indices);
	return indices;
}

tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp) {
		return NULL;
	}

	tp1_t *union_tps = calloc(1, sizeof(tp1_t));

	if (!union_tps) {
		return NULL;
	}

	size_t indice_un_tp = 0;
	size_t indice_otro_tp = 0;
	size_t cant_un_tp = un_tp ? un_tp->cantidad_pokemones : 0;
	size_t cant_otro_tp = otro_tp ? otro_tp->cantidad_pokemones : 0;

	while (indice_un_tp < cant_un_tp || indice_otro_tp < cant_otro_tp) {
		struct pokemon *pokemon_elegido = NULL;

		bool elegir_un_tp = indice_un_tp < cant_un_tp;
		bool elegir_otro_tp = indice_otro_tp < cant_otro_tp;

		int pokeid_un_tp =
			elegir_un_tp ? un_tp->pokemones[indice_un_tp].id : 0;
		int pokeid_otro_tp =
			elegir_otro_tp ? otro_tp->pokemones[indice_otro_tp].id :
					 0;

		if (elegir_un_tp &&
		    (!elegir_otro_tp || pokeid_un_tp < pokeid_otro_tp)) {
			pokemon_elegido = &un_tp->pokemones[indice_un_tp];
			indice_un_tp++;

		} else if (elegir_otro_tp &&
			   (!elegir_un_tp || pokeid_otro_tp < pokeid_un_tp)) {
			pokemon_elegido = &otro_tp->pokemones[indice_otro_tp];
			indice_otro_tp++;
		} else {
			if (elegir_un_tp) {
				pokemon_elegido =
					&un_tp->pokemones[indice_un_tp];
				indice_un_tp++;
			}
			if (elegir_otro_tp) {
				indice_otro_tp++;
			}
		}
		if (!ampliar_capacidad(union_tps)) {
			if (union_tps) {
				for (size_t i = 0;
				     i < union_tps->cantidad_pokemones; i++) {
					free(union_tps->pokemones[i].nombre);
				}
				free(union_tps->pokemones);
				free(union_tps);
			}
			return NULL;
		}
		if (!copiar_pokemon_parseado(
			    (struct pokemon *)pokemon_elegido,
			    &union_tps->pokemones
				     [union_tps->cantidad_pokemones])) {
			if (union_tps) {
				for (size_t i = 0;
				     i < union_tps->cantidad_pokemones; i++) {
					free(union_tps->pokemones[i].nombre);
				}
				free(union_tps->pokemones);
				free(union_tps);
			}
			return NULL;
		}
		union_tps->cantidad_pokemones++;
	}
	union_tps->cantidad_total_registros = union_tps->cantidad_pokemones;
	return union_tps;
}

tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp) {
		return NULL;
	}

	tp1_t *intersec_tps = calloc(1, sizeof(tp1_t));

	if (!intersec_tps) {
		return NULL;
	}

	size_t indice_un_tp = 0;
	size_t indice_otro_tp = 0;
	size_t cant_un_tp = un_tp ? un_tp->cantidad_pokemones : 0;
	size_t cant_otro_tp = otro_tp ? otro_tp->cantidad_pokemones : 0;

	while (indice_un_tp < cant_un_tp && indice_otro_tp < cant_otro_tp) {
		int pokeid_un_tp = un_tp->pokemones[indice_un_tp].id;
		int pokeid_otro_tp = otro_tp->pokemones[indice_otro_tp].id;

		if (pokeid_un_tp == pokeid_otro_tp) {
			if (!ampliar_capacidad(intersec_tps)) {
				for (size_t i = 0;
				     i < intersec_tps->cantidad_pokemones;
				     i++) {
					free(intersec_tps->pokemones[i].nombre);
				}
				free(intersec_tps->pokemones);
				free(intersec_tps);
				return NULL;
			}

			if (!copiar_pokemon_parseado(
				    &un_tp->pokemones[indice_un_tp],
				    &intersec_tps->pokemones
					     [intersec_tps->cantidad_pokemones])) {
				for (size_t i = 0;
				     i < intersec_tps->cantidad_pokemones;
				     i++) {
					free(intersec_tps->pokemones[i].nombre);
				}
				free(intersec_tps->pokemones);
				free(intersec_tps);
				return NULL;
			}

			intersec_tps->cantidad_pokemones++;
			indice_un_tp++;
			indice_otro_tp++;
		} else if (pokeid_un_tp < pokeid_otro_tp) {
			indice_un_tp++;
		} else {
			indice_otro_tp++;
		}
	}
	intersec_tps->cantidad_total_registros =
		intersec_tps->cantidad_pokemones;
	return intersec_tps;
}

tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp)
{
	if (!un_tp || !otro_tp) {
		return NULL;
	}

	tp1_t *dif_tps = calloc(1, sizeof(tp1_t));
	if (!dif_tps) {
		return NULL;
	}

	size_t indice_un_tp = 0;
	size_t indice_otro_tp = 0;
	size_t cant_un_tp = un_tp->cantidad_pokemones;
	size_t cant_otro_tp = otro_tp ? otro_tp->cantidad_pokemones : 0;

	while (indice_un_tp < cant_un_tp) {
		int pokeid_un_tp = un_tp->pokemones[indice_un_tp].id;
		int pokeid_otro_tp =
			(indice_otro_tp < cant_otro_tp) ?
				otro_tp->pokemones[indice_otro_tp].id :
				0;

		if (indice_otro_tp >= cant_otro_tp) {
			if (!ampliar_capacidad(dif_tps)) {
				for (size_t i = 0;
				     i < dif_tps->cantidad_pokemones; i++)
					free(dif_tps->pokemones[i].nombre);
				free(dif_tps->pokemones);
				free(dif_tps);
				return NULL;
			}
			if (!copiar_pokemon_parseado(
				    &un_tp->pokemones[indice_un_tp],
				    &dif_tps->pokemones
					     [dif_tps->cantidad_pokemones])) {
				for (size_t i = 0;
				     i < dif_tps->cantidad_pokemones; i++)
					free(dif_tps->pokemones[i].nombre);
				free(dif_tps->pokemones);
				free(dif_tps);
				return NULL;
			}
			dif_tps->cantidad_pokemones++;
			indice_un_tp++;
		} else if (pokeid_un_tp < pokeid_otro_tp) {
			if (!ampliar_capacidad(dif_tps)) {
				for (size_t i = 0;
				     i < dif_tps->cantidad_pokemones; i++)
					free(dif_tps->pokemones[i].nombre);
				free(dif_tps->pokemones);
				free(dif_tps);
				return NULL;
			}
			if (!copiar_pokemon_parseado(
				    &un_tp->pokemones[indice_un_tp],
				    &dif_tps->pokemones
					     [dif_tps->cantidad_pokemones])) {
				for (size_t i = 0;
				     i < dif_tps->cantidad_pokemones; i++)
					free(dif_tps->pokemones[i].nombre);
				free(dif_tps->pokemones);
				free(dif_tps);
				return NULL;
			}
			dif_tps->cantidad_pokemones++;
			indice_un_tp++;
		} else if (pokeid_un_tp == pokeid_otro_tp) {
			indice_un_tp++;
			indice_otro_tp++;
		} else {
			indice_otro_tp++;
		}
	}
	dif_tps->cantidad_total_registros = dif_tps->cantidad_pokemones;
	return dif_tps;
}
