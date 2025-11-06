#include <stdio.h>
#include <stdlib.h>

struct roja {
	int numero[3];
	struct roja *siguiente;
};

void mostrar_conexiones(struct roja *ptr_rojo1, struct roja **ptr_rojo2,
			struct roja **ptr_rojo3, struct roja **ptr_rojo4,
			struct roja ***ptr_negro)
{
	printf("\n=== Estructuras Rojas ===\n");
	printf("ptr_rojo1: %p -> siguiente: %p\n", (void *)ptr_rojo1,
	       (void *)ptr_rojo1->siguiente);

	for (int i = 0; i < 2; i++)
		printf("ptr_rojo2[%d]: %p -> siguiente: %p\n", i,
		       (void *)ptr_rojo2[i],
		       (void *)(ptr_rojo2[i] ? ptr_rojo2[i]->siguiente : NULL));

	for (int i = 0; i < 3; i++)
		printf("ptr_rojo3[%d]: %p -> siguiente: %p\n", i,
		       (void *)ptr_rojo3[i],
		       (void *)(ptr_rojo3[i] ? ptr_rojo3[i]->siguiente : NULL));

	for (int i = 0; i < 4; i++)
		printf("ptr_rojo4[%d]: %p -> siguiente: %p\n", i,
		       (void *)ptr_rojo4[i],
		       (void *)(ptr_rojo4[i] ? ptr_rojo4[i]->siguiente : NULL));

	printf("\n=== Punteros Negros ===\n");
	for (int i = 0; i < 4; i++)
		printf("ptr_negro[%d]: %p\n", i, (void *)ptr_negro[i]);

	printf("\nptr_negro[0] (→ ptr_rojo1): %p\n",
	       (void *)*(ptr_negro[0])); // contenido de &ptr_rojo1
}

int main(void)
{
	struct roja *roja1 = malloc(sizeof *roja1);
	struct roja *roja2 = malloc(sizeof *roja2);
	struct roja *roja3 = malloc(sizeof *roja3);
	struct roja *roja4 = malloc(sizeof *roja4);

	if (!roja1 || !roja2 || !roja3 || !roja4) {
		return -1;
	}

	*roja1 = (struct roja){ .numero = { 1, 2, 3 }, .siguiente = NULL };
	*roja2 = (struct roja){ .numero = { 1, 2, 3 }, .siguiente = NULL };
	*roja3 = (struct roja){ .numero = { 4, 5, 6 }, .siguiente = NULL };
	*roja4 = (struct roja){ .numero = { 7, 8, 9 }, .siguiente = NULL };

	struct roja ***ptr_negro = malloc(4 * sizeof(*ptr_negro));
	if (!ptr_negro) {
		return -1;
	}

	struct roja *ptr_rojo1 = malloc(sizeof(*ptr_rojo1));
	if (!ptr_rojo1) {
		free(ptr_rojo1);
		return -1;
	}

	ptr_rojo1->siguiente = NULL;

	struct roja **ptr_rojo2 = malloc(2 * sizeof(*ptr_rojo2));

	struct roja **ptr_rojo3 = malloc(3 * sizeof(*ptr_rojo3));

	struct roja **ptr_rojo4 = malloc(4 * sizeof(*ptr_rojo4));

	if (!ptr_rojo2 || !ptr_rojo3 || !ptr_rojo4) {
		return -1;
	}

	struct roja ***tope = ptr_negro;
	(void)tope;

	ptr_negro[0] = &ptr_rojo1;
	ptr_negro[1] = ptr_rojo2;
	ptr_negro[2] = ptr_rojo3;
	ptr_negro[3] = ptr_rojo4;

	ptr_rojo4[0] = roja4;
	ptr_rojo4[1] = roja3;
	ptr_rojo4[2] = roja2;
	ptr_rojo4[3] = roja1;

	for (int i = 0; i < 3; i++) {
		ptr_rojo3[i] = ptr_rojo4[i + 1];
	}

	for (int i = 0; i < 2; i++) {
		ptr_rojo2[i] = ptr_rojo3[i + 1];
	}

	ptr_rojo1->siguiente = ptr_rojo2[0];
	ptr_rojo2[0]->siguiente = ptr_rojo3[0];
	ptr_rojo3[0]->siguiente = ptr_rojo4[0];
	ptr_rojo4[0]->siguiente = NULL;

	mostrar_conexiones(ptr_rojo1, ptr_rojo2, ptr_rojo3, ptr_rojo4,
			   ptr_negro);

	free(ptr_negro);

	free(ptr_rojo1);
	free(ptr_rojo2);
	free(ptr_rojo3);
	free(ptr_rojo4);

	free(roja1);
	free(roja2);
	free(roja3);
	free(roja4);

	return 0;
}
