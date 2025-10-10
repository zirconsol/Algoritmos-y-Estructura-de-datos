<div align="right">
<img width="32px" src="img/algo2.svg">
</div>

# TDA ABB

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

Para el TP de TDA ABB, se realizaron varias estructuras que garantizan el correcto funcionamiento e implementación de un **arbol binario de busqueda**, para el cual se realizo una implementacion principal `abb.c`, con su respectivo header `abb.h`. Ademas, para modularizar el codigo y separar las primitivas disponibles para el usuario, se implemento un modulo `abb_aux_rec.c` con su respectivo header `abb_aux_rec.h`, que contiene distintas implementaciones tanto de funciones recursivas, como auxiliares para la correcta implementacion de las primitivas principales.

## abb.c
Para poder implementar correctamente el abb, trabaje con una `estruc_interna.h` predefinida que determina la estructura propia del abb. Para trabajar con los nodos, se utilizaria `struct nodo` que contiene un puntero `void` `dato`, y un puntero al nodo `struct nodo *izq` y `struct nodo *der` correspondientes a las dos posibles ramas de un nodo en un ABB.
Por otra parte, también se definio el `struct` del abb con dos punteros, un puntero a `nodo_t`, que apuntaria a la raiz del abb, y el otro, un puntero int a un `comparador`, que servira para recorrer el arbol.


***
### <ins>Primitivas de ABB
#### abb_t *abb_crear(int (*cmp)(const void *, const void *))
Función encargada de la creación del arbol. Recibe como parametro un comparador para el arbol.

Revisa si el comparador es valido, para luego, con `calloc`, reservar la memoria para la estructura.
Luego, asigna el comparador que se ingreso como parametro al comparador del arbol.
```
if (!cmp) {
  return **NULL**;
}
abb_t *arbol = calloc(1, sizeof(abb_t));
```

Al finalizar, devuelve el arbol ya creado.
```
return arbol;
```

Como solo inicializa la estructura sin recorrer nada, tiene una complejidad **O(1)**.

<p align="center">
  <img src="https://i.postimg.cc/mgtCzHvH/Captura-de-pantalla-2025-09-25-a-la-s-6-09-19-p-m.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_crear.</em>
</p>
<br><br>

#### bool abb_esta_vacio(abb_t *abb);
Esta función se encarga de verificar si el arbol está vacío o no, chequeando si el puntero `raiz` apunta a ****NULL**** o no. Esto lo definí así ya que, al ingresar el primer nodo al arbol, `raiz` va a apuntar a ese nodo, y por ende, verificando su estado podemos ver rápidamente si el arbol tiene raiz,y por ende, si se encuentra vacío o no.

En cuanto a su complejidad, como solamente devuelve un valor, tiene una complejidad de **O(1)**.

#### size_t abb_cantidad(abb_t *abb);
Esta función chequea si el arbol es valido, y luego devuelve el valor del contador `cantidad` dentro de `abb_t`, que incrementa o decrementa al realizar operaciones de agregado o quitado de nodos en el arbol. Al tratarse de una función que accede y devuelve el valor de un campo de la estructura, tiene una complejidad de **O(1)**.

#### bool abb_insertar(abb_t *abb, void *dato)
La función `abb_insertar` tiene como finalidad agregar nodos al arbol. Para ello, primero verifica si el arbol y el comparador son validos, para luego utilizar la funcion auxiliar `abb_insertar_nodo_rec`.
 Esta funcion reserva memoria con `calloc` para el nuevo nodo, y apunta los punteros del `nodo` segun su `dato`, y sus punteros `izq` y `der` a ****NULL****.
```
 if (nodo == **NULL**) {
	nodo_t *nuevo_nodo = calloc(1, sizeof(nodo_t));
	if (nuevo_nodo != **NULL**) {
		nuevo_nodo->dato = (void *)dato;
		*insertado = true;
	}
	return nuevo_nodo;
}
  ```
 
 Luego, realiza la comparacion de insercion, aqui, el problema tendra tres caminos, uno de ellos, que no haya elementos para comparar, con lo cual, el nodo se inserta en la raiz del arbol. Para ello, simplemente apunta `*raiz` del `abb_t` al `nodo` creado y se aumenta la `cantidad` en uno.
```
 abb->raiz = abb_insertar_nodo_rec(abb->raiz, (void *)dato, abb->comparador, &insertado);

  if (insertado){
    abb->cantidad++;
  }
```

Para el caso de insertar en la raiz, estamos ante el mejor caso de complejidad, que tendria una complejidad de **O(1)**.

<p align="center">
  <img src="https://i.postimg.cc/Hnwpq0jJ/Lista-agregar.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_agregar.</em>
</p>
<br><br>

Los otros dos caminos a la hora de insertar, son si el dato del nodo a insertar es mayor o menor al dato que ya esta en la raiz, o bien, en el nodo padre.

Para simplificar, utilice una llamada recursiva para asi analizar un nodo con potenciales izquierda y derecha, y de esa manera, si tuviera que bajar un nivel, simplemente vuelve a llamarse a si misma y continua comparando hasta lograr insertar.

Para el caso en que el nodo a insertar es mayor que su nodo padre, posterior a reservarle la memoria al nodo con `calloc`, se intenta insertar a la derecha del padre.

Para esto, dentro de la funcion `abb_insertar_nodo_rec` utiliza el comparador, chequea si es posible insertar en esa posicion, si no se puede insertar, hace una llamada recursiva con el nodo hijo y repite hasta encontrar un lugar donde insertar, para luego apuntar el punter `*der` del nodo padre correspondiente al hijo a insertar.

Al igual que para insertar a la derecha, para insertar un nodo con dato menor al del padre, el programa realiza la misma tarea, pero en lugar de usar el puntero `*der` del nodo, utiliza el puntero `*izq` correspondiente al lado izquierdo para asi insertar menores o iguales a la izquierda del padre.

Para el caso de insertar a izquierda o derecha, la complejidad se torna **O(n)** como peor caso, mejorando a **O(log(n)**) si se trata de un arbol balanceado.
```
int cmp = comparador(dato, nodo->dato);
  if (cmp <= 0)
    nodo->izq = abb_insertar_nodo_rec(
      nodo->izq, dato, comparador, insertado);
  else
    nodo->der = abb_insertar_nodo_rec(
      nodo->der, dato, comparador, insertado);
	}
	return nodo;
```

<p align="center">
  <img src="https://i.postimg.cc/Hnwpq0jJ/Lista-agregar.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_agregar.</em>
</p>
<br><br>
///
<p align="center">
  <img src="https://i.postimg.cc/Hnwpq0jJ/Lista-agregar.png" alt="Lista agregar" width="800"/>
  <br>
  <em>Diagrama de la función lista_agregar.</em>
</p>
<br><br>
////
///
///
//
//




#### void *abb_eliminar(abb_t *abb, void *dato)

La función `abb_eliminar` recibe como parametros un puntero a la estructura arbol `abb`, un puntero al dato `dato`, y tiene como finalidad eliminar el nodo correspondiente al dato pasado como parametro.

Para ello, el programa define dos punteros auxiliares, `padre` y `actual`, que los utiliza para ir recorriendo el arbol. Luego, realiza un bucle de comparacion entre el dato pasado como parametro y el dato del puntero `actual` hasta hallar o no una coincidencia.

Si encuentra el nodo a eliminar, procedemos a una comparacion mas, para saber de que tipo de eliminacion se trata, ya que las mismas fueron categorizadas en tres, una con cada funcion, segun el estado del nodo: nodo hoja, nodo con un hijo, o nodo con dos hijos.

Para el nodo hoja, es decir, sin hijos, la eliminacion se realiza reconectando el arbol con la funcion auxiliar `reconectar_arbol(abb_t *abb, nodo_t *padre, nodo_t *nodo_viejo,nodo_t *nodo_nuevo)`, que simplemente verifica, primero, si el nodo tiene padre, para luego desconectar el nodo a eliminar, del arbol y luego liberar la memoria de ese nodo con la funcion auxiliar `liberar_nodo`.
```
// Fragmento dentro de reconectar_arbol
if (padre) {
  if (padre->izq == nodo_viejo) {
    padre->izq = nodo_nuevo;
  } else {
    padre->der = nodo_nuevo;
  }
} else {
  abb->raiz = nodo_nuevo;
}

// Funcion liberar_nodo
void liberar_nodo(abb_t *abb, nodo_t *nodo)
{
	free(nodo);
	abb->cantidad--;
}
  ```

<p align="center">
  <img src="https://i.postimg.cc/kGsDBVts/eliminar-primer-elemento.png" alt="Lista eliminar primer elemento" width="800"/>
  <br>
  <em>Caso de eliminación del primer nodo de la lista.</em>
</p>
<br><br>



///////



Luego, para el nodo con un hijo, utiliza un puntero `*hijo` para identificar el hijo del nodo que vamos a eliminar, para luego con la funcion `reconectar_arbol`, conectar el nodo padre del eliminado, con el nodo hijo del eliminado, para luego, liberar la memoria del nodo desconectado del arbol con `liberar_nodo`.

```
// Fragmento de la funcion eliminar_nodo_con_un_hijo

if (nodo->izq) {
  hijo = nodo->izq;
} else {
  hijo = nodo->der;
}
reconectar_arbol(abb, padre, nodo, hijo);
liberar_nodo(abb, nodo);
```


Para finalizar, tenemos el caso de querer eliminar un nodo que tiene tanto hijo a la izquierda como a la derecha. Para este caso defini la funcion auxiliar `eliminar_nodo_con_dos_hijos(abb_t *abb, nodo_t *nodo, nodo_t *padre)`, y una funcion `buscar_maximo` para hallar el predecesor del nodo a eliminar.

Una vez que tenemos el predecesor, copiamos el dato del predecesor, al nodo que queriamos eliminar, de esta manera, quedan dos nodos con el mismo valor. Por ultimo, borramos el predecesor y luego liberamos el nodo eliminado.

```
//Fragmento de buscar maximo
while (nodo->der) {
  if (padre){
    *padre = nodo;
  nodo = nodo->der;
  }
}
return nodo;


// Fragmento de la funcion eliminar_nodo_con_dos_hijos
(void)padre;
nodo_t *padre_pred = nodo;
nodo_t *pred = buscar_maximo(nodo->izq, &padre_pred);
nodo->dato = pred->dato;

if (!pred->izq && !pred->der) {
  eliminar_nodo_hoja(abb, pred, padre_pred);
} else {
  eliminar_nodo_con_un_hijo(abb, pred, padre_pred);
}
```

<p align="center">
  <img src="https://i.postimg.cc/kGsDBVts/eliminar-primer-elemento.png" alt="Lista eliminar primer elemento" width="800"/>
  <br>
  <em>Caso de eliminación del primer nodo de la lista.</em>
</p>
<br><br>



#### void *abb_buscar(abb_t *abb, void *dato)

La función `abb_posicion`, recibe como parámetros un puntero al arbol, y un puntero a dato, que son la el arbol donde vamos a buscar, y el dato que vamos a buscar.

Para buscar utiliza dentro de una funcion auxiliar `abb_buscar_nodo_rec`, un comparador que verifica si el dato a comparar existe o no en alguno de los nodos del arbol. Para ello, compara si el dato del nodo en el que nos encontramos coincide o no con el buscado. Si no coincide, en base al resultado del comparador, se llama recursivamente pero cambiando el parametro inicial por el nodo de la izquierda si es menor, o de la derecha si mayor. Si al finalizar encuentra el nodo, lo devuelve, y si se queda sin nodos para comparar, el comparador devuelve **NULL** y se devuelve **NULL** como respuesta a no haber encontrado el nodo con el dato buscado.



#### void abb_destruir(abb_t *abb);

Esta función recibe como parámetro un puntero el arbol y se encarga de destruir los nodos y la estructura del arbol a traves de la función recursiva `destruir_nodo_rec`, que destruye los nodos a izquierda y derecha de cada nodo, para luego, si existiese un destructor, destruir el `dato` del nodo.

En cuanto a la complejidad, como debe recorrer todos los nodos para liberar la memoria utilizada, tendra una complejidad **O(n)**.

#### void abb_destruir_todo(abb_t *abb, void (*destructor)(void *));

Esta función recibe como parámetros un puntero al, y un destructor, lo que permite que a diferencia de `abb_destruir`, si pueda destruir los datos almacenados en cada nodo cuando llama recursivamente a `destruir_nodo_rec`.

En cuanto a la complejidad, al igual que `abb_destruir`,como debe recorrer todos los nodos para liberar la memoria utilizada, tendra una complejidad **O(n)**.

<hr>

### Función para el iterador interno del arbol.

#### size_t abb_con_cada_elemento(abb_t *abb, enum abb_recorrido modo, bool (*f)(void *, void *), void *extra)

Esta función recibe como parámetros el arbol , un modo de recorrido dado por un enum, que permite definir como se quiere recorrer el arbol y un puntero a función `f`.

La finalidad de la misma es aplicar una función`f` a los elementos del arbol segun el orden establecido.

Los recorridos utilizados para ABB son **INORDEN**, que recorre el arbol nodo a nodo de la siguiente manera, primero el nodo de la izquierda, luego la raiz o nodo padre, y luego el nodo de la derecha, permitiendo recibir un recorrido en orden ascendente de los elementos del arbol.

Luego tenemos el recorrido **PREORDEN**, que recorre primero la raiz o nodo padre, luego el nodo de la izquierda, y luego el nodo de la derecha, de esta manera, recibiendo como resultado un orden que permite reconstruir el arbol.

Por ultimo, tenemos el recorrido **POSTORDEN**, que recorre primero el nodo de la derecha, luego el de la izquierda, y luego la raiz, que resulta muy conveniente para la eliminacion de nodos del arbol ya que evita los casos de eliminacion de nodos con uno o dos hijos.

<hr>



### Uso del iterador, vectorizar el arbol

#### size_t abb_vectorizar(abb_t *abb, enum abb_recorrido tipo_recorrido, size_t cant, void **vector);


## Respuestas a las preguntas teóricas


