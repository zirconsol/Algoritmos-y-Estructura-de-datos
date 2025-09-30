<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA LISTA - PILA - COLA

## Alumno: Juan Pablo Romano - 96.508 - jpromano@fi.uba.ar

- Para compilar:

```bash
gcc -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O2 -g \
	    main.c src/lista.c src/pila.c src/cola.c src/tp1.c -o lista
```

- Para ejecutar **(ejemplo de busqueda por nombre, chequear comandos)**:

```bash
./lista normal.csv nombre Chikorita
```

- Para ejecutar con valgrind:
```bash
valgrind --track-origins=yes --leak-check=full \
    ./lista normal.csv nombre Chikorita
```

---

#  Funcionamiento

Para el TP de TDAs, se realizaron varias estructuras que garantizaban el correcto funcionamiento e implementación de los tipos de datos abstractos estudiados, para este caso, la **lista simplemente enlazada**, la **pila** y la **cola**, cada uno con su propio archivo fuente `lista.c`, `pila.c`, `cola.c` y su respectivo header `lista.h`, `cola.h`, `pila.h`.

## lista.c
La implementación de la lista elegida fue la **lista simplemente enlazada** en `lista.c`. Para trabajar con los nodos, incorporé un `struct nodo` que contiene un puntero `void` `dato`, y un puntero al nodo `struct nodo *siguiente`. Por otra parte, también definí el `struct` de lista con dos punteros a los nodos `primero` y `último`, además de la cantidad de nodos que almacena la lista en `cantidad`.

Por último, también definí una estructura `lista_iterador` con dos punteros, uno a la lista `lista` y otro al nodo `actual`, ya que el iterador lo vamos a usar para recorrer la totalidad de la lista con un bucle `for`.

## pila.c
La implementación de la pila la hice usando las primitivas de lista adaptadas a los requerimientos que debe tener una pila, correspondientes al principio **LIFO**, y para ello se utilizaron las funciones `lista_insertar`, `lista_eliminar_elemento` y `lista_buscar_elemento`, adaptadas en cada caso acorde a las necesidades del principio.

## cola.c
Por otra parte, y al igual que con la pila, la implementación de la cola la hice reutilizando las primitivas de la lista, pero en este caso, adaptándolas a los requerimientos que tiene una cola, siguiendo el principio **FIFO**, y para ello se utilizaron las funciones de lista `lista_agregar`, `lista_eliminar_elemento` y `lista_buscar_elemento`, adaptadas a las necesidades del principio.


***
### <ins>Primitivas de LISTA
#### lista_t *lista_crear(void)
Función encargada de la creación de la lista. Utiliza `malloc` para reservar memoria para una `lista_t` y un puntero para almacenar la dirección en memoria de la misma. Si la lista fuese **NULL** o inválida, retorna **NULL**. Si estamos ante el caso de que la lista es válida, crea la lista y apunta los punteros `primero` y `último` a **NULL**.  

Como solo inicializa la estructura sin recorrer nada, tiene una complejidad **O(1)**.

<p align="center">
  <img src="https://i.postimg.cc/mgtCzHvH/Captura-de-pantalla-2025-09-25-a-la-s-6-09-19-p-m.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_crear.</em>
</p>
<br><br>

#### bool lista_vacia(lista_t *lista);
Esta función se encarga de verificar si una lista está vacía o no, chequeando si el puntero `primero` apunta a **NULL** o no. Esto lo definí así ya que, al ingresar el primer nodo a la lista, `primero` va a apuntar a ese nodo, y por ende, verificando su estado podemos ver rápidamente si la lista se encuentra vacía o no. En cuanto a su complejidad, como solamente devuelve un valor, tiene una complejidad de **O(1)**.

#### size_t lista_cantidad(lista_t *lista);
Esta función chequea el valor del contador `cantidad` dentro de la `lista_t`, que incrementa o decrementa al realizar operaciones de agregado o quitado de nodos. Al tratarse de una función que accede y devuelve el valor de un campo de la estructura, tiene una complejidad de **O(1)**.

#### bool lista_agregar(lista_t *lista, void *dato);
La función `lista_agregar` tiene como finalidad agregar nodos al final de la lista. Para ello, primero reserva memoria con `malloc` para el nuevo nodo, y apunta los punteros del `nuevo_nodo`: `dato` con la información que se almacena en el nodo, y `siguiente` a **NULL**, ya que el último nodo de una lista enlazada debe apuntar siempre a **NULL**. Luego enlazo los punteros `primero` y `último` con este `nuevo_nodo`, haciendo que ambos punteros lo apunten, para finalmente incrementar en 1 el contador `cantidad`.  

Debido a la estructura de nodos que elegí, con un puntero al último elemento, se evita recorrer la lista y, por ende, tiene una complejidad de **O(1)**.

<p align="center">
  <img src="https://i.postimg.cc/Hnwpq0jJ/Lista-agregar.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_agregar.</em>
</p>
<br><br>

#### bool lista_insertar(lista_t *lista, void *elemento, size_t posición);
Para la función `lista_insertar` trabajé con tres posibles escenarios. Como primer caso, si se quería insertar en la primera posición de la lista, un segundo caso para insertar en la última posición de la lista, y un tercer caso intermedio para cualquier otra posición de la lista.

Para el caso en que se quisiera agregar el elemento al principio de la lista, el programa comienza creando el `nuevo_nodo`, de manera similar a como se hizo en `lista_agregar`. Luego, lo re-enlaza en la lista haciendo que el puntero `primero` de la lista apunte a este `nuevo_nodo`, y que el puntero `siguiente` dentro de `nuevo_nodo` apunte al nodo que, hasta la creación del `nuevo_nodo`, ocupaba la posición de primer nodo.

<p align="center">
  <img src="https://i.postimg.cc/Hkv9cR97/Lista-insertar.png" alt="Lista insertar" width="800"/>
  <br>
  <em>Caso de inserción en primera posición.</em>
</p>
<br><br>

Luego tenemos el caso intermedio, es decir, cuando se desea insertar un nodo entre dos nodos existentes. Al igual que en la inserción en posición cero, primero se reserva la memoria para el `nuevo_nodo`, para luego insertarlo.

Para ello, lo primero que hace la función es definir un puntero `actual` que recorre los nodos hasta hallar el nodo anterior al lugar donde se desea insertar el `nuevo_nodo`, y lo apunta con `actual`. Una vez hecho esto, el `nuevo_nodo` se enlaza con la lista haciendo que su puntero `siguiente` apunte al nodo posterior al señalado como `actual`. De esta manera, queda correctamente enlazado al resto de la lista. Finalmente, se actualiza el puntero `siguiente` dentro del nodo apuntado por `actual` para que apunte al nuevo nodo insertado, y se incrementa la `cantidad` en 1.

<p align="center">
  <img src="https://i.postimg.cc/K8pDkwDK/Lista-insertar-intermedio.png" alt="Lista insertar" width="800"/>
  <br>
  <em>Caso de inserción en posición intermedia.</em>
</p>
<br><br>


Por último, está el caso en que se quisiera insertar un nodo al final de la lista.

Para ello, el programa inicia reservando memoria, como en los dos casos previos, para el `nuevo_nodo`. Luego, utiliza el puntero `último` de la lista para tomarlo como nuevo nodo final, y enlaza el nodo que, hasta ese momento, era el último, haciendo que su puntero `siguiente` pase de apuntar a **NULL** al `nuevo_nodo`.

<p align="center">
  <img src="https://i.postimg.cc/x11PdnMb/Lista-insertar-final.png" alt="Lista insertar final" width="800"/>
  <br>
  <em>Caso de inserción en la ultima posición.</em>
</p>
<br><br>


En cuanto a la complejidad, varía en función de dónde se inserte. Debido a la estructura del nodo con punteros al primer y al último elemento, insertar tanto en la primera como en la última posición tiene una complejidad de **O(1)**, mientras que insertar en una posición intermedia tendrá una complejidad de **O(n)**, ya que dependerá de recorrer hasta el nodo en la posición anterior a la pedida.

#### void *lista_eliminar_primer_elemento(lista_t *lista);

La función `lista_eliminar_primer_elemento` fue diseñada para eliminar el nodo dentro de la lista que se encuentre en la primera posición.

Para ello, el programa define un puntero auxiliar `borrar_nodo` el cual apunta a este primer nodo a borrar.

Luego, apunta el puntero `primero` de la lista al nodo posterior al nodo que se quiere eliminar, desenlazando así el nodo que queremos borrar, de la lista.

Luego, utilizando la función `free`, se libera la memoria ocupada por el nodo, y llegamos al estado final sin ese nodo en la lista.

En cuanto a la complejidad computacional, eliminar el primer nodo de una lista tendrá una complejidad de **O(1)**.

<p align="center">
  <img src="https://i.postimg.cc/kGsDBVts/eliminar-primer-elemento.png" alt="Lista eliminar primer elemento" width="800"/>
  <br>
  <em>Caso de eliminación del primer nodo de la lista.</em>
</p>
<br><br>


#### void *lista_eliminar_ultimo_elemento(lista_t *lista);
Similar a la función `lista_eliminar_primer_elemento`, esta función fue diseñada para eliminar el nodo dentro de la lista que ocupa la última posición. Para ello, el programa define dos punteros auxiliares, `borrar_nodo` que es el puntero que usa para eliminar el nodo, y `anterior`, que es un puntero pensado para recorrer los nodos hasta el `último-1`. Una vez encontrado el anteúltimo nodo, hacemos que el puntero `siguiente` del mismo apunte a **NULL**, y que el puntero `último` de la lista apunte a este nodo. De esa forma, desenlazamos el nodo que queríamos eliminar, y procedemos a eliminarlo utilizando la función `free`. En cuanto a la complejidad de esta función, ya que debe recorrer hasta el nodo anterior al que se quiere eliminar, tendrá una complejidad **O(n)**.

<p align="center">
  <img src="https://i.postimg.cc/wvH7DYcy/Sin-ti-tulo-3.jpg" alt="Lista eliminar último elemento" width="800"/>
  <img src="https://i.postimg.cc/gJdn3bqq/Sin-ti-tulo-4.jpg" alt="Lista eliminar último elemento" width="800"/>

  <br>
  <em>Caso de eliminación del último nodo de la lista.</em>
</p>
<br><br>

#### void *lista_eliminar_elemento(lista_t *lista, size_t posición);
La función `eliminar_elemento`, recibe como parámetros un puntero a la lista, y la posición, y a diferencia de las funciones vistas anteriormente, se encarga de eliminar cualquier nodo en cualquier posición de la lista. Para ello utiliza un puntero auxiliar `borrar_nodo` que es el que utiliza para eliminar el nodo de la lista y liberar memoria, pero además, utiliza el puntero auxiliar `anterior` para recorrer la lista nodo a nodo hasta la posición `posición-1`, para luego modificar el puntero `siguiente` en esta posición haciendo que apunte a **NULL** para luego realizar la ya mencionada liberacion de memoria. Para cerrar, similar a la función `lista_eliminar_ultimo_elemento`, como tenemos que recorrer hasta el nodo anterior al eliminado, tendra una complejidad **O(1n)**.

<p align="center">
  <img src="https://i.postimg.cc/rsXg1mZ5/Sin-ti-tulo-1.jpg" alt="Lista eliminar elemento" width="800"/>
  <img src="https://i.postimg.cc/tJQD3Tv1/Sin-ti-tulo-2.jpg" alt="Lista eliminar elemento" width="800"/>
  <br>
  <em>Caso de eliminación de nodo intermedio de la lista.</em>
</p>
<br><br>

#### int lista_buscar_posicion(lista_t *lista, void *dato, int (*comparador)(const void *, const void *));

La función `buscar_posicion`, recibe como parámetros un puntero a lista, y un puntero a dato, que son la lista donde vamos a buscar, y el dato que vamos a buscar.

Para buscar utiliza un comparador que verifica si el dato a comparar existe o no en alguno de los nodos de la lista. Define tambien un puntero `actual`, que recorre todos los nodos en busca de alguno que satisfaga al comparador.

Luego con un bucle chequea uno a uno los nodos hasta encontrar o no el dato. Si el dato es encontrado devuelve la posición del nodo en la lista, y si no encuentra el dato, devuelve **-1**. Con respecto a la complejidad, ya que debe recorrer uno a uno los nodos hasta encontrar el buscado o no encontrarlo, tendra una complejidad **O(n)**.

#### void *lista_buscar_elemento(lista_t *lista, size_t posicion);

La función `buscar_elemento` tiene una funcionalidad similar a `buscar_posicion`, solo que como parámetros recibe una lista, y una posición, y con un bucle va recorriendo las posiciones de los nodos hasta hallar o no la posición pedida. Si encuentra la posición, devuelve el `dato` correspondiente a ese nodo, y si no encuentra la posición, devuelve **NULL**. En cuanto a la complejidad, similar a `buscar_posicion`, como debe recorrer los nodos uno a uno en busca de coincidencia o error, tendra una complejidad **O(n)**.

#### void lista_destruir(lista_t *lista);

Esta función recibe como parámetro un puntero a la lista y se encarga de destruir los nodos y la estructura de la lista a traves de la función `free`. Es importante mencionar que si bien libera los nodos y la lista, no libera los datos almacenados dentro de cada nodo.
En cuanto a la complejidad, como debe recorrer todos los nodos para liberar la memoria utilizada, tendra una complejidad **O(n)**.

#### void lista_destruir_todo(lista_t *lista, void (*destructor)(void *));

Esta función recibe como parámetros un puntero a la lista, y un destructor, lo que permite que a diferencia de `lista_destruir`, si pueda destruir los datos almacenados en cada nodo. En cuanto a la complejidad, al igual que `lista_destruir`,como debe recorrer todos los nodos para liberar la memoria utilizada, tendra una complejidad **O(n)**.

<hr>

### Función para el iterador interno de la lista.

#### size_t lista_con_cada_elemento(lista_t *lista, bool (*f)(void *, void *), void *extra);

Esta función recibe como parámetros la lista , y un puntero a función `f`. La finalidad de la misma es aplicar una función`f` a los elementos de la lista, permitiendo asi, por ejemplo, cortar el bucle de manera temprana en caso de que lo pedido por la función `f` no haya sido satisfecho. La complejidad para esta función, depende de la cantidad de nodos que tenga la lista ya que hay que recorrerlos uno a uno, haciendo que su complejidad sea **O(n)**.

<hr>

### Funciones del iterador externo de la lista.

#### lista_iterador_t *lista_iterador_crear(lista_t *lista);

Esta función recibe como parámetro un puntero a lista, y crea un iterador interno de la lista `it`. Primero reserva memoria con `malloc` para el iterador, y luego apunta los punteros `lista` dentro del iterador a la lista, y el puntero `actual` al primero nodo de la lista. 

#### bool lista_iterador_hay_mas_elementos(lista_iterador_t *it);
Esta función recibe como parámetro un puntero al iterador, y chequea si se acabaron los nodos en la lista. Para ello simplemente verifica hacia donde apunta el puntero `actual` dentro del iterador, ya que en una lista simplemente enlazada, el último nodo de la lista debe apuntar a **NULL**. Si apunta a **NULL** es porque ya no hay mas nodos que recorrer.

#### void lista_iterador_siguiente(lista_iterador_t *it);
Esta función se encarga de modificar la dirección en la que apunta el puntero `actual` dentro del iterador de la lista.

#### void *lista_iterador_obtener_actual(lista_iterador_t *it);
Esta función recibe como parámetro el iterador y se encarga de devolver el dato almacenado en el nodo `actual`.

#### void lista_iterador_destruir(lista_iterador_t *it);
Esta función recibe como parámetro el puntero al iterador y se encarga de liberar la memoria que habiamos reservado para el iterador.

Con respecto a la complejidad de las funciones del iterador, en todos los casos operan con un parámetro del puntero iterador, por lo cual, para todas las funciones la complejidad sera **O(1)**.


### <ins>Primitivas de PILA y COLA

#### cola_t *cola_crear(void);
Función encargada de la creacion de la cola. Utiliza malloc para reservar memoria para una `cola_t` y un puntero para almacenar la dirección en memoria de la misma. Utiliza la primitiva de lista `lista_crear` para el creado de la lista. Si la cola fuese **NULL** o invalida, retorna **NULL**. Si estamos ante el caso de que la cola es valida, crea la cola y la devuelve. 

Como solo inicializa la estructura sin recorrer nada, tiene una complejidad **O(1)**

#### bool cola_encolar(cola_t *cola, void *elemento);
Función que recibe como parámetro un puntero a la cola, y un puntero a un elemento, *encolando* el elemento a la cola. Utiliza la primitiva `lista_agregar` para agregar al final.

Como utiliza la primitiva `lista_agregar` que tiene una complejidad **O(1)**, esta función tambien sera **O(1)**.


#### void *cola_desencolar(cola_t *cola);
Función que recibe como parámetro un puntero a la cola, y *desencola* el primer elemento de la cola usando la primitiva de lista `lista_eliminar_elemento`, pero como para lista, eliminar el primer elemento tiene una complejidad **O(1)**, esta función tambien tendra esa misma complejidad.

#### void *cola_ver_primero(cola_t *cola);
Función que recibe como parámetro un puntero a la cola, y accede al primer nodo. Como utiliza la primitiva de lista `lista_buscar_elemento` para ver el primer elemento, y esta función tiene complejidad **O(1)**, esta tambien tendra esa complejidad.

#### size_t cola_cantidad(cola_t *cola);
Función que recibe como parámetro un puntero a la cola, y devuelve la cantidad de elementos en la cola utilizando la primitiva de lista `lista_cantidad`. Como esa primitiva de lista tiene una complejided **O(1)**, esta función tambien tendra esa complejidad.

#### void cola_destruir(cola_t *cola);
Función que recibe como parámetro un puntero a la cola, y libera la memoria de la cola usando la primitiva de lista `lista_destruir` que tiene una complejidad **O(n)** por lo cual, esta función tambien tendra esa complejidad.

#### pila_t *pila_crear(void);
Función encargada de la creación de la pila. Utiliza malloc para reservar memoria para una pila_t y un puntero para almacenar la dirección en memoria de la misma. Utiliza la primitiva de lista lista_crear para el creado de la lista.

Si la pila fuese **NULL** o invalida, retorna **NULL**. Si estamos ante el caso de que la pila es válida, crea la pila y la devuelve.

Como solo inicializa la estructura sin recorrer nada, tiene una complejidad **O(1)**.

#### bool pila_apilar(pila_t *pila, void *dato);
Función que recibe como parámetro un puntero a la pila, y un puntero a un dato, *apilando* el elemento en la pila. Utiliza la primitiva de lista lista_agregar cuando la lista está vacia, y `lista_insertar` en la posición 0 cuando ya tiene elementos.

Dado que ambas primitivas tienen complejidad **O(1)** en estos casos, esta función tambien tendrá una complejidad O(1).

#### void *pila_desapilar(pila_t *pila);
Función que recibe como parámetro un puntero a la pila, y *desapila* el elemento que se encuentra en el *tope*. Internamente utiliza la primitiva de lista `lista_eliminar_elemento` en la posición 0, que tiene una complejidad **O(1)**, por lo tanto, esta función también será **O(1)**.

#### void *pila_ver_primero(pila_t *pila);
Función que recibe como parámetro un puntero a la pila, y *desapila* el elemento que se encuentra en el tope. Internamente utiliza la primitiva de lista `lista_eliminar_elemento` en la posición 0, que tiene una complejidad **O(1)**, por lo tanto, esta función también será **O(1)**.


#### bool pila_cantidad(pila_t *pila);
Función que recibe como parámetro un puntero a la pila, y devuelve la cantidad de elementos que contiene. Utiliza la primitiva de lista `lista_cantidad`, que tiene una complejidad **O(1)**, por lo cual esta función también tendrá dicha complejidad.


#### void pila_destruir(pila_t *pila);
Función que recibe como parámetro un puntero a la pila, y libera la memoria utilizada por la pila, incluyendo la lista interna. Utiliza la primitiva de lista `lista_destruir`, que tiene una complejidad **O(n)**, por lo cual esta función también tendrá una complejidad **O(n)**.




## Respuestas a las preguntas teóricas

### Diferencia entre lista, lista enlazada y doblemente enlazada.

Por empezar, la lista como tal es tipo de dato abstracto que define que operaciones estan permitidas, pero no asi como se van a almacenar los datos.

Luego, esta la lista simplemente enlazada, que tiene como diferencial que se almacenan los datos a traves de nodos enlazados, donde cada nodo sabe cual es el nodo siguiente mediante un puntero.

Otra de las caracteristicas de este tipo de lista es que el recorrido de la misma solo se puede hacer en un unico sentido que es el sentido de los nodos consecutivos entre si, y como beneficio, pese a que el recorrido es en una unica dirección, solamente tiene un puntero dentro del nodo hacia otro nodo, y requiere de menos memoria que su contraparte la doblemente enlazada.

<p align="center">
  <img src="https://i.postimg.cc/KzwLLHjF/Sin-ti-tulo-5.jpg" alt="Diagrama lista simplemente enlazada" width="800"/>
  <br>
  <em>Diagrama de una lista simplemente enlazada.</em>
</p>
<br><br>

Por último, se encuentra la lista doblemente enlazada, que tiene caracteristicas similares a la simple, ya que tambien tiene nodos enlazados, pero la principal diferencia radica en que no tiene un unico sentido de recorrido, ya que posee un puntero hacia el nodo previo, lo que permite que en caso de que el usuario lo demande, pueda recorrer la lista en ambas direcciones.

La desventaja de esto es que tiene un mayor uso de memoria y tiene mayor complejidad en las operaciones de inserción y eliminación, comparado con la lista simplemente enlazada.

<p align="center">
  <img src="https://i.postimg.cc/J0XXQ5Df/Sin-ti-tulo-6.jpg" alt="Diagrama lista doblemente enlazada" width="800"/>
  <br>
  <em>Diagrama de una lista doblemente enlazada.</em>
</p>
<br><br>

<hr>

### Lista circular

La lista circular es una variante de la lista enlazada, y tiene como diferencia que en su último nodo, en lugar de apuntar hacia **NULL**, apunta nuevamente al primer nodo de la lista, dando asi la ventaja de que el recorrido se pueda iniciar desde cualquier nodo de la lista, y al tener el último nodo enlazado con el primero, permite un recorrido continuo.

<p align="center">
  <img src="https://i.postimg.cc/SR99Gf2w/Sin-ti-tulo-7.jpg" alt="Diagrama lista simple circular" width="800"/>
  <br>
  <em>Diagrama de una lista simple circular.</em>
</p>
<br><br>

<hr>

### Diferencia entre pila y cola

La diferencia que existe entre una pila y una cola es la manera en la que manejan la inserción de nodos y la eliminación de los mismos.

Por un lado, la pila maneja un principio de **LIFO**, el cual implica que inserta elementos por el *tope*, que en una lista seria el último nodo de la misma, y al eliminar, tambien lo hacemos iniciando por el tope, por lo cual si quisieramos desapilar el primer elemento de una pila, deberiamos desapilar todos los elementos para llegar a desapilar el primero.

<p align="center">
  <img src="https://i.postimg.cc/269px4RT/Sin-ti-tulo-8.jpg" alt="Diagrama Pila" width="800"/>
  <br>
  <em>Diagramas de inserción y eliminación de una pila.</em>
</p>
<br><br>


Por otra parte, la cola, maneja un principio de **FIFO**, el cual implica que se inserta por el *final* al igual que la pila, pero esta se desencola empezando por el primer elemento, con lo cual, si quisieras desencolar el primero elemento, se puede hacer directamente, y si se quisiera desencolar el último elemento insertado, se deberian desencolar todos los elementos de la cola.

<p align="center">
  <img src="https://i.postimg.cc/85X8bhQR/Sin-ti-tulo-9.jpg" alt="Diagrama Cola" width="800"/>
  <br>
  <em>Diagramas de inserción y eliminación de una cola.</em>
</p>
<br><br>

<hr>

### Diferencia entre un iterador interno y externo

La diferencia entre un iterador interno y uno externo es fundamentalmente como permiten manipular el recorrido de los nodos de una lista.

Mientras que para el iterador interno, el recorrido es llevado a cabo por la propia lista, por ejemplo, en nuestro programa con la función `lista_con_cada_elemento` que corta tempranamente con una evaluacion de un bool, el iterador externo es una estructura independiente, y permite asi que el usuario tenga el control sobre como se realiza el recorrido de la lista.

Podemos ver un ejemplo de como se compone un iterador externo con las funciones definidas para la lista, `lista_iterador_hay_mas_elementos`, `lista_iterador_siguiente`, `lista_iterador_obtener_actual`.
