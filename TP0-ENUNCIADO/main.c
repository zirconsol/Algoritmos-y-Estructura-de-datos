#include <stdio.h>
#include <stdlib.h>
#include "src/tp0.h"

int main(int argc, char *argv[])
{
	if (argc < 2) {
		fprintf(stderr, "Uso: %s <archivo.csv>\n", argv[0]);
		return 1;
	}

	FILE *archivo = fopen(argv[1], "r");
	if (!archivo) {
		perror("No se pudo abrir el archivo");
		return 1;
	}

	struct pokemon *vector_pokemones = NULL;
	size_t cantidad_pokemones = 0;
	size_t capacidad_vector = 0;

	char *linea_csv = leer_linea(archivo);

	while (linea_csv != NULL) {
		// Saltar líneas vacías
		if (linea_csv[0] == '\0') {
			free(linea_csv);
			linea_csv = leer_linea(archivo);
			continue;
		}

		struct pokemon *pokemon_parseado = parsear_pokemon(linea_csv);
		free(linea_csv);

		// Saltar líneas inválidas
		if (!pokemon_parseado) {
			linea_csv = leer_linea(archivo);
			continue;
		}

		// Redimensionar vector si está lleno
		if (cantidad_pokemones == capacidad_vector) {
			capacidad_vector =
				capacidad_vector ? capacidad_vector * 2 : 16;
			struct pokemon *nuevo_vector = realloc(
				vector_pokemones,
				capacidad_vector * sizeof(*vector_pokemones));
			if (!nuevo_vector) {
				perror("realloc");
				// liberar lo acumulado
				for (size_t i = 0; i < cantidad_pokemones;
				     i++) {
					free(vector_pokemones[i].nombre);
				}
				free(vector_pokemones);
				free(pokemon_parseado->nombre);
				free(pokemon_parseado);
				fclose(archivo);
				return 1;
			}
			vector_pokemones = nuevo_vector;
		}

		// Copiar el struct al vector
		vector_pokemones[cantidad_pokemones++] = *pokemon_parseado;
		free(pokemon_parseado); // liberar el contenedor (no el nombre)

		linea_csv = leer_linea(archivo);
	}

	fclose(archivo);

	// Ordenar por nombre
	ordenar_pokemon(vector_pokemones, (int)cantidad_pokemones);

	// Mostrar todos los pokemones cargados
	for (size_t i = 0; i < cantidad_pokemones; i++) {
		printf("%d,%s,%d,%d,%d,%d\n", vector_pokemones[i].id,
		       vector_pokemones[i].nombre, vector_pokemones[i].tipo,
		       vector_pokemones[i].ataque, vector_pokemones[i].defensa,
		       vector_pokemones[i].velocidad);

		free(vector_pokemones[i].nombre);
	}
	free(vector_pokemones);
	return 0;

	// Mostrar todos los pokemones cargados
	for (size_t i = 0; i < cantidad_pokemones; i++) {
		printf("%d,%s,%d,%d,%d,%d\n", vector_pokemones[i].id,
		       vector_pokemones[i].nombre, vector_pokemones[i].tipo,
		       vector_pokemones[i].ataque, vector_pokemones[i].defensa,
		       vector_pokemones[i].velocidad);

		free(vector_pokemones[i].nombre);
	}

	free(vector_pokemones);
	return 0;
}
