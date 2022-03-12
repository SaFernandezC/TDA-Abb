#ifndef __PRUEBAS_H__
#define __PRUEBAS_H__

#include <stdio.h>
#include <stdbool.h>

typedef struct prueba prueba_t;

/*
 * Crea la estructura de las pruebas reservando la memoria necesaria.
 * Devuelve un puntero a las pruebas creadas o NULL en caso de error.
 */
prueba_t* prueba_crear();

/*
 * Sale de la funcion si el tester es NULL.
 * Muestra por pantalla la descripcion de la prueba y si el estado
 * es EXITO o FALLA, suma uno a la cantidad de pruebas totales
 * y dependiendo del estado tambien suma uno a las pruebas exitosas.
 */
void prueba_afirmar(prueba_t* tester, char* descripcion, bool estado);

/*
 * Sale de la funcion si el tester es NULL.
 * Muestra por pantalla la cantidad de pruebas exitosas sobre
 * el total de pruebas.
 */
void imprimir_estadisticas(prueba_t* tester);

/*
 * Libera la memoria reservada por las pruebas.
 */
void prueba_destruir(prueba_t* tester);

#endif /* __PRUEBAS_H__ */
