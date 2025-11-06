#include "src/tp0.h"
#include <stdio.h>

void dado_pokemon_invalido_parsear_pokemon_devuelve_null()
{
	char *invalido = "2;charmande;FUEG;52;43;65";

	struct pokemon *parseado = parsear_pokemon(invalido);

	if (parseado == NULL) {
		printf("Todo ok \n");
	} else {
		printf("Todo mal \n");
	}
}

int main(int argc, char *argv[])
{
	dado_pokemon_invalido_parsear_pokemon_devuelve_null();

	return 0;
}