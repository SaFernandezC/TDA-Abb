#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "abb.h"
#include "pruebas.h"

#define ERROR -1
#define EXITO 0
#define VACIO 0

typedef struct alumno {
	int edad;
} alumno_t;

typedef struct arreglo {
	void** array;
	int	posicion;
} arreglo_t;

/*------------------------------------------FUNCIONES DE "USUARIO"------------------------------------------*/
alumno_t* crear_alumno(int edad){
	alumno_t* alumno = calloc(1,sizeof(alumno_t));
	if (alumno) {
		alumno->edad = edad;
	}
	return alumno;
}

int contar_nodos(nodo_abb_t* nodo, int contador){
  if (!nodo) {
    return contador;
  }

  contador =  contar_nodos(nodo->izquierda, contador);
  contador++;
  contador= contar_nodos(nodo->derecha, contador);

  return contador;
}

bool mostrar_elemento(void* elemento, void* extra){

	if(elemento){
		printf("%i ", ((alumno_t*)elemento)->edad);
		return false;
	}

	return true;
}

bool recorrido(void* elemento, void* extra){

	if(elemento){
		*(int*)extra += 1;
		return false;
	}
	return true;
}

bool array_hasta_30(void* elemento, void* arreglo){

	if(!elemento){
		return true;
	}

	int posicion = ((arreglo_t*)arreglo)->posicion;
	((arreglo_t*)arreglo)->array[posicion]= elemento;

	if (((alumno_t*)elemento)->edad == 30) {
		return true;
	}

	((arreglo_t*)arreglo)->posicion++;
	return false;
}

void destructor_de_alumnos(void* elemento){
	if(!elemento)
		return;
  free(elemento);
}

int comprar_por_edad(void* dato1, void* dato2){
	if (!dato1 || !dato2) {
		return 0;
	}
  int primero = ((alumno_t*)dato1)->edad;
  int segundo = ((alumno_t*)dato2)->edad;
  if (primero == segundo) {
    return 0;
  } else if (primero < segundo) {
    return -1;
  } else {
    return 1;
  }
}

/*------------------------------------------FUNCIONES DE PRUEBAS------------------------------------------*/
void pruebas_crear_arbol(prueba_t* tester) {
	abb_t* arbol = arbol_crear(comprar_por_edad, NULL);
	prueba_afirmar(tester, "Creo un arbol correctamente", arbol);
	free(arbol);

	abb_t* arbol1 = arbol_crear(comprar_por_edad, NULL);
	prueba_afirmar(tester, "Creo un arbol con destructor NULL correctamente", arbol1);
	free(arbol1);

	abb_t* arbol2 = arbol_crear(NULL, NULL);
	prueba_afirmar(tester, "Creo arbol con comparador NULL y falla", !arbol2);
	free(arbol2);
}

void pruebas_funcionamiento(prueba_t* tester) {

	{
		char x = 'x';
		prueba_afirmar(tester, "Insertar en arbol NULL devuelve error", arbol_insertar(NULL,&x) == ERROR);
		prueba_afirmar(tester, "Borrar en arbol NULL devuelve error", arbol_borrar(NULL,&x) == ERROR);
		prueba_afirmar(tester, "Buscar en arbol NULL devuelve error", arbol_buscar(NULL,&x) == NULL);
		prueba_afirmar(tester, "Arbol_raiz con arbol NULL devuelve error", arbol_raiz(NULL) == NULL);
		prueba_afirmar(tester, "Arbol_vacio con arbol NULL devuelve true", arbol_vacio(NULL) == true);
	}

	{
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		void* array[1];
		alumno_t* a1 = crear_alumno(20);
		alumno_t* a2 = crear_alumno(54);

		prueba_afirmar(tester, "Arbol_raiz devuelve NULL con arbol vacio", arbol_raiz(arbol) == NULL);
		prueba_afirmar(tester, "Busco un elemento con arbol vacio devuelve NULL", arbol_buscar(arbol, NULL) == NULL);
		prueba_afirmar(tester, "Arbol_vacio con arbol vacio devuelve true", arbol_vacio(arbol) == true);
		arbol_insertar(arbol, a1);
		prueba_afirmar(tester, "Busco un elemento que no existe y devuelve NULL", arbol_buscar(arbol, a2) == NULL);
		prueba_afirmar(tester, "Inserto elemento NULL en arbol valido", arbol_insertar(arbol, NULL) == EXITO);
		prueba_afirmar(tester, "Recorrido Inorden devuelve vacio con arbol NULL", arbol_recorrido_inorden(NULL, array,0) == VACIO);
		prueba_afirmar(tester, "Recorrido Inorden devuelve vacio con array NULL", arbol_recorrido_inorden(arbol, NULL,0) == VACIO);
		prueba_afirmar(tester, "Recorrido Preorden devuelve vacio con arbol NULL", arbol_recorrido_preorden(NULL, array,0) == VACIO);
		prueba_afirmar(tester, "Recorrido Preorden devuelve vacio con array NULL", arbol_recorrido_preorden(arbol,NULL,0) == VACIO);
		prueba_afirmar(tester, "Recorrido Postorden devuelve vacio con arbol NULL", arbol_recorrido_postorden(NULL, array,0) == VACIO);
		prueba_afirmar(tester, "Recorrido Postorden devuelve vacio con array NULL", arbol_recorrido_postorden(arbol, NULL,0) == VACIO);

		free(a2);
		arbol_destruir(arbol);
	}
}

void pruebas_insertar(prueba_t* tester){
	{
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		prueba_afirmar(tester, "Creo un arbol correctamente", arbol);
		alumno_t* a1 = crear_alumno(20);
		alumno_t* a2 = crear_alumno(30);
		alumno_t* a3 = crear_alumno(15);
		alumno_t* a4 = crear_alumno(20);

		prueba_afirmar(tester, "El arbol se crea vacio", arbol_vacio(arbol) == true);
		prueba_afirmar(tester, "Inserto un elemento (20)", arbol_insertar(arbol,a1) == EXITO);
		prueba_afirmar(tester, "El arbol deja de estar vacio", arbol_vacio(arbol) == false);
		prueba_afirmar(tester, "El elemento insertado se posiciona como raiz", arbol->nodo_raiz->elemento == a1);
		prueba_afirmar(tester, "Inserto un elemento menor (15)", arbol_insertar(arbol,a3) == EXITO);
		prueba_afirmar(tester, "El elemento insertado se posiciona a la izq. de raiz", arbol->nodo_raiz->izquierda->elemento == a3);
		prueba_afirmar(tester, "Inserto un elemento mayor (30)", arbol_insertar(arbol,a2) == EXITO);
		prueba_afirmar(tester, "El elemento insertado se posiciona a la der. de raiz", arbol->nodo_raiz->derecha->elemento == a2);
		prueba_afirmar(tester, "Cantidad de elementos es 3", contar_nodos(arbol->nodo_raiz,0) == 3);
		prueba_afirmar(tester, "Inserto un elemento repetido (20)", arbol_insertar(arbol,a4) == EXITO);
		prueba_afirmar(tester, "El elemento repetido se inserta a la derecha de su igual", arbol->nodo_raiz->derecha->izquierda->elemento == a4);
		prueba_afirmar(tester, "Cantidad de elementos es 4", contar_nodos(arbol->nodo_raiz,0) == 4);
		prueba_afirmar(tester, "No se modifico la raiz en todo el proceso", arbol->nodo_raiz->elemento == a1);

		arbol_destruir(arbol);
	}
}

void pruebas_borrar(prueba_t* tester) {
	{
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
		alumno_t* a2 = crear_alumno(30); arbol_insertar(arbol,a2);
		alumno_t* a3 = crear_alumno(15); arbol_insertar(arbol,a3);
		alumno_t* a4 = crear_alumno(14); arbol_insertar(arbol,a4);
		alumno_t* a5 = crear_alumno(16); arbol_insertar(arbol,a5);
		alumno_t* a6 = crear_alumno(35); arbol_insertar(arbol,a6);
		alumno_t* a7 = crear_alumno(24); arbol_insertar(arbol,a7);


		prueba_afirmar(tester, "Inserto 7 elementos (20,30,15,14,16,35,24)", contar_nodos(arbol->nodo_raiz,0) == 7);
		prueba_afirmar(tester, "Borro elemento con dos hijos (30)", arbol_borrar(arbol, a2) == EXITO);
		prueba_afirmar(tester,"Elemento (24) se posiciona a la derecha de raiz", arbol_buscar(arbol, a7) == arbol->nodo_raiz->derecha->elemento);
		prueba_afirmar(tester, "Borro un elemento hoja (35)", arbol_borrar(arbol, a6) == EXITO);
		prueba_afirmar(tester, "El padre del nodo borrado apunta a NULL", arbol->nodo_raiz->derecha->derecha == NULL);
		prueba_afirmar(tester, "Borro nodo con un solo hijo a izquierda (24)", arbol_borrar(arbol, a7) == EXITO);
		prueba_afirmar(tester, "Borro nodo con un solo hijo a derecha (20)", arbol_borrar(arbol, a1) == EXITO);
		prueba_afirmar(tester,"La cantidad de elementos es 3", contar_nodos(arbol->nodo_raiz,0) == 3);
		arbol_destruir(arbol);
		prueba_afirmar(tester, "Destruyo el arbol",true);
	}

	{
		printf("\nBorrado sobre la raiz\n");
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
		alumno_t* a2 = crear_alumno(30); arbol_insertar(arbol,a2);
		alumno_t* a3 = crear_alumno(35); arbol_insertar(arbol,a3);

		prueba_afirmar(tester, "Inserto 3 elementos, todos a la derecha de raiz (20,30,35)", contar_nodos(arbol->nodo_raiz,0) == 3);
		prueba_afirmar(tester, "Borro raiz sin hijo a izquierda", arbol_borrar(arbol, a1) == EXITO);
		prueba_afirmar(tester, "Elemento raiz es el hijo derecho del borrado (30)", arbol_raiz(arbol) == a2);
		prueba_afirmar(tester,"La cantidad de elementos es 2", contar_nodos(arbol->nodo_raiz,0) == 2);

		arbol_destruir(arbol);
	}

	{
		printf("\n");
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
		alumno_t* a2 = crear_alumno(15); arbol_insertar(arbol,a2);
		alumno_t* a3 = crear_alumno(18); arbol_insertar(arbol,a3);
		alumno_t* a4 = crear_alumno(16); arbol_insertar(arbol,a4);

		prueba_afirmar(tester, "Inserto 3 elementos, todos a la izquierda de raiz (20,15,18,16)", contar_nodos(arbol->nodo_raiz,0) == 4);
		prueba_afirmar(tester, "Borro raiz sin hijo a derecha", arbol_borrar(arbol, a1) == EXITO);
		prueba_afirmar(tester, "Elemento raiz es el predecesor in orden (18)", arbol_raiz(arbol) == a3);
		prueba_afirmar(tester, "En el lugar del (18) queda el (16)", arbol->nodo_raiz->izquierda->derecha->elemento == a4);
		prueba_afirmar(tester,"La cantidad de elementos es 3", contar_nodos(arbol->nodo_raiz,0) == 3);
		arbol_destruir(arbol);
	}

	{
		printf("\n");
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		alumno_t* a1 = crear_alumno(20);
		prueba_afirmar(tester, "Inserto 1 elemento", arbol_insertar(arbol,a1) == EXITO);
		prueba_afirmar(tester, "Borro raiz sin hijos", arbol_borrar(arbol, a1) == EXITO);
		prueba_afirmar(tester, "Nodo raiz apunta a null", arbol->nodo_raiz == NULL);
		prueba_afirmar(tester,"El arbol queda vacio", arbol_vacio(arbol) == true);

		arbol_destruir(arbol);
	}
}

void pruebas_busqueda(prueba_t* tester) {
	{
		abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
		alumno_t* a1 = crear_alumno(4); arbol_insertar(arbol,a1);
		alumno_t* a2 = crear_alumno(6); arbol_insertar(arbol,a2);
		alumno_t* a3 = crear_alumno(7); arbol_insertar(arbol,a3);
		alumno_t* a4 = crear_alumno(5); arbol_insertar(arbol,a4);
		alumno_t* a5 = crear_alumno(8); arbol_insertar(arbol,a5);

		prueba_afirmar(tester, "Inserto 5 elementos (4,6,7,5,8)", contar_nodos(arbol->nodo_raiz, 0) == 5);
		prueba_afirmar(tester, "Busco un nodo hoja (5)", arbol_buscar(arbol,a4) == a4);
		prueba_afirmar(tester, "Busco un nodo con un solo hijo (7)", arbol_buscar(arbol,a3) == a3);
		prueba_afirmar(tester, "Busco un nodo con dos hijos (6)", arbol_buscar(arbol,a2) == a2);
		prueba_afirmar(tester, "Busco el nodo raiz (4)", arbol_buscar(arbol,a1) == a1);
		arbol_destruir(arbol);
	}
}

void pruebas_recorridos(prueba_t* tester) {

	abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
	alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
	alumno_t* a2 = crear_alumno(30); arbol_insertar(arbol,a2);
	alumno_t* a3 = crear_alumno(15); arbol_insertar(arbol,a3);
	alumno_t* a4 = crear_alumno(14); arbol_insertar(arbol,a4);
	alumno_t* a5 = crear_alumno(16); arbol_insertar(arbol,a5);
	alumno_t* a6 = crear_alumno(35); arbol_insertar(arbol,a6);
	alumno_t* a7 = crear_alumno(24); arbol_insertar(arbol,a7);

	prueba_afirmar(tester, "Inserto 7 elementos", contar_nodos(arbol->nodo_raiz, 0) == 7);

	void* array_inorden[7] = {a4,a3,a5,a1,a7,a2,a6};
	void* array_preoden[7] = {a1,a3,a4,a5,a2,a7,a6};
	void* array_postorden[7] = {a4,a5,a3,a7,a6,a2,a1};
	void* array[7];
	void* array2[4];
	bool hay_errores = false;
	void* array3[15];
	for (int i = 0; i < 15; i++) {
		array3[i] = NULL;
	}

	prueba_afirmar(tester, "Recorrido inorden con tamanio 0 devuelve vacio", arbol_recorrido_inorden(arbol,array,0) == VACIO);
	prueba_afirmar(tester, "Recorrido preorden con tamanio 0 devuelve vacio", arbol_recorrido_preorden(arbol,array,0) == VACIO);
	prueba_afirmar(tester, "Recorrido postorden con tamanio 0 devuelve vacio", arbol_recorrido_postorden(arbol,array,0) == VACIO);

  {
		arbol_recorrido_inorden(arbol, array, 7);
		for (int i = 0; i < 7 && !hay_errores; i++) {
			if (array[i] != array_inorden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo inorden sin errores", hay_errores == false);
	}
	{
		hay_errores = false;
		arbol_recorrido_preorden(arbol, array, 7);
		for (int i = 0; i < 7 && !hay_errores; i++) {
			if (array[i] != array_preoden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo preorden sin errores", hay_errores == false);
	}
	{
		hay_errores = false;
		arbol_recorrido_postorden(arbol, array, 7);
		for (int i = 0; i < 7 && !hay_errores; i++) {
			if (array[i] != array_postorden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo postorden sin errores", hay_errores == false);
	}
	{
		hay_errores = false;
		arbol_recorrido_inorden(arbol, array2, 4);
		for (int i = 0; i < 4 && !hay_errores; i++) {
			if (array2[i] != array_inorden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo inorden con array de menor tamanio sin errores", hay_errores == false);

		arbol_recorrido_inorden(arbol, array3, 15);
		prueba_afirmar(tester, "Recorro el arbol completo inorden con array de mayor tamanio y solo se llenan 7 posiciones", (array3[7] == NULL && array3[6] != NULL));

	}
	{
		hay_errores = false;
		arbol_recorrido_preorden(arbol, array2, 4);
		for (int i = 0; i < 4 && !hay_errores; i++) {
			if (array2[i] != array_preoden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo preorden con array de menor tamanio sin errores", hay_errores == false);

		arbol_recorrido_preorden(arbol, array3, 15);
		prueba_afirmar(tester, "Recorro el arbol completo preorden con array de mayor tamanio y solo se llenan 7 posiciones", (array3[7] == NULL && array3[6] != NULL));
	}
	{
		hay_errores = false;
		arbol_recorrido_postorden(arbol, array2, 4);
		for (int i = 0; i < 4 && !hay_errores; i++) {
			if (array2[i] != array_postorden[i])
				hay_errores = true;
		}
		prueba_afirmar(tester, "Recorro el arbol completo postorden con array de menor tamanio sin errores", hay_errores == false);

		arbol_recorrido_postorden(arbol, array3, 15);
		prueba_afirmar(tester, "Recorro el arbol completo postorden con array de mayor tamanio y solo se llenan 7 posiciones", (array3[7] == NULL && array3[6] != NULL));
	}

	arbol_destruir(arbol);
}

void pruebas_iterador(prueba_t* tester) {

	abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
	alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
	alumno_t* a2 = crear_alumno(30); arbol_insertar(arbol,a2);
	alumno_t* a3 = crear_alumno(15); arbol_insertar(arbol,a3);
	alumno_t* a4 = crear_alumno(14); arbol_insertar(arbol,a4);
	alumno_t* a5 = crear_alumno(16); arbol_insertar(arbol,a5);
	alumno_t* a6 = crear_alumno(35); arbol_insertar(arbol,a6);
	alumno_t* a7 = crear_alumno(24); arbol_insertar(arbol,a7);

	{
		prueba_afirmar(tester, "Inserto 7 elementos", contar_nodos(arbol->nodo_raiz,0)==7);
		int contador = 0;
		abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN,recorrido, &contador);
		prueba_afirmar(tester, "Recorrido inorden con iterador recorre todos los elementos", contador == 7);

		contador = 0;
		abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN,recorrido, &contador);
		prueba_afirmar(tester, "Recorrido preorden con iterador recorre todos los elementos", contador == 7);

		contador = 0;
		abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN,recorrido, &contador);
		prueba_afirmar(tester, "Recorrido postorden con iterador recorre todos los elementos", contador == 7);
	}

	{
		void* array[7];
		arreglo_t arreglo;
		arreglo.array = array;
		arreglo.posicion = 0;

		abb_con_cada_elemento(arbol, ABB_RECORRER_INORDEN, array_hasta_30, &arreglo);
		prueba_afirmar(tester, "Recorro inorden hasta cierto elemento", arreglo.array[arreglo.posicion] == a2);

		arreglo.posicion = 0;
		abb_con_cada_elemento(arbol, ABB_RECORRER_PREORDEN, array_hasta_30, &arreglo);
		prueba_afirmar(tester, "Recorro preorden hasta cierto elemento", arreglo.array[arreglo.posicion] == a2);

		arreglo.posicion = 0;
		abb_con_cada_elemento(arbol, ABB_RECORRER_POSTORDEN, array_hasta_30, &arreglo);
		prueba_afirmar(tester, "Recorro postorden hasta cierto elemento", arreglo.array[arreglo.posicion] == a2);
	}
	arbol_destruir(arbol);
}

void pruebas_conjunto(prueba_t* tester) {
	abb_t* arbol = arbol_crear(comprar_por_edad,destructor_de_alumnos);
	alumno_t* a1 = crear_alumno(20); arbol_insertar(arbol,a1);
	alumno_t* a2 = crear_alumno(30); arbol_insertar(arbol,a2);
	alumno_t* a3 = crear_alumno(15); arbol_insertar(arbol,a3);
	alumno_t* a4 = crear_alumno(14); arbol_insertar(arbol,a4);
	alumno_t* a5 = crear_alumno(16); arbol_insertar(arbol,a5);
	alumno_t* a6 = crear_alumno(35); arbol_insertar(arbol,a6);
	alumno_t* a7 = crear_alumno(24); arbol_insertar(arbol,a7);
	alumno_t* aux = crear_alumno(20);

	prueba_afirmar(tester,"Inserto 7 elementos (20,30,15,14,16,35,24)", contar_nodos(arbol->nodo_raiz,0)== 7);
	prueba_afirmar(tester,"Nodo raiz es 20", arbol_raiz(arbol) == a1);
	prueba_afirmar(tester, "Elimino la raiz", arbol_borrar(arbol, a1) == EXITO);
	prueba_afirmar(tester, "El arbol queda con 6 elementos", contar_nodos(arbol->nodo_raiz, 0) == 6);
	prueba_afirmar(tester, "Queda como raiz el predecesor inorden (16)", arbol_raiz(arbol) == a5);
	prueba_afirmar(tester, "Borro otro nodo con dos hijos (30)", arbol_borrar(arbol, a2) == EXITO);
	prueba_afirmar(tester, "El arbol queda con 5 elementos", contar_nodos(arbol->nodo_raiz, 0) == 5);
	prueba_afirmar(tester, "Arbol vacio devuelve false", arbol_vacio(arbol) == false);
	prueba_afirmar(tester, "Busco un elemento que no existe y obtengo NULL", arbol_buscar(arbol, aux) == NULL);

	bool hay_errores = false;
	void* array_inorden[5] = {a4,a3,a5,a7,a6};
	void* array[5];

	arbol_recorrido_inorden(arbol, array, 5);
	for (int i = 0; i < 5 && !hay_errores; i++) {
		if(array[i] != array_inorden[i]) hay_errores = true;
	}

	prueba_afirmar(tester,"Recorro inorden y obtengo los valores correctos (14,15,16,24,35)", !hay_errores);
	prueba_afirmar(tester,"Borro el elemento (15)", arbol_borrar(arbol, a3) == EXITO);
	prueba_afirmar(tester,"Borro el elemento (16)", arbol_borrar(arbol, a5) == EXITO);
	prueba_afirmar(tester, "El arbol queda con 3 elementos", contar_nodos(arbol->nodo_raiz, 0) == 3);

	hay_errores = false;
	void* array_inorden2[3] = {a4,a7,a6};

	arbol_recorrido_inorden(arbol, array, 5);
	for (int i = 0; i < 3 && !hay_errores; i++) {
		if(array[i] != array_inorden2[i]) hay_errores = true;
	}

	prueba_afirmar(tester,"Recorro inorden y obtengo los valores correctos (14,24,35)", !hay_errores);
	prueba_afirmar(tester,"Borro el elemento (14)", arbol_borrar(arbol, a4) == EXITO);
	prueba_afirmar(tester,"Borro el elemento (25)", arbol_borrar(arbol, a7) == EXITO);
	prueba_afirmar(tester,"Borro el elemento (35)", arbol_borrar(arbol, a6) == EXITO);
	prueba_afirmar(tester,"El arbol queda vacio", arbol_vacio(arbol) == true);
	prueba_afirmar(tester,"La raiz es NULL", arbol_raiz(arbol) == NULL);
	arbol_destruir(arbol);
	prueba_afirmar(tester,"Destruyo el arbol", true);

	free(aux);
}

/*------------------------------------------MAIN------------------------------------------*/

int main() {

	prueba_t* tester = prueba_crear();

	printf("\n\n|------------------------------------Pruebas crear------------------------------------|\n\n");
	pruebas_crear_arbol(tester);

	printf("\n\n|------------------------------------Pruebas funcionamiento------------------------------------|\n\n");
	pruebas_funcionamiento(tester);

	printf("\n\n|------------------------------------Pruebas incercion------------------------------------|\n\n");
	pruebas_insertar(tester);

	printf("\n\n|------------------------------------Pruebas borrado------------------------------------|\n\n");
	pruebas_borrar(tester);

	printf("\n\n|------------------------------------Pruebas busqueda------------------------------------|\n\n");
	pruebas_busqueda(tester);

	printf("\n\n|------------------------------------Pruebas recorridos------------------------------------|\n\n");
	pruebas_recorridos(tester);

	printf("\n\n|------------------------------------Pruebas iterador interno------------------------------------|\n\n");
	pruebas_iterador(tester);

	printf("\n\n|------------------------------------Pruebas en conjunto------------------------------------|\n\n");
	pruebas_conjunto(tester);


	imprimir_estadisticas(tester);
	free(tester);
  return 0;
}
