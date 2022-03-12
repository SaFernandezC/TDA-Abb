#include <stdio.h>
#include <stdlib.h>
#include "pruebas.h"

#define COLOR_ROJO     "\x1b[31m\x1b[1m"
#define COLOR_VERDE    "\x1b[32m\x1b[1m"
#define COLOR_NORMAL   "\x1b[0m"

struct prueba{
  int pruebas_exitosas;
  int total_pruebas;
};

prueba_t* prueba_crear(){
  return calloc(1, sizeof(prueba_t));
}

void prueba_afirmar(prueba_t* tester, char* descripcion, bool estado){
  if (!tester){
    return;
  }

  if (estado){
    tester->pruebas_exitosas++;
  }

  tester->total_pruebas++;
  printf("PRUEBA: %s -> %s\n", descripcion, estado?COLOR_VERDE"EXITO"COLOR_NORMAL:COLOR_ROJO"FALLO"COLOR_NORMAL);
}

void imprimir_estadisticas(prueba_t* tester){
  if (!tester){
    return;
  }
  printf("\n\n\t\t\t\t\t========================\n");
  printf("\t\t\t\t\t%i/%i Pruebas exitosas\n", tester->pruebas_exitosas, tester->total_pruebas);
  printf("\t\t\t\t\t========================\n");
}

void prueba_destruir(prueba_t* tester){
  free(tester);
}
