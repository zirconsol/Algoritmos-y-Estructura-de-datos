/**
 * Return an array of arrays of size *returnSize.
 * The sizes of the arrays are returned as *returnColumnSizes array.
 * Note: Both returned array and *columnSizes array must be malloced, assume caller calls free().
 */

#include <stdbool.h>
#include <stdlib.h>

struct reservador_memoria {
	int **subsets;
	int *cap_subset;
	int *buffer_backtrack;
	int cap;
	int cant;
};

bool reservar_memoria_inicial(struct reservador_memoria *estructura, int n)
{
	if (!estructura) {
		return false;
	}
	size_t cap = (size_t)1 << n;
	estructura->cap = cap;
	estructura->subsets = (int **)calloc(estructura->cap, sizeof(int *));
	estructura->cap_subset = (int *)calloc(estructura->cap, sizeof(int));
	estructura->buffer_backtrack = (int *)malloc(n * sizeof(int));
	estructura->cant = 0;

	if (!estructura->subsets || !estructura->cap_subset ||
	    !estructura->buffer_backtrack) {
		free(estructura->subsets);
		free(estructura->cap_subset);
		free(estructura->buffer_backtrack);
		return false;
	}
	return true;
}

void liberar_memoria(struct reservador_memoria *estructura)
{
	if (!estructura)
		return;

	for (size_t i = 0; i < estructura->cant; i++) {
		free(estructura->subsets[i]);
	}

	free(estructura->subsets);
	free(estructura->cap_subset);
	free(estructura->buffer_backtrack);

	estructura->subsets = NULL;
	estructura->cap_subset = NULL;
	estructura->buffer_backtrack = NULL;
	estructura->cap = 0;
	estructura->cant = 0;
}

bool agregar_subset(struct reservador_memoria *estructura, int k)
{
	int *copia = (int *)malloc((size_t)k * sizeof(int));

	if (!copia && k > 0) {
		return false;
	}

	for (int i = 0; i < k; i++) {
		copia[i] = estructura->buffer_backtrack[i];
	}

	estructura->subsets[estructura->cant] = copia;
	estructura->cap_subset[estructura->cant] = k;
	estructura->cant += 1;
	return true;
}

bool backtrack(int *nums, int n, int start,
	       struct reservador_memoria *estructura, int k)
{
	bool seguir = true;

	if (!agregar_subset(estructura, k)) {
		seguir = false;
	}

	for (int i = start; seguir && i < n; i++) {
		estructura->buffer_backtrack[k] = nums[i];

		if (!backtrack(nums, n, i + 1, estructura, k + 1)) {
			seguir = false;
		}
	}
	return seguir;
}

int **subsets(int *nums, int numsSize, int *returnSize, int **returnColumnSizes)
{
	struct reservador_memoria mem;
	int **ret = NULL;

	if (!returnSize || !returnColumnSizes) {
		return NULL;
	}

	*returnSize = 0;
	*returnColumnSizes = NULL;

	if (!reservar_memoria_inicial(&mem, numsSize)) {
		ret = NULL;
	} else {
		if (!backtrack(nums, numsSize, 0, &mem, 0)) {
			liberar_memoria(&mem);
			ret = NULL;
		} else {
			*returnSize = (int)mem.cant;
			*returnColumnSizes = mem.cap_subset;
			ret = mem.subsets;
			free(mem.buffer_backtrack);
			mem.buffer_backtrack = NULL;
		}
	}

	return ret;
}
