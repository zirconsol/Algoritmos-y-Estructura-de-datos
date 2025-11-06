#ifndef FUNCIONES_H
#define FUNCIONES_H

#include "tp1.h"

void imprimir_pokemon(const struct pokemon *p);
/*
 *Dado un pokemon, imprime su informacion por pantalla.
*/

int buscar_nombre(const char *ruta, const char *nombre);
/*
 * Lee el archivo indicado y busca el pokemon segun el nombre indicado.
 * Si lo encuentra devuelve 0 y el pokemon 
 * En caso de error devuelve -2. Si el error es del archivo devuelve -1.
*/
int buscar_id(const char *ruta, int id);
/*
 * Lee el archivo indicado y busca el pokemon segun el id indicado.
 * Si lo encuentra devuelve 0 y el pokemon 
 * En caso de error devuelve -2. Si el error es del archivo devuelve -1.
*/
int union_arch(const char *ruta_a, const char *ruta_b, const char *ruta_out);
/*
 * Lee dos archivos indicados y guarda el resultado en un archivo nuevo sin duplicados.
 * Si alguno de los dos archivos no se puede abrir, devuelve -1.
 * Si la union y el archivo guardado salen sin errores, devuelve 0.
*/
int intersec_arch(const char *ruta_a, const char *ruta_b, const char *ruta_out);
/*
 * Lee dos archivos indicados y guarda los datos en comun en un archivo nuevo sin duplicados.
 * Si alguno de los dos archivos no se puede abrir, devuelve -1.
 * Si la interseccion y el archivo guardado salen sin errores, devuelve 0.
*/
int dif_arch(const char *ruta_a, const char *ruta_b, const char *ruta_out);
/*
 * Lee dos archivos indicados y guarda los datos del primero que no estan en el segundo en un archivo nuevo sin duplicados.
 * Si alguno de los dos archivos no se puede abrir, devuelve -1.
 * Si la union y el archivo guardado salen sin errores, devuelve 0.
*/
int mostrar_por_id(const char *ruta);
/*
 * Muestra por pantalla todos los pokemons de una pokedex ordenados por id.
 * Si lo consigue sin errores, devuelve 0.
 * En caso de error devuelve -1.
*/
int mostrar_por_nombre(const char *ruta);
/*
 * Muestra por pantalla todos los pokemons de una pokedex ordenados por nombre en orden alfabetico.
 * Si lo consigue sin errores, devuelve 0.
 * En caso de error devuelve -1.
*/

#endif
