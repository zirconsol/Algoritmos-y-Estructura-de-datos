#include <stdlib.h>

struct roja {
	struct azul *ptr_rojo_azul_uno;
	struct azul *ptr_rojo_azul_dos;
	struct azul *ptr_rojo_azul_tres;
	char *ptr_letra_a;
	char *ptr_letra_l;
	char *ptr_letra_g;
};

struct azul {
	int nums[3];
	struct roja *ptr_rojo;
	struct azul *ptr_azul;
};

int main(void)
{
	struct azul *ptr_primer_azul = malloc(sizeof(*ptr_primer_azul));
	struct azul *ptr_segundo_azul = malloc(sizeof(*ptr_segundo_azul));
	struct azul *ptr_tercer_azul = malloc(sizeof(*ptr_tercer_azul));

	if (!ptr_primer_azul || !ptr_segundo_azul || !ptr_tercer_azul) {
		free(ptr_primer_azul);
		free(ptr_segundo_azul);
		free(ptr_tercer_azul);
		return -1;
	}

	struct roja roja_stack;
	char letra_a = 'A';
	char letra_l = 'L';
	char letra_g = 'G';
	roja_stack.ptr_rojo_azul_uno = NULL;
	roja_stack.ptr_rojo_azul_dos = NULL;
	roja_stack.ptr_rojo_azul_tres = NULL;
	roja_stack.ptr_letra_a = &letra_a;
	roja_stack.ptr_letra_l = &letra_l;
	roja_stack.ptr_letra_g = &letra_g;

	struct azul *inicio = ptr_primer_azul;
	(void)inicio;

	for (int i = 0; i < 3; i++) {
		ptr_primer_azul->nums[i] = i + 1;
	}

	for (int i = 0; i < 3; i++) {
		ptr_segundo_azul->nums[i] = i + 4;
	}

	for (int i = 0; i < 3; i++) {
		ptr_tercer_azul->nums[i] = i + 7;
	}

	ptr_tercer_azul->ptr_azul = ptr_primer_azul;
	ptr_segundo_azul->ptr_azul = ptr_tercer_azul;
	ptr_primer_azul->ptr_azul = ptr_segundo_azul;

	ptr_primer_azul->ptr_rojo = &roja_stack;
	ptr_segundo_azul->ptr_rojo = &roja_stack;
	ptr_tercer_azul->ptr_rojo = &roja_stack;

	roja_stack.ptr_rojo_azul_uno = ptr_segundo_azul;
	roja_stack.ptr_rojo_azul_dos = ptr_primer_azul;
	roja_stack.ptr_rojo_azul_tres = ptr_tercer_azul;

	free(ptr_primer_azul);
	free(ptr_segundo_azul);
	free(ptr_tercer_azul);
	return 0;
}