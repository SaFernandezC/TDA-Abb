## TDA Arbol Binario de Busqueda (ABB)

**Alumno:** Santiago Fernandez Caruso

**Materia:** Algoritmos y programacion II

**Catedra:** Mendez

------

El programa desarrollado para este trabajo consta de la implementacion de un Arbol Binario de Busqueda (ABB) y una serie de pruebas para corroborar el correcto funcionamiento del mismo. Se opto por una implementacion con nodos, donde cada nodo contiene un elemento y punteros a sus dos hijos.

Aclaraciones sobre la implementacion:

- Al insertar los mayores se inseran a derecha y los menores a izquierda.
- Al borrar, cuando es necesario, se busca el predecesor inorden como nodo de reemplazo (mayor de los menores).
- El arbol se encarga de liberar la memoria reservada para los elementos introducidos si se lo crea con un destructor.

------

### Pasos a seguir para compilar y ejecutar el programa 		


Para compilar el programa basta con abrir la terminal en el directorio donde se encuentran los archivos del programa y ejecutar el siguiente comando:
		

	gcc *.c -o "nombre_ejecutable"

donde "nombre_ejecutable" puede ser a elección y luego para ejecutar el programa:

	./"nombre_ejecutable"

Tambien se puede compilar pasandole Flags mas estrictos al compilador para detectar mas "errores" como por ejemplo -Wall -Werror -Wconversion, un ejemplo de compilacion y ejecucion podria ser:

	Compilacion ->	gcc *.c -Wall -Werror -Wconversion -std=c99 -o abb
	
	Ejecucion -> ./abb

Aclaracion: Se puede correr el programa con Valgrind para detectar posibles fallas con:

	valgrind ./abb o, para mas detalles:
	
	valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./abb





