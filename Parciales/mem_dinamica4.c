#include <stdlib.h>

struct roja {
	struct roja *primer_ptr;
	struct roja *segundo_ptr;
	struct roja **ptr_negro;
};

int main(void)
{
	struct roja *roja_uno = malloc(sizeof(*roja_uno));
	struct roja *roja_dos = malloc(sizeof(*roja_dos));
	struct roja *roja_tres = malloc(sizeof(*roja_tres));
	struct roja *roja_cuatro = malloc(sizeof(*roja_cuatro));

	struct roja **ptr_a_rojo = malloc(4 * sizeof(*ptr_a_rojo));

	if (!roja_uno || !roja_dos || !roja_tres || !roja_cuatro ||
	    !ptr_a_rojo) {
		free(roja_uno);
		free(roja_dos);
		free(roja_tres);
		free(roja_cuatro);
		free(ptr_a_rojo);
		return -1;
	}

	struct roja *inicio = roja_uno;
	(void)inicio;

	ptr_a_rojo[0] = roja_uno;
	ptr_a_rojo[1] = roja_dos;
	ptr_a_rojo[2] = roja_tres;
	ptr_a_rojo[3] = roja_cuatro;

	roja_uno->primer_ptr = roja_cuatro;
	roja_uno->segundo_ptr = roja_dos;
	roja_uno->ptr_negro = &ptr_a_rojo[2];

	roja_dos->primer_ptr = roja_uno;
	roja_dos->segundo_ptr = roja_tres;
	roja_dos->ptr_negro = &ptr_a_rojo[3];

	roja_tres->primer_ptr = roja_dos;
	roja_tres->segundo_ptr = roja_cuatro;
	roja_tres->ptr_negro = &ptr_a_rojo[0];

	roja_cuatro->primer_ptr = roja_tres;
	roja_cuatro->segundo_ptr = roja_uno;
	roja_cuatro->ptr_negro = &ptr_a_rojo[1];

	free(roja_uno);
	free(roja_dos);
	free(roja_tres);
	free(roja_cuatro);
	free(ptr_a_rojo);

	return 0;
}