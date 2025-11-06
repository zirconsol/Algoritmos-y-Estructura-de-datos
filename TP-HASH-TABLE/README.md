<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA HASH

## Repositorio de (Nombre Apellido) - (Padrón) - (Mail)

- Para compilar:

```bash
línea de compilación
```

- Para ejecutar:

```bash
línea de ejecución
```

- Para ejecutar con valgrind:
```bash
línea con valgrind
```
---
#  Funcionamiento

Para el TP de **TDA Hash**, implementé una tabla hash abierta con *direccionamiento cerrado*, que permite insertar, buscar, verificar existencia, quitar, iterar y destruir pares clave-valor.

La estructura central mantiene un arreglo de contenedores, donde cada posición almacena una lista simplemente enlazada de nodos. Las colisiones se resuelven encadenando nodos en la misma posición.

La tabla crece dinámicamente en función del factor de carga, redimensionando cuando se supera un valor predefinido y por defecto, se utiliza la función de hash djb2.

 Se realizo una implementación `hash.c`, con su respectivo header `abb.h`, que contiene distintas implementaciones de las funciones para el hash como `insertar`,`quitar`,`buscar` y `destruir`.

## abb.c

Para poder implementar correctamente el abb, trabaje con una estructura para los nodos definida como:
```
typedef struct nodo {
    char *clave;
    void *valor;
    struct nodo *sig;
} nodo_t;
```
 Donde cada nodo de la lista guarda la pareja clave–valor y un puntero para enlazarse al siguiente.

 Ademas, tambien defini la estructura principal del hash como:

```
struct hash {
    size_t capacidad;
    size_t cantidad;
    nodo_t **contenedores;
    size_t (*funcion_hash)(const char *);
};
```

Donde el mismo almacena la cantidad de contenedores, la cantidad actual de elementos, un puntero doble al arreglo de contenedores y un puntero a la función de hash.

***
### <ins>Primitivas del Hash

#### hash_t *hash_crear(size_t capacidad_inicial)

Crea la estructura del hash, asegurando una capacidad mínima. Reserva memoria con calloc tanto para la estructura como para el arreglo de contenedores y configura funcion_hash = hash_djb2.

Como no recorre elementos, la complejidad temporal es **O(1)**.


<p align="center">
  <img src="img/diagrama1.png" alt="Hash crear" width="800"/>
  <br>
  <em>Diagrama de la función abb_crear.</em>
</p>
<br><br>


#### size_t hash_cantidad(hash_t *hash)

Esta funcion devuelve la cantidad de pares almacenados, o 0 si el hash es **NULL**, y como simplemente retorna un valor, su complejidad temporal es **O(1)**.

#### bool hash_insertar(hash_t *hash, char *clave, void *valor, void **encontrado)
Esta funcion inserta o reemplaza el valor asociado a una clave.

Antes de insertar, calcula el factor de carga esperado para el hash con el nuevo elemento, y, si supera `MAX_FACTOR_DE_CARGA`, intenta redimensionar multiplicando la capacidad por `FACTOR_REDIM`. 

Luego, calcula `pos` con `obtener_posicion_hash`. Si la clave ya existe en el contenedor, reemplaza valor, y si no existe, crea un nuevo `nodo`, duplica la clave y lo enlaza al frente de la lista en ese contenedor. Al finalizar, incrementa `cantidad`.

Para la funcion `hash_insertar`, la complejidad temporal es **O(n)** donde **n** es la cantidad promedio de elementos que colisionan y terminan en el mismo contenedor.

#### bool hash_contiene(hash_t *hash, char *clave)

Devuelve true si la clave existe, false en caso contrario. Internamente reutiliza la misma lógica de búsqueda por contenedor.

Para el peor caso, donde haya colisiones en una lista, la complejidad temporal se vuelve **O(n)** con **n** la longitud promedio que colisionan y terminan en el mismo contenedor.

#### void *hash_quitar(hash_t *hash, char *clave)

Esta funcion se encarga de eliminar el nodo que coincide con la clave. Primero, ubica el contenedor y busca obteniendo también el anterior.

Luego, desenlaza el nodo ajustando el `sig` del anterior o el inicio de la lista del contenedor. Continua liberando la clave y el nodo, decrementando `cantidad` y devuelve el valor almacenado.

Si no existe, devuelve **NULL**.

Para la funcion `hash_quitar`, la complejidad temporal es **O(n)** con **n** la longitud promedio que colisionan y terminan en el mismo contenedor, porque si bien elimina directamente del contenedor, puede necesitar recorrer la lista si hay colisiones.

#### void hash_destruir(hash_t *hash)

Libera todos los nodos y sus claves y luego la estructura del hash, sin llamar al destructor.
Para `hash_destruir`, la complejidad temporal es **O(n)** porque debe liberar cada nodo y su clave recorriendo todos los elementos de la tabla.

#### void hash_destruir_todo(hash_t *hash, void (*destructor)(void *))

Similar a `hash_destruir`, pero si se pasa destructor, lo invoca sobre cada valor antes de liberar los nodos.

Para `hash_destruir_todo`, simiilar a `hash_destruir`, la complejidad temporal es **O(n)** porque debe liberar cada nodo y su clave recorriendo todos los elementos de la tabla.

### Iterador Interno

Para recorrer el hash, se declaro la funcion `hash_iterar`, definida como:

#### size_t hash_iterar(hash_t *hash, bool (*f)(char *, void *, void *), void *ctx).

Esta funcion recorre contendor por contenedor y nodo por nodo, llamando a `f(clave, valor, ctx)` por cada elemento.

Si f devuelve false, corta tempranamente. Caso contrario recorre la totalidad del hash y en ambos casos devuelve la cantidad de llamadas realizadas.

Como debe recorrer la totalidad del hash, en el peor escenario tendra que recorrer n elementos cayendo a una complejidad temporal de **O(n)**.

