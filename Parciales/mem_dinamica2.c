#include <stdlib.h>
#include <stdio.h>
struct azul {
	char *letra;
	struct azul *siguiente;
};

struct verde {
	int numero;
	struct azul *siguiente;
};

struct violeta {
	int numero;
	char *letras[4];
};

int main(void)
{
	char *letra_c = malloc(sizeof(char));
	char *letra_o = malloc(sizeof(char));
	char *letra_s = malloc(sizeof(char));
	char *letra_a = malloc(sizeof(char));
	if (!letra_c || !letra_o || !letra_s || !letra_a) {
		free(letra_c);
		free(letra_o);
		free(letra_s);
		free(letra_a);
		return -1;
	}

	struct azul *azul1 = malloc(sizeof(*azul1));
	struct azul *azul2 = malloc(sizeof(*azul2));
	struct azul *azul3 = malloc(sizeof(*azul3));
	struct azul *azul4 = malloc(sizeof(*azul4));
	struct verde *verde = malloc(sizeof(*verde));

	if (!azul1 || !azul2 || !azul3 || !azul4 || !verde) {
		free(azul1);
		free(azul2);
		free(azul3);
		free(azul4);
		free(verde);
		free(letra_c);
		free(letra_o);
		free(letra_s);
		free(letra_a);

		return -1;
	}

	struct verde *tope = verde;

	struct violeta violeta;
	violeta.numero = 4;

	*letra_c = 'C';
	*letra_o = 'O';
	*letra_s = 'S';
	*letra_a = 'A';

	azul1->letra = letra_c;
	azul2->letra = letra_o;
	azul3->letra = letra_s;
	azul4->letra = letra_a;

	azul1->siguiente = azul2;
	azul2->siguiente = azul3;
	azul3->siguiente = azul4;
	azul4->siguiente = NULL;

	verde->numero = 4;
	verde->siguiente = azul1;

	violeta.letras[0] = azul1->letra;
	violeta.letras[1] = azul2->letra;
	violeta.letras[2] = azul3->letra;
	violeta.letras[3] = azul4->letra;

	printf("El número en tope es: %d\n", tope->numero);

	free(azul1);
	free(azul2);
	free(azul3);
	free(azul4);
	free(verde);
	free(letra_c);
	free(letra_o);
	free(letra_s);
	free(letra_a);

	return 0;
}
