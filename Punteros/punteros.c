#include <stdio.h>

int sumar(int a, int b)
{
	return a + b;
}

int restar(int a, int b)
{
	return a - b;
}

void ordenar(void **elementos, size_t cantidad,
	     int (*comparador)(void *, void *))
{
	for (size_t i = 0; i < cantidad - 1; i++) {
		for (size_t j = 0; j < cantidad - i - 1; j++) {
			if (comparador(elementos[j], elementos[j + 1]) > 0) {
				void *aux = elementos[j];
				elementos[j] = elementos[j + 1];
				elementos[j + 1] = aux;
			}
		}
	}
}

int main()
{
	int (*puntero_a_suma)(int, int) = &sumar;

	int (*operaciones[])(int, int) = { sumar, restar };
}