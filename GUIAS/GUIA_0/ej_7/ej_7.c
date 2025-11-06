#include "solucion.h"

/*
 * Dado un vector de chars, su tope y la letra a buscar
 * se devuelve true si la letra se encuentra aunque sea una vez en la 
 * palabra, y false si la letra no se encuentra en la palabra.
*/
bool contiene_letra(char palabra[MAX_VECTOR], int tope, char letra)
{
	for (int i = 0; i < tope; i++) {
		if (palabra[i] == letra) {
			return true;
		}
	}
	return false;
}