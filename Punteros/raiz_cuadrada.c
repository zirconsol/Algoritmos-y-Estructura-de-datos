#include "solucion.h"

double raiz_cuadrada(double valor)
{
	if (valor < 0) {
		return ERROR;
	}
	if (valor == 0) {
		return 0.0;
	}

	double estimacion = valor / 2.0;

	while ((estimacion * estimacion - valor >= 0 ?
			estimacion * estimacion - valor :
			-(estimacion * estimacion - valor)) > PRECISION) {
		estimacion = (estimacion + valor / estimacion) / 2.0;
	}
	return estimacion;
}
