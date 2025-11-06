#include <string.h>
#include "solucion.h"

char ejercicio(char string1[MAX_STRING], char string2[MAX_STRING],
	       char string3[MAX_STRING])
{
	if (strlen(string1) + strlen(string2) >= MAX_STRING) {
        strcat(string1, string2);
		return (strcmp(string1, string3) <= 0) ? 'C' : 'S';
	}
}