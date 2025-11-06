#include <stdio.h>

enum tipo_pokemon {
	TIPO_ELEC,
	TIPO_FUEG,
	TIPO_PLAN,
	TIPO_AGUA,
	TIPO_NORM,
	TIPO_FANT,
	TIPO_PSI,
	TIPO_LUCH
};

char *tipo_a_texto(enum tipo_pokemon tipo)
{
	switch (tipo) {
	case TIPO_AGUA:
		return "AGUA";
	case TIPO_ELEC:
		return "ELEC";
	default:
		return "????";
	}
}

int main()
{
	enum tipo_pokemon mi_tipo;

	mi_tipo = TIPO_ELEC;

	printf("Valor de mi tipo %i (%s)\n", mi_tipo, tipo_a_texto(mi_tipo));

	return 0;
}