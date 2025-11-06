#include <stdio.h>
#include "solucion.h"

int main()
{
	printf("sqrt(25) ≈ %.5f\n", raiz_cuadrada(25)); // ~5.00000
	printf("sqrt(2) ≈ %.5f\n", raiz_cuadrada(2)); // ~1.41421
	printf("sqrt(0) ≈ %.5f\n", raiz_cuadrada(0)); // 0.00000
	printf("sqrt(-9) = %.1f\n", raiz_cuadrada(-9)); // -1.0 (ERROR)

	return 0;
}