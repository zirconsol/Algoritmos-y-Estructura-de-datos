// src/tp0.c
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tp0.h"

char *leer_linea(FILE *archivo)
{
	if (!archivo)
		return NULL;

	size_t capacidad = 16;
	size_t longitud_usada = 0;
	char *linea = malloc(capacidad);
	if (linea == NULL)
		return NULL;

	int caracter_actual;

	while ((caracter_actual = fgetc(archivo)) != EOF) {
		if (caracter_actual == '\n')
			break;

		if (longitud_usada + 1 >= capacidad) {
			capacidad *= 2;
			char *linea_nueva = realloc(linea, capacidad);
			if (linea_nueva == NULL) {
				free(linea);
				return NULL;
			}
			linea = linea_nueva;
		}

		linea[longitud_usada++] = (char)caracter_actual;
	}

	if (longitud_usada == 0 && caracter_actual == EOF) {
		free(linea);
		return NULL;
	}

	linea[longitud_usada] = '\0';
	return linea;
}

/* ======================== PARSEO SIMPLE (sin strtok) ======================== */
/* Espera: id,nombre,tipo,ataque,defensa,velocidad  (6 campos exactos) */
/* Validación mínima: 6 campos, nombre no vacío, tipo válido, números enteros. */

static struct pokemon *crear_pokemon(void)
{
	struct pokemon *p = (struct pokemon *)calloc(1, sizeof(struct pokemon));
	return p;
}

static int mapear_tipo_basico(const char *s, enum tipo_pokemon *out)
{
	if (!s || !*s)
		return 0;
	/* Comparamos en mayúsculas sin convertir (simple): aceptar exactamente ELEC/FUEG/... */
	if (strcmp(s, "ELEC") == 0) {
		*out = TIPO_ELEC;
		return 1;
	} else if (strcmp(s, "FUEG") == 0) {
		*out = TIPO_FUEG;
		return 1;
	} else if (strcmp(s, "PLAN") == 0) {
		*out = TIPO_PLAN;
		return 1;
	} else if (strcmp(s, "AGUA") == 0) {
		*out = TIPO_AGUA;
		return 1;
	} else if (strcmp(s, "NORM") == 0) {
		*out = TIPO_NORM;
		return 1;
	} else if (strcmp(s, "FANT") == 0) {
		*out = TIPO_FANT;
		return 1;
	} else if (strcmp(s, "PSI") == 0) {
		*out = TIPO_PSI;
		return 1;
	} else if (strcmp(s, "LUCH") == 0) {
		*out = TIPO_LUCH;
		return 1;
	}
	return 0;
}

struct pokemon *parsear_pokemon(char *linea_csv)
{
	if (!linea_csv)
		return NULL;

	// Copiamos porque vamos a partir en '\0'
	char *copia = strdup(linea_csv);
	if (!copia)
		return NULL;

	const int CAMPOS = 6;
	char *campo[CAMPOS] = { 0 };

	int n = 0;
	char *p = copia;
	char *ini = copia;

	while (*p && n < CAMPOS - 1) {
		if (*p == ',') {
			*p = '\0';
			campo[n++] = ini;
			ini = p + 1;
		}
		p++;
	}
	campo[n++] = ini;

	if (n != CAMPOS) {
		free(copia);
		return NULL;
	}

	// Crear struct
	struct pokemon *pk = crear_pokemon();
	if (!pk) {
		free(copia);
		return NULL;
	}

	// id
	char *e = NULL;
	pk->id = (int)strtol(campo[0], &e, 10);
	if (e == campo[0] || *e != '\0') {
		free(pk);
		free(copia);
		return NULL;
	}

	// nombre
	if (!campo[1] || !*campo[1]) {
		free(pk);
		free(copia);
		return NULL;
	}
	pk->nombre = strdup(campo[1]);
	if (!pk->nombre) {
		free(pk);
		free(copia);
		return NULL;
	}

	// tipo
	if (!mapear_tipo_basico(campo[2], &pk->tipo)) {
		free(pk->nombre);
		free(pk);
		free(copia);
		return NULL;
	}

	// ataque/defensa/velocidad
	pk->ataque = (int)strtol(campo[3], &e, 10);
	if (e == campo[3] || *e != '\0') {
		free(pk->nombre);
		free(pk);
		free(copia);
		return NULL;
	}

	pk->defensa = (int)strtol(campo[4], &e, 10);
	if (e == campo[4] || *e != '\0') {
		free(pk->nombre);
		free(pk);
		free(copia);
		return NULL;
	}

	pk->velocidad = (int)strtol(campo[5], &e, 10);
	if (e == campo[5] || *e != '\0') {
		free(pk->nombre);
		free(pk);
		free(copia);
		return NULL;
	}

	free(copia);
	return pk;
}

/* ======================== ORDEN (opcional) ======================== */
/* Si no lo usan, igual compila. Implementación cortita por inserción. */
void ordenar_pokemon(struct pokemon *vector_pokemones, int cantidad_pokemones)
{
	if (!vector_pokemones || cantidad_pokemones <= 1)
		return;

	for (int pasada = 0; pasada < cantidad_pokemones - 1; pasada++) {
		for (int indice = 0; indice < cantidad_pokemones - pasada - 1;
		     indice++) {
			const char *nombre_actual =
				vector_pokemones[indice].nombre ?
					vector_pokemones[indice].nombre :
					"";
			const char *nombre_siguiente =
				vector_pokemones[indice + 1].nombre ?
					vector_pokemones[indice + 1].nombre :
					"";

			if (strcmp(nombre_actual, nombre_siguiente) > 0) {
				// Intercambio de structs completos
				struct pokemon temporal =
					vector_pokemones[indice];
				vector_pokemones[indice] =
					vector_pokemones[indice + 1];
				vector_pokemones[indice + 1] = temporal;
			}
		}
	}
}
