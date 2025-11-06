#include "solucion.h"
/*
 * Se devuelve la cantidad de elementos válidos que el string posee
 * o -1 si no se encuentra el caracter '\0' en el vector
 */
int longitud_string(char string[MAX_STRING])
{
	for (int i = 0; i < MAX_STRING; i++) {
		if (string[i] == '\0') {
			return i;
		}
	}
	return ERROR;
}