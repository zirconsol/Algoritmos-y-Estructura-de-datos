#include <stdio.h>
#include <string.h>
#include "src/tp1.h"
#include "src/funciones.h"

int main(int argc, char *argv[])
{
	if (argc == 2) {
		tp1_t *tp = tp1_leer_archivo(argv[1]);
		tp1_destruir(tp);
		return tp ? 0 : 1;
	}

	if (argc < 3)
		return 1;

	const char *ruta = argv[1];
	const char *comando = argv[2];

	if (strcmp(comando, "buscar") == 0 && argc >= 5) {
		if (strcmp(argv[3], "nombre") == 0)
			return buscar_nombre(ruta, argv[4]);
		if (strcmp(argv[3], "id") == 0)
			return buscar_id(ruta, atoi(argv[4]));
		return 1;
	}

	if (strcmp(comando, "union") == 0 && argc >= 5) {
		return union_arch(ruta, argv[3], argv[4]);
	}

	if (strcmp(comando, "interseccion") == 0 && argc >= 5) {
		return intersec_arch(ruta, argv[3], argv[4]);
	}
	if (strcmp(comando, "diferencia") == 0 && argc >= 5) {
		return dif_arch(ruta, argv[3], argv[4]);
	}
	if (strcmp(comando, "mostrar") == 0 && argc >= 4) {
		if (strcmp(argv[3], "id") == 0) {
			return mostrar_por_id(ruta);
		}
		if (strcmp(argv[3], "nombre") == 0) {
			return mostrar_por_nombre(ruta);
		}
		return 1;
	}
	return 0;
}