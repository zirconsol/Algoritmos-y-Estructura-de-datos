<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA LISTA

## Alumno: Juan Pablo Romano - 96.508 - jpromano@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g main.c src/tp1.c src/funciones.c -g -o tp1
```

- Para ejecutar **(ejemplo de busqueda por nombre, chequear comandos)**:

```bash
./tp1 pokedex.csv buscar nombre Pikachu
```

- Para ejecutar con valgrind:
```bash
valgrind --track-origin=yes --leak-check=full ./tp1 ejemplos/normal.csv
```

---

#  Funcionamiento

Durante el TP1, se realizaron varias estructuras para un correcto funcionamiento del programa como por ejemplo`tp1.c` con su respectivo header `tp1.h`. Ademas, para evitar que el `main.c` quedara por demas extenso, opte por definir un segundo archivo denominado `funciones.c` que utilizaria exclusivamente para mantener compacto `main.c` a traves de llamadas directamente a las funciones. Sumado a `funciones.c`, incorpore un segundo header `funciones.h` donde se listan las primitivas de las distintas funciones disponibles para el usuario.

## tp1.c
La implementacion general de `tp1.c` corresponde a la gestion de archivos de texto, ya sean CSV o TXT para luego realizar distintas operaciones sobre sus registros. La complejidad de `tp1.c` radicaba no solo en la lectura de los archivos a traves de funciones como `leer_linea`, sino ademas, de un correcto manejo de memoria dinamica para el mismo debido al desconocimiento del tamaño de cada linea. Para ello, posterior a su lectura, la informacion recopilada se guardaria en un vector dinamico de `struct pokemon`, que nos permitia añadir distintas funcionalidades como la busqueda por ID, o el recorrido de los registros, ademas de operar sobre el mismo con operaciones de conjuntos, como por ejemplo, la union de dos archivos distintos para unificarlos.
Resumido esto, a continuacion dejo un desglose de las distintas estructuras utilizadas para el desarrollo de `tp1.c`.

<div align="center">
<img width="70%" src="/diagrama3.png">
</div>
#### Diagrama del flujo del programa desde el comienzo con la apertura del archivo hasta la obtencion de la pokedex.

***
### <ins>Estructuras para la lectura y parseo del archivo
#### char *leer_linea(FILE *f);
Funcion encargada de la lectura de lineas con tamaño desconocido. Lee caracter a caracter con `fgetc()` Utiliza `realloc` para un crecimiento del buffer de memoria en caso de requerirlo. Tiene una complejidad O(n) con n la cantidad de caracteres de la linea.

#### size_t split_por_comas(char *linea, char **campos, size_t max_campos);
Se encarga de recorrer la linea, y donde encuentra el separador, reemplaza por '\0' y guarda en `campos[i]` la direccion del caracter siguiente.

Recibe un `*linea` que es la linea a procesar, un `**campos`, que es un arreglo de punteros que referencian donde inicia cada campo y `max_campos` que son la cantidad de campos a separar.
Su complejidad computacional depende de la longitud de la linea, asi que tambien tendra una complejidad de O(n).

#### bool mapeo_tipo_pokemones(char *tipo_leido, enum tipo_pokemon *ptipo);
Se encarga de chequear que el tipo sea valido y traducirlo a enum para luego ser procesado.
Recibe `*tipo_leido` que es el codigo del tipo de pokemon y `*ptipo` que es la direccion de memoria donde se va a almacenar el valor del enum que le corresponda. La complejidad tambien depende de la cantidad de tipos que haya, si fueran n seria O(n), para nuestro caso es O(8) porque esa es la cantidad de tipos disponibles.

#### char *convertir_tipo_enum_a_string(enum tipo_pokemon poketipo);
Se encarga de nuevamente pasar el tipo de pokemon desde su convertido en enum a string. Es un paso adicional que utilizamos para pasar de "TIPO_PLAN" a "PLAN". Recibe un enum `poketipo` y devuelve un `*char` que guarda la direccion en memoria donde esta el string del tipo almacenado.


#### int parsear_pokemon(char *linea, struct pokemon *pokemon_parseado);
Se encarga del pasaje de una linea de un CSV a un `struct pokemon` valido. Recibe un `*linea` que representa el registro de un pokemon con sus campos separados y `*pokemon_parseado` que es el puntero que almacena donde se van a guardar los datos en memoria. Llama a `split_por_comas` para separar correctamente los campos, valida los stats del pokemon usando `strtol`, convierte el tipo de pokemon con `mapeo_tipo_pokemones` y reserva memoria dinamica para el campo nombre usando `malloc` y copiando el string del nombre con `memcpy`. Ademas realiza las gestiones de memoria dinamica (ver ejemplos de manejo de memoria dinamica mas abajo). Tiene tambien una complejidad de O(n) siendo n la longitud de la linea.

#### struct tp1 *tp1_leer_archivo(const char *ruta);
Lee un archivo de texto indicado por `*ruta`, luego parsea cada linea valida a `struct pokemon`, descarta los duplicados por id, almacena los pokemon en un vector dinamico y finalmente ordena el vector por id para retornar el tp1_t correcto o NULL si hay algun error.
Como tenemos un ordenamiento por seleccion con una complejidad de O(n²) para ordenar por id, pese que el parseo es de complejidad O(n), el peor caso posible es O(n²) y esa sera su complejidad computacional.


<div align="center">
<img width="70%" src="/diagrama1.png">
</div>
#### Diagrama de como funciona el `malloc` y `realloc` en la lectura de linear y lectura de un archivo 


***
### <ins>Funciones de gestion de memoria dinamica

#### void tp1_destruir(tp1_t *pokedex);
tp1_destruir se encarga de liberar toda la memoria que tenga asociada una pokedex `tp1_t`, tanto el vector dinamico de pokemons como el nombre de cada pokemon. Recibe como parametro un `*pokedex` que almacena la direccion en memoria de la estructura creada `tp1_t`, y al ser void, no tiene salida, simplemente libera memoria. Su complejidad computacional depende de la cantidad de pokemons almacenados, por lo cual sera O(n) para n pokemones almacenados.


<div align="center">
<img width="70%" src="/diagrama2.jpg">
</div>
#### Diagrama de como liberamos la memoria con tp1_destruir

***
### <ins>Busquedas

#### struct pokemon *tp1_buscar_id(tp1_t *pokedex, int id);
Esta funcion se encarga de recibir `*pokedex` que es un puntero a la estructura `tp1_t` que contiene un vector dinamico de pokemons, y de buscar un pokemon cuyo id coincida con el que se recibe como parametro `id` recorriendo uno a uno el vector. Si halla una coincidencia, devuelve un puntero al `struct pokemon` y caso contario devuelve NULL. Ademas, tiene una complejidad temporal de O(n) ya que depende de la cantidad de pokemones en la pokedex y de tener que recorrerla en su totalidad como peor escenario.

#### struct pokemon *tp1_buscar_nombre(tp1_t *pokedex, const char *pokenombre);
Esta funcion se encarga de recibir `*pokedex` que es un puntero a la estructura `tp1_t` que contiene un vector dinamico de pokemons, y de buscar un pokemon cuyo nombre coincida con el almacenado en memoria al que apunta `*pokenombre`. Tiene una complejidad de O(n) ya que depende de la cantidad de pokemones en la pokedex.
***
### <ins>Iterador

#### size_t tp1_con_cada_pokemon(tp1_t *pokedex, bool (*f)(struct pokemon *, void *), void *extra);
Esta funcion recibe como parametros un `*pokedex` que apunta a la estructura `tp1_t`, un puntero a funcion, *f, y un extra en caso de necesitarlo y simplemente se encarga de iterar sobre cada pokemon almacenado en la pokedex y de aplicarles una funcion f. Por si sola, esta funcion tendria una complejidad computacional de O(n) con n la cantidad de pokemones almacenados en el vector dinamico, pero como ordenamos los pokemons por id con un algoritmo de ordenamiento por seleccion que tiene una complejidad de O(n²), la complejidad final resultara ser de O(n²).

***
### <ins>Acumuladores

#### size_t tp1_cantidad(tp1_t *tp);
Esta funcion recibe un puntero `*tp` a la pokedex en la estructura `tp1_t` y devuelve la cantidad de pokemons validos y unicos.  Tiene una complejidad computacional de O(1) ya que lee directamente el campo `tp->cantidad_pokemones`.

***
### <ins>Operaciones de conjuntos

#### tp1_t *tp1_union(tp1_t *un_tp, tp1_t *otro_tp);
Esta funcion recibe como parametro dos punteros `un_tp` y `otro_tp`que apuntan a la direccion de memoria donde estan almacenadas las dos pokedex, y obtiene como resultado una nueva estructura `tp_1` con los datos de ambas pokedex, priorizando en caso de repetidos a los de `un_tp`, y resolviendo copiar una sola pokedex en caso de que alguna de las dos fuera invalida. Ademas realiza una gestion de memoria dinamica que puede verse en ejemplos de memoria dinamica mas abajo. En cuanto a la complejidad computacional, como decidimos ordenar por id previo a la union, redujimos la complejidad computacional de O(n²), que es lo que habria dado al tener que ordenarlos en el momento de unirlos a O(n), por tener cada pokedex previamente ordenada.

#### tp1_t *tp1_interseccion(tp1_t *un_tp, tp1_t *otro_tp);
Esta funcion recibe como parametro dos punteros `un_tp` y `otro_tp`que apuntan a la direccion de memoria donde estan almacenadas las dos pokedex, y obtiene como resultado una nueva estructura `tp_1` con los datos en comun entre ambas pokedex. Tambien tiene una gestion de memoria dinamica que puede verse en ejemplos de memoria dinamica mas abajo. En cuanto a la complejidad computacional, al igual que ocurre con la union, redujimos la complejidad computacional de O(n²), que es lo que habria dado al tener que ordenarlos en el momento de unirlos a O(n), por tener cada pokedex previamente ordenada.

#### tp1_t *tp1_diferencia(tp1_t *un_tp, tp1_t *otro_tp);
Esta funcion recibe como parametro dos punteros `un_tp` y `otro_tp`que apuntan a la direccion de memoria donde estan almacenadas las dos pokedex, y obtiene como resultado una estructura compuesta de los pokemons que estan en la estructura correspondiente a `un_tp` pero no se encuentran en `otro_tp`.
Tambien tiene una gestion de memoria dinamica que puede verse en ejemplos de memoria dinamica mas abajo ya que funciona igual que en union e interseccion. En cuanto a la complejidad computacional, como ya mencionamos previamente, al igual que ocurre con la union e interseccion, redujimos la complejidad computacional de O(n²), que es lo que habria dado al tener que ordenarlos en el momento de unirlos a O(n), por tener cada pokedex previamente ordenada.


***
### <ins>Guardado de archivo
#### tp1_t *tp1_guardar_archivo(tp1_t *estructura_tp1, const char *ruta_archivo);
La funcion guardar recibe parametros una `estructura_tp1_t` cuya direccion en memoria es almacenada por `*estructura_tp1` y la guarda en un archivo en la ruta a la cual apuntamos con `*ruta_archivo`. La complejidad computacional de la misma tambien depende de la cantidad de pokemons en el vector dinamico al que apuntamos con `estructura_tp1_t` con lo cual su complejidad computacional sera de O(n) para n pokemones.


***
## funciones.c

La implementacion de `funciones.c` aparece de la necesidad de compactar y dejar de forma mucho mas legible el `main.c`. Si bien no era necesario agregar ese modulo, su incorporacion permite tener un `main.c` mucho mas conciso, y permitiendo ademas, un acceso a las primitivas de `funciones.c` de forma que de ser necesario, se pudieran limitar o incorporar funcionalidades en un futuro.
`funciones.c` ademas, implementa todos las funciones necesarias a ejecutar por linea de comando. 


<div align="center">
<img width="70%" src="img/diagrama1.svg">
</div>

## Ejemplos de manejo de Memoria Dinamica
#### Lectura de linea

En el archivo `tp1.c` la función `leer_linea` utiliza `realloc` para agrandar el buffer utilizado en la lectura de la linea caracter a caracter. En mi caso, utilice `nuevo_buffer` como puntero auxiliar permitiendo que `buffer` siga apuntando a la memoria previa, de manera que no se pierda el `buffer` original, y asi ademas evitar fugas. Si `nuevo_buffer == NULL`, el `buffer` original sigue siendo valido, no se pierde la referencia, cortamos con la lectura y luego liberamos memoria.

```c
if (usado + 1 > tamaño) {
    size_t nuevo_tamaño =
        (tamaño == 0) ?
            MEMORIA_INICIAL :
            tamaño * FACTOR_CRECIMIENTO;
    nuevo_buffer = realloc(buffer, nuevo_tamaño);
    if (!nuevo_buffer) {
        error = true;
    } else {
        buffer = nuevo_buffer;
        tamaño = nuevo_tamaño;
    }
}
```


<div align="center">
<img width="70%" src="img/diagrama2.svg">
</div>


#### Ampliado de capacidad del vector dinamico

Siguiendo con funciones del archivo `tp1.c`, la función `ampliar_capacidad` tambien utiliza `realloc` para el crecimiento del vector dinamico de `struct pokemon`, en este caso para guardar los pokemons sin depender de conocer previamente la cantidad de pokemones a almacenar. Esta vez, el puntero auxiliar utilizado es `nuevo_pokemon`, que se usa para guardar el resultado del `realloc`, asi evitando fugas, asegurando que el puntero original se preserve en caso de fallos. Si el `realloc` se  pudo realizar correctamente, `nueva_capacidad` toma el nuevo tamaño del vector y se actualiza la pokedex con el `nuevo_pokemon` mediante `pokedex->pokemons = nuevo_pokemon`

```c
struct pokemon *nuevo_pokemon = realloc(
		pokedex->pokemones, nueva_capacidad * sizeof(struct pokemon));
	if (!nuevo_pokemon) {
		return 0;
	}

	pokedex->pokemones = nuevo_pokemon;

	for (size_t i = capacidad_actual; i < nueva_capacidad; i++) {
		pokedex->pokemones[i] = (struct pokemon){ 0 };
	}
	pokedex->capacidad_pokemones = nueva_capacidad;
	return 1;
```


#### Parseado del pokemon
Tambien, tuve que tener presente la gestion de memoria dinamica en `tp1.c` en la funcion `parsear_pokemon`. En esta, entre varias de las funcionalidades que incorpore, implemente que la linea de texto del archivo tomara la forma de un `struct pokemon` que pudiera ser copiado. La implementacion de memoria dinamica ocurre utilizando `malloc`, para asi poder reservar el buffer de memoria para el campo del nombre, y luego copiar el string que le corresponde al nombre del pokemon con `memcpy`. Asi, conseguimos un `struct pokemon` temporal en el stack que luego puedo utilizar para copiarlo a un vector dinamico en la pokedex.

```c
size_t longitud_nombre = strlen(campos[NOMBRE]);
	pokemon_parseado->nombre = (char *)malloc(longitud_nombre + 1);
	if (pokemon_parseado->nombre == NULL) {
		return 0;
	}
	memcpy(pokemon_parseado->nombre, campos[NOMBRE], longitud_nombre + 1);

```


#### Copiado del pokemon parseado

Otra de las funciones que utilice donde tuve que gestionar memoria dinamica dentro de `tp1.c` fue en `copiar_pokemon_parseado`. Como lo describe su nombre, la utilice para copiar un `struct pokemon` desde `fuente` hacia una posicion dentro del vector dinamico que almacena los pokemons a la que llame `destino`. Para hacerlo, copie el contenido del struct, pero asignando un nuevo buffer en el nombre `destino` con `malloc` y copiando el contenido con `memcpy`. De esa forma, cada `struct pokemon` tendra su propio lugar en memoria para el nombre, evitando asi errores de manejo de memoria ocasionados por liberar un puntero de tipo char al que referenciaban dos punteros diferentes.


```c

*destino = *fuente;
	destino->nombre = NULL;
	size_t n = strlen(fuente->nombre);
	destino->nombre = malloc(n + 1);
	if (!destino->nombre) {
		return 0;
	}
	memcpy(destino->nombre, fuente->nombre, n + 1);
	return 1;

```

#### Operaciones de conjuntos Union, Interseccion y Diferencia

Si bien `tp1_union`, `tp1_interseccion`, y `tp1_diferencia ` son funciones son independientes unas de otras en `tp1.c`, todas gestionan memoria dinamica de forma similar, marcando la diferencia de una y otra en las condiciones de los bucles iteradores.

La manera en que utilizamos memoria dinamica en este caso fue mediante `calloc`, para asi inicializar la pokedex con todos sus campos en `0` luego de reservar el buffer memoria. Luego, se recorren todos los elementos, invocando la funcion `ampliar_capacidad` si fuera necesario, para proceder con la copia de los pokemons utilizando `copiar_pokemon_parseado`. Ante cualquier fallo, ya sea de capacidad, o durante la copia, se continua con la liberacion en el siguiente orden, usando `free(nombre)` para los strings ya asignados, `free(pokemones)` para liberar el vector dinamico, y `free(tp1_t)` para la liberacion de estructura principal, para luego devolver `NULL`, evitando asi que quede memoria sin liberar.


```c
//El puntero para este ejemplo es union, pero como mencione, en los tres la MD se maneja igual
tp1_t *union_tps = calloc(1, sizeof(tp1_t)); 

	if (!union_tps) {
		return NULL;
	}
    /*variables necesarias*/
    while (/*condicion de union*/){
        struct pokemon *pokemon_elegido = NULL;

        /*chequeo de capacidad*/
        if (!ampliar_capacidad(union_tps)) {
			if (union_tps) {
				for (size_t i = 0;
				     i < union_tps->cantidad_pokemones; i++) {
					free(union_tps->pokemones[i].nombre);
				}
				free(union_tps->pokemones);
				free(union_tps);
			}
			return NULL;
		}
        /*copiado de pokemon*/
       if (!copiar_pokemon_parseado(
			    (struct pokemon *)pokemon_elegido,
			    &union_tps->pokemones
				     [union_tps->cantidad_pokemones])) {
			if (union_tps) {
				for (size_t i = 0;
				     i < union_tps->cantidad_pokemones; i++) {
					free(union_tps->pokemones[i].nombre);
				}
				free(union_tps->pokemones);
				free(union_tps);
			}
			return NULL;
		}
		union_tps->cantidad_pokemones++;
	}
	union_tps->cantidad_total_registros = union_tps->cantidad_pokemones;
	return union_tps;
```

---

## Respuestas a las preguntas teóricas
### Complicaciones del trabajo practico

Durante la implementacion de varias de las funciones, tuve muchas dificultades en como interpretar el header de `tp1.h`, como por ejemplo el manejo de memoria dinamica, cuando designar punteros auxiliares, o incluso algo mas trivial como el manejo de potenciales errores y casos bordes.

Tambien me encontre con el problema de la complejidad temporal y como abaratar costos en algunas funciones, que me llevo a tener que refactorizar el codigo en varias ocasiones, como por ejemplo las operaciones de conjuntos que no solo eran costosas, de O(n²) sino que ademas eran dificiles de leer por tener desordenados los vectores dinamicos con los pokemones, lo que hacian que el bloque de las funciones quedara demasiado extenso 