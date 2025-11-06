#include <stdlib.h>

struct azul {
	char letras[4];
	char **ptr_estructura[3];
};

int main(void)
{
	char **ptr_negro = malloc(sizeof(**ptr_negro));
	char **ptr_negro2 = malloc(2 * sizeof(**ptr_negro2));
	char **ptr_negro3 = malloc(3 * sizeof(**ptr_negro3));
	char **ptr_negro4 = malloc(4 * sizeof(**ptr_negro4));

	if (!ptr_negro || !ptr_negro2 || !ptr_negro3 || !ptr_negro4) {
		free(ptr_negro);
		free(ptr_negro2);
		free(ptr_negro3);
		free(ptr_negro4);
	}

	struct azul estructura;
	struct azul estructura = { .letras = { 'A', 'B', 'C', 'D' } };

	for (int i = 0; i < 3; i++) {
		ptr_negro4[i] = NULL;
	}
	ptr_negro4[3] = &estructura.letras[3];

	ptr_negro3[0] = ptr_negro4[0];
	ptr_negro3[1] = ptr_negro4[1];
	ptr_negro3[2] = &estructura.letras[2];

	ptr_negro2[0] = ptr_negro3[0];
	ptr_negro2[1] = &estructura.letras[1];

	ptr_negro[0] = &estructura.letras[0];

	char **ptr_desde_stack[4];
	ptr_desde_stack[0] = ptr_negro[0];
	ptr_desde_stack[1] = ptr_negro2[0];
	ptr_desde_stack[2] = ptr_negro3[0];
	ptr_desde_stack[3] = ptr_negro4[0];

	char **inicio = ptr_desde_stack;
	estructura.ptr_estructura[0] = inicio;
	estructura.ptr_estructura[1] = &ptr_negro2[1];
	estructura.ptr_estructura[2] = &ptr_negro4[3];

	free(ptr_desde_stack);
	free(ptr_negro);
	free(ptr_negro2);
	free(ptr_negro3);
	free(ptr_negro4);
	return 0;
}