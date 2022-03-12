#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "abb.h"

#define ERROR -1
#define EXITO 0
#define IGUAL 0
#define MAYOR 1
#define MENOR -1
#define VACIO 0

typedef struct arreglo{
  void** array;
  int tamanio_array;
  int posicion;
}arreglo_t;

/*--------------------------------------------------Crear--------------------------------------------------*/
abb_t* arbol_crear(abb_comparador comparador, abb_liberar_elemento destructor){
  if (!comparador)
    return NULL;

  abb_t* arbol = calloc(1,sizeof(abb_t));
  arbol->comparador = comparador;
  arbol->destructor = destructor;
  return arbol;
}

/*
 * Crea un nuevo nodo, reservando memoria la memoria necesaria
 * y asignando el elemento corresponiente a dicho nodo.
 */
nodo_abb_t* crear_nuevo_nodo(void* elemento){

  nodo_abb_t* nuevo = calloc(1,sizeof(nodo_abb_t));
  nuevo->elemento = elemento;
  return nuevo;
}

/*--------------------------------------------------Insertar--------------------------------------------------*/
/*
 * Recorre el arbol hasta encontrar la posicion donde debe insertarse
 * el nuevo nodo. Este nuevo nodo ya viene creado y cos sus datos ya
 * cargados.
 */
nodo_abb_t* insertar_nodo(abb_comparador comparador, nodo_abb_t* actual, nodo_abb_t* nuevo){

  if (!actual){
    actual = nuevo;
    return actual;
  }

  int comparacion = comparador(actual->elemento, nuevo->elemento);

  if (comparacion == MAYOR) {
    actual->izquierda = insertar_nodo(comparador,actual->izquierda,nuevo);
  } else {
    actual->derecha = insertar_nodo(comparador,actual->derecha,nuevo);
  }
  return actual;
}

int arbol_insertar(abb_t* arbol, void* elemento){
  if (!arbol)
    return ERROR;

  nodo_abb_t* nuevo = crear_nuevo_nodo(elemento);
  if (!nuevo)
    return ERROR;

  if (!arbol->nodo_raiz) {
    arbol->nodo_raiz = nuevo;
  } else {
    insertar_nodo(arbol->comparador, arbol->nodo_raiz, nuevo);
  }

  return EXITO;
}

/*--------------------------------------------------Otras--------------------------------------------------*/
void* arbol_raiz(abb_t* arbol){
  if (!arbol || !arbol->nodo_raiz)
    return NULL;
  return arbol->nodo_raiz->elemento;
}

bool arbol_vacio(abb_t* arbol){
  if (!arbol || !arbol->nodo_raiz)
    return true;
  return false;
}

/*
 * Recorre el arbol hasta encontrar el nodo con el elemento igual al
 * pedido. Devuelve dicho nodo y tambien se le puede pasar por parametro
 * un puntero para guardar la dirrecion de memoria del padre del nodo buscado.
 */
nodo_abb_t* buscar_nodo (abb_comparador comparador, nodo_abb_t* nodo, nodo_abb_t** padre, void* elemento){

  if (!nodo)
    return NULL;

  int resultado = comparador(nodo->elemento, elemento);

  if (resultado == IGUAL)
    return nodo;

  if (padre)
    *padre = nodo;

  if (resultado == MAYOR) {
    nodo = buscar_nodo(comparador, nodo->izquierda, padre, elemento);
  } else {
    nodo = buscar_nodo(comparador, nodo->derecha, padre, elemento);
  }
  return nodo;
}

void* arbol_buscar(abb_t* arbol, void* elemento){
  if (!arbol || !arbol->nodo_raiz)
    return NULL;

  nodo_abb_t* nodo = buscar_nodo(arbol->comparador, arbol->nodo_raiz, NULL, elemento);
  if (nodo)
    return nodo->elemento;
  return NULL;
}

/*--------------------------------------------------Borrar--------------------------------------------------*/
void liberar_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor);

/*
 * Devuelve true si el nodo que recibe es hoja o false
 * si no lo es.
 */
bool es_hoja(nodo_abb_t* nodo){
  if (!nodo->derecha && !nodo->izquierda)
    return true;
  return false;
}

/*
 * Busca en el arbol, a partir del nodo que le llega, el predecesor inorden.
 * Duevle el nodo padre del predecesor inorden, se usa para los
 * reemplazos en los borrados.
 */
nodo_abb_t* buscar_predecesor(nodo_abb_t* nodo){
  if (!nodo)
    return NULL;

  if (!nodo->derecha)
    return nodo;

  if (nodo->derecha->derecha)
    return buscar_predecesor(nodo->derecha);

  return nodo;
}

/*
 * Usando la funcion de comparacion determina si el nodo a borrar
 * es hijo derecho o hijo izquierdo de su padre, deuvuelve la
 * direccion de ese hijo. De igual manera,
 * si es raiz, devuelve la direccion del nodo_raiz.
 */
void** dirreccion_hijo(abb_t* arbol ,void* elemento_borrar, nodo_abb_t* padre){
  if (elemento_borrar == arbol->nodo_raiz->elemento)
    return (void**)&arbol->nodo_raiz;

  if (arbol->comparador(elemento_borrar, padre->elemento) == MENOR)
    return (void**)&padre->izquierda;

  return (void**)&padre->derecha;
}

/*
 * Funcion para borrar nodos que tienen a ambos hijos o solo tienen hijo/s a
 * izquierda. Se encarga de liberar la memoria.
 */
int borrado_dos_hijos_o_izquierda(nodo_abb_t* nodo_borrar, abb_liberar_elemento destructor){

  nodo_abb_t* padre_reemplazo = buscar_predecesor(nodo_borrar->izquierda); //NO LAS PUEDO DECLARAR ANTES
  nodo_abb_t* reemplazo = padre_reemplazo->derecha;
  destructor(nodo_borrar->elemento);

  if (!reemplazo) {
    nodo_borrar->elemento = padre_reemplazo->elemento;
    nodo_borrar->izquierda = padre_reemplazo->izquierda;
    liberar_nodo(padre_reemplazo, NULL);
  } else {
    nodo_borrar->elemento = reemplazo->elemento;
    padre_reemplazo->derecha = reemplazo->izquierda;
    liberar_nodo(reemplazo, NULL);
  }

  return EXITO;
}

/*
 * Funcion para borrar nodos que no tienen hijo/s a izquierda.
 * Se encarga de liberar la memoria.
 */
int borrado_sin_izquierda(void** padre, nodo_abb_t* nuevo_hijo, nodo_abb_t* nodo_borrar, abb_liberar_elemento destructor){
  *padre = nuevo_hijo;
  liberar_nodo(nodo_borrar, destructor);
  return EXITO;
}

/*
 * Funcion para borrar nodos no tienen hijo/s.
 * Se encarga de liberar la memoria.
 */
int borrado_hoja(void** direccion_nodo, nodo_abb_t* nodo_borrar, abb_liberar_elemento destructor){
  liberar_nodo(nodo_borrar, destructor);
  *direccion_nodo = NULL;
  return EXITO;
}

int arbol_borrar(abb_t* arbol, void* elemento){
  if (!arbol)
    return ERROR;

  nodo_abb_t* nodo_padre = arbol->nodo_raiz;
  nodo_abb_t* nodo_borrar = buscar_nodo(arbol->comparador, arbol->nodo_raiz, &nodo_padre, elemento);
  void** direc_hijo = dirreccion_hijo(arbol, elemento, nodo_padre);

  if (!nodo_borrar)
    return ERROR;

  if (es_hoja(nodo_borrar))
    return borrado_hoja(direc_hijo, nodo_borrar, arbol->destructor);

  if (!nodo_borrar->izquierda)
    return borrado_sin_izquierda(direc_hijo, nodo_borrar->derecha, nodo_borrar, arbol->destructor);

  return borrado_dos_hijos_o_izquierda(nodo_borrar, arbol->destructor);
}

/*-----------------------------Func. para recorridos (Con y sin iterador)----------------------------------------------*/
/*
 * Las 3 funciones reciben un nodo, un puntero a funcion y un extra. Recorren
 * el arbol en diferente orden llamando con cada elemento a la funcion que le
 * llega por parametro. Cuando las funciones que llegan por parametro devuelven true
 * o no hay mas nodos, dejan de recorrer. (Se usan tanto para el iterador como para
 * los recorridos llamados por el usuario).
 */
bool recorrido_inorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
  if (!nodo)
    return false;

  if (recorrido_inorden(nodo->izquierda, funcion, extra)) return true;
  if (funcion(nodo->elemento, extra)) return true;
  if (recorrido_inorden(nodo->derecha, funcion, extra)) return true;

  return false;
}

bool recorrido_preorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
  if (!nodo)
    return false;

  if (funcion(nodo->elemento, extra)) return true;
  if (recorrido_preorden(nodo->izquierda, funcion, extra)) return true;
  if (recorrido_preorden(nodo->derecha, funcion, extra)) return true;

  return false;
}

bool recorrido_postorden(nodo_abb_t* nodo, bool (*funcion)(void*, void*), void* extra){
  if (!nodo)
    return false;

  if (recorrido_postorden(nodo->izquierda, funcion, extra)) return true;
  if (recorrido_postorden(nodo->derecha, funcion, extra)) return true;
  if (funcion(nodo->elemento, extra)) return true;

  return false;
}

/*------------------------------------------Recorridos (Func. llamadas por usr)------------------------------------------*/

/*
 * Funcion que se pasa por parametro para los recorridos.
 * Recibe una estructura de arreglo y llena la posicion del array
 * con el elemento que recibe. Suma uno a la posicion. Si posicion es
 * igual a tamanio devuelve true y frena los recorridos.
 */
bool llenar_array(void* elemento, void* arreglo){
  int posicion = ((arreglo_t*)arreglo)->posicion;
  int tamanio = ((arreglo_t*)arreglo)->tamanio_array;

  if (posicion == tamanio)
    return true;

  ((arreglo_t*)arreglo)->array[posicion] = elemento;
  ((arreglo_t*)arreglo)->posicion++;
  return false;
}

/*
 * Crea una estructura de arreglo e inicializa los campos de dicha estructura.
 * Invoca a la funcion de recorrido corresponiente segun lo que pidio el usuario,
 * pasando por parametro la funcion "llenar_array" y como "extra" la estructura
 * de arreglo.
 */
int recorridos(abb_t* arbol, int recorrido, void** array, int tamanio_array){

  arreglo_t arreglo;
  arreglo.array = array;
  arreglo.tamanio_array = tamanio_array;
  arreglo.posicion = VACIO;

  if (recorrido == ABB_RECORRER_INORDEN) {
    recorrido_inorden(arbol->nodo_raiz, llenar_array, &arreglo);
  } else
  if (recorrido == ABB_RECORRER_PREORDEN) {
    recorrido_preorden(arbol->nodo_raiz, llenar_array, &arreglo);
  } else {
    recorrido_postorden(arbol->nodo_raiz, llenar_array, &arreglo);
  }
  return arreglo.posicion;
}

int arbol_recorrido_inorden(abb_t* arbol, void** array, int tamanio_array){
  if (!arbol || !array || tamanio_array == VACIO)
    return VACIO;

  return recorridos(arbol, ABB_RECORRER_INORDEN, array, tamanio_array);
}

int arbol_recorrido_preorden(abb_t* arbol, void** array, int tamanio_array){
  if (!arbol || !array || tamanio_array == VACIO)
    return VACIO;

  return recorridos(arbol, ABB_RECORRER_PREORDEN, array, tamanio_array);
}

int arbol_recorrido_postorden(abb_t* arbol, void** array, int tamanio_array){
  if (!arbol || !array || tamanio_array == VACIO)
    return VACIO;

  return recorridos(arbol, ABB_RECORRER_POSTORDEN, array, tamanio_array);
}

/*--------------------------------------------------Iterador Interno--------------------------------------------------*/

void abb_con_cada_elemento(abb_t* arbol, int recorrido, bool (*funcion)(void*, void*), void* extra){
  if (!arbol || !funcion)
    return;

  if (recorrido == ABB_RECORRER_INORDEN) {
    recorrido_inorden(arbol->nodo_raiz, funcion, extra);
  } else
  if (recorrido == ABB_RECORRER_PREORDEN) {
    recorrido_preorden(arbol->nodo_raiz, funcion, extra);
  } else {
    recorrido_postorden(arbol->nodo_raiz, funcion, extra);
  }
}

/*--------------------------------------------------Destruir arbol--------------------------------------------------*/
/*
 * Recibe un nodo y un destructor. Se invoca cuando se borra un nodo.
 * Llama al destructor con el elemento del nodo (si existe el elemento),
 * luego libera el nodo.
 */
void liberar_nodo(nodo_abb_t* nodo, abb_liberar_elemento destructor){
  if (destructor)
    destructor(nodo->elemento);
  free(nodo);
}

/*
 * Cuando se llama a "arbol_destruir", se invoca a esta funcion que
 * se encarga de recorrer el arbol e ir liberando los nodos y elementos
 * que quedaron insertados en el mismo.(Llamma a liberar_nodo).
 */
void destruir_nodos(nodo_abb_t* nodo, abb_liberar_elemento destructor) {
  if (nodo->izquierda)
    destruir_nodos(nodo->izquierda, destructor);

  if (nodo->derecha)
    destruir_nodos(nodo->derecha, destructor);

  liberar_nodo(nodo, destructor);
}

void arbol_destruir(abb_t* arbol){
  if (!arbol)
    return;

  if (arbol->nodo_raiz)
    destruir_nodos(arbol->nodo_raiz, arbol->destructor);

  free(arbol);
}
