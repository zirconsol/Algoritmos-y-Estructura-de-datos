#include <stdlib.h>

struct roja {
	char letra[3];
	char *letra_stack;
};

struct azul {
	struct roja *roja_uno;
	struct roja *roja_dos;
	struct azul *siguiente;
};

int main(void)
{
	char letra_a = 'A';
	char letra_b = 'B';
	char letra_c = 'C';
	char letra_d = 'D';
	int numero_cero = 0;

	struct roja *roja1 = malloc(sizeof(*roja1));
	struct roja *roja2 = malloc(sizeof(*roja2));
	struct roja *roja3 = malloc(sizeof(*roja3));
	struct roja *roja4 = malloc(sizeof(*roja4));
	struct azul *azul1 = malloc(sizeof(*azul1));
	struct azul *azul2 = malloc(sizeof(*azul2));
	struct azul *azul3 = malloc(sizeof(*azul3));

	if (!roja1 || !roja2 || !roja3 || !roja4 || !azul1 || !azul2 ||
	    !azul3) {
		free(roja1);
		free(roja2);
		free(roja3);
		free(azul1);
		free(azul2);
		free(azul3);
		return -1;
	}
	*roja1 = (struct roja){ .letra = { 'A', 'B', 'C' },
				.letra_stack = NULL };
	*roja2 = (struct roja){ .letra = { 'D', 'E', 'F' },
				.letra_stack = NULL };
	*roja3 = (struct roja){ .letra = { 'G', 'H', 'I' },
				.letra_stack = NULL };
	*roja4 = (struct roja){ .letra = { 'J', 'K', 'L' },
				.letra_stack = NULL };

	struct azul *inicio = azul1;

	azul1->roja_uno = roja1;
	azul1->roja_dos = roja2;
	azul1->siguiente = azul2;

	azul2->roja_uno = roja2;
	azul2->roja_dos = roja3;
	azul2->siguiente = azul3;

	azul3->roja_uno = roja3;
	azul3->roja_dos = roja4;
	azul3->siguiente = NULL;

	roja1->letra_stack = &letra_a;
	roja2->letra_stack = &letra_b;
	roja3->letra_stack = &letra_c;
	roja4->letra_stack = &letra_d;

	free(roja1);
	free(roja2);
	free(roja3);
	free(azul1);
	free(azul2);
	free(azul3);

	return 0;
}