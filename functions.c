#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "functions.h"
#include "structs.h"

/*
 * Crea un nodo que representa a un consultorio.
 * Entradas: Nodo* padre -> padre del nodo a crear.
 * 			 char* nombreConsultorio -> nombre del consultorio que representará el nodo.
 * 			 char* especialidad -> especialidad del consultorio que representará el nodo.
 * 			 int pacientesMaximos -> capacidad máxima de pacientes que puede atender el consultorio.
 * 			 int tiempoAcumulado -> tiempo empleado en la búsqueda de un camino más corto.
 * Salida: Nodo* -> Un puntero al nodo creado.
*/
Nodo* crearNodo(Nodo* padre, char* nombreConsultorio, char* especialidad,
	int pacientesMaximos, int pacientesActuales, int tiempoAcumulado){
	Nodo* nuevoNodo = (Nodo*)malloc(sizeof(Nodo));
	nuevoNodo->padre = padre;
	nuevoNodo->nombreConsultorio = nombreConsultorio;
	nuevoNodo->especialidad = especialidad;
	nuevoNodo->pacientesMaximos = pacientesMaximos;
	nuevoNodo->pacientesActuales = pacientesActuales;
	nuevoNodo->tiempoAcumulado = tiempoAcumulado;
	return nuevoNodo;
}

/*
 * Busca un nodo dentro de la matriz de adyacencia del grafo de la red de consultorios.
 * Entradas: char* nombreNodo -> nombre del consultorio a buscar.
 * 			 Grafo* g -> grafo de la red de consultorios.
 * Salida: Nodo* -> puntero al nodo encontrado.
*/
Nodo* buscarNodo(char* nombreNodo, Grafo* g){
	for (int i = 0; i < g->numNodos; i++){
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, nombreNodo) == 1){
			return g->matrizAdyacencia[i]->origen;
		}
	}
	return NULL;
}
/*
 * Agrega un consultorio a la lista de adyacentes de un consultorio origen.
 * Entradas: Grafo* g -> grafo que representa la red de consultorios.
 * 			 char* consultorioOrigen -> nombre del consultorio origen de la lista adyacente.
 * 			 char* consultorioAdyacente -> nombre del consultorio a agregar.
 * 			 int tiempoTrayecto -> tiempo de trayecto entre los consultorios.
 * Salida: void.
*/
void agregarAdyacente(Grafo* g, char* consultorioOrigen, char* consultorioAdyacente, int tiempoTrayecto){
	for (int i = 0; i < g->numNodos; i++){
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, consultorioOrigen) == 1){
			NodoAdyacente* adj = (NodoAdyacente*)malloc(sizeof(NodoAdyacente));
			adj->tiempo = tiempoTrayecto;
			adj->consultorio = buscarNodo(consultorioAdyacente, g);
			adj->siguiente = NULL;
			if (g->matrizAdyacencia[i]->inicio == NULL){
				g->matrizAdyacencia[i]->inicio = adj;
				g->matrizAdyacencia[i]->final = adj;
				g->matrizAdyacencia[i]->numNodosAdyacentes = 1;
			}
			else{
				g->matrizAdyacencia[i]->final->siguiente = adj;
				g->matrizAdyacencia[i]->final = adj;
				g->matrizAdyacencia[i]->numNodosAdyacentes += 1;
			}
		}
	}
}
/*
 * Lee y almacena un grafo que representa la red de consultorios desde dos archivos de texto.
 * Entradas: char* path -> nombre del archivo que posee la lista de consultorios.
 * 			 char* pathAristas -> nombre del archivo que posee las conexiones entre consultorios.
 * Saluda: Grafo* -> un puntero al grafo creado a través de la lectura.
*/
Grafo* leerGrafo(char* path, char* pathAristas){
	Grafo* g = (Grafo*)malloc(sizeof(Grafo));
	Nodo* nodoLeido;
	char* linea = (char*)malloc(sizeof(char)*100);
	char* nombreConsultorio = (char*)malloc(sizeof(char)*30);
	char* especialidad = (char*)malloc(sizeof(char)*30);
	int pacientesMaximos;
	int	pacientesActuales;
	char* consultorioAdyacente = (char*)malloc(sizeof(char)*30);
	int tiempoTrayecto;
	int numAdyacentes;
	int i = 0;

	FILE* archivo = fopen(path, "rb");
	if (archivo ==  NULL){
		printf("No se pudo abrir el archivo de consultorios\n");
		exit(-1);
	}
	fscanf(archivo, "%d", &g->numNodos);
	g->matrizAdyacencia = (ListaAdyacencia**)malloc(sizeof(ListaAdyacencia*)*g->numNodos);
	// Leer el salto de linea pendiente
	fgetc(archivo);
	while (i < g->numNodos){
		g->matrizAdyacencia[i] = (ListaAdyacencia*)malloc(sizeof(ListaAdyacencia));
		g->matrizAdyacencia[i]->inicio = NULL;
		g->matrizAdyacencia[i]->final = NULL;
		g->matrizAdyacencia[i]->numNodosAdyacentes = 0;
		// Leer caracteres mientras no sea salto de linea
		fscanf(archivo, "%99[^\n]", linea);
		// Leer el salto de linea pendiente
		if (i != g->numNodos - 1){
			fgetc(archivo);
		}
		linea = strtok(linea, " ");
		strcpy(nombreConsultorio, linea);
		linea = strtok(NULL, " ");
		strcpy(especialidad, linea);
		linea = strtok(NULL, " ");
		pacientesMaximos = atoi(linea);
		linea = strtok(NULL, " ");
		pacientesActuales = atoi(linea);
		nodoLeido = crearNodo(NULL, nombreConsultorio, especialidad,
			pacientesMaximos, pacientesActuales, 0);
		g->matrizAdyacencia[i]->origen = nodoLeido;
		i++;
	}
	fclose(archivo);
	FILE* archivoAdj = fopen(pathAristas, "rb");
	if (archivoAdj == NULL){
		printf("No se pudo abrir el archivo de consultorios adyacentes\n");
		exit(-1);
	}
	fscanf(archivoAdj, "%d", &numAdyacentes);
	i = 0;
	while (i < numAdyacentes){
		fscanf(archivo, "%99[^\n]", linea);
		if (i != numAdyacentes - 1){
			fgetc(archivoAdj);
		}
		linea = strtok(linea, " ");
		strcpy(nombreConsultorio, linea);
		linea = strtok(NULL, " ");
		strcpy(consultorioAdyacente, linea);
		linea = strtok(NULL, " ");
		tiempoTrayecto = atoi(linea);
		agregarAdyacente(g, nombreConsultorio, consultorioAdyacente, tiempoTrayecto);
		i++;
	}
	fclose(archivoAdj);
	free(linea);
	free(nombreConsultorio);
	free(especialidad);
	free(consultorioAdyacente);
	return g;
}