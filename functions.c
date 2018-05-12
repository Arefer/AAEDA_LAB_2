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
	nuevoNodo->nombreConsultorio = (char*)malloc(sizeof(char)*(strlen(nombreConsultorio)+1));
	nuevoNodo->especialidad = (char*)malloc(sizeof(char)*(strlen(especialidad)+1));
	nuevoNodo->padre = padre;
	strcpy(nuevoNodo->nombreConsultorio, nombreConsultorio);
	strcpy(nuevoNodo->especialidad, especialidad);
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
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, nombreNodo) == 0){
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
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, consultorioOrigen) == 0){
			printf("Agregando adyacente a nodo '%s', ubicado en la posicion |%d|...\n", g->matrizAdyacencia[i]->origen->nombreConsultorio, i);
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
	char* buffer = (char*)malloc(sizeof(char)*100);
	char* valor;
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
	fscanf(archivo, "%s", buffer);
	g->numNodos = atoi(buffer);
	printf("Num nodos: %d\n", g->numNodos);
	g->matrizAdyacencia = (ListaAdyacencia**)malloc(sizeof(ListaAdyacencia*)*(g->numNodos));
	// Leer el salto de linea pendiente
	fgets(buffer, sizeof(buffer), archivo);
	while (i < g->numNodos){
		g->matrizAdyacencia[i] = (ListaAdyacencia*)malloc(sizeof(ListaAdyacencia));
		g->matrizAdyacencia[i]->inicio = NULL;
		g->matrizAdyacencia[i]->final = NULL;
		g->matrizAdyacencia[i]->numNodosAdyacentes = 0;
		// Leer caracteres mientras no sea salto de linea
		fscanf(archivo, "%99[^\n]", buffer);
		// Leer el salto de línea pendiente
		if (i != g->numNodos - 1){
			fgetc(archivo);
		}
		valor = strtok(buffer, " ");
		strcpy(nombreConsultorio, valor);
		printf("Nombre consultorio: %s ", nombreConsultorio);
		valor = strtok(NULL, " ");
		strcpy(especialidad, valor);
		printf("Especialidad: %s ", especialidad);
		valor = strtok(NULL, " ");
		pacientesMaximos = atoi(valor);
		printf("Pacientes maximos: %d ", pacientesMaximos);
		valor = strtok(NULL, " ");
		pacientesActuales = atoi(valor);
		printf("Pacientes actuales: %d\n", pacientesActuales);
		Nodo* nodoLeido = crearNodo(NULL, nombreConsultorio, especialidad,
			pacientesMaximos, pacientesActuales, 0);
		g->matrizAdyacencia[i]->origen = nodoLeido;
		//printf("Ciclo %d completado\n", i);
		i++;
	}
	printf("Lectura de nodos completada\n");
	fclose(archivo);
	FILE* archivoAdj = fopen(pathAristas, "rb");
	if (archivoAdj == NULL){
		printf("No se pudo abrir el archivo de consultorios adyacentes\n");
		exit(-1);
	}
	fscanf(archivoAdj, "%s", valor);
	numAdyacentes = atoi(valor);
	fgets(valor, sizeof(valor), archivoAdj);
	i = 0;
	while (i < numAdyacentes){
		fscanf(archivo, "%99[^\n]", buffer);
		if (i != numAdyacentes - 1){
			fgetc(archivoAdj);
		}
		valor = strtok(buffer, " ");
		strcpy(nombreConsultorio, valor);
		printf("Nombre consultorio: %s ", nombreConsultorio);
		valor = strtok(NULL, " ");
		strcpy(consultorioAdyacente, valor);
		printf("Consultorio adyacente: %s ", consultorioAdyacente);
		valor = strtok(NULL, " ");
		tiempoTrayecto = atoi(valor);
		printf("Tiempo: %d\n", tiempoTrayecto);
		agregarAdyacente(g, nombreConsultorio, consultorioAdyacente, tiempoTrayecto);
		i++;
	}
	fclose(archivoAdj);
	free(buffer);
	free(valor);
	free(nombreConsultorio);
	free(especialidad);
	free(consultorioAdyacente);
	return g;
}
/*
 * Imprime la lista de adyacencia de un nodo con el formato nodoOrigen consultorioAdyaycente (tiempo) ; ...
 * Entrada: ListaAdyacencia* lista -> la lista a imprimir.
 * Salida: void.
*/
void imprimirAdyacentes(ListaAdyacencia* lista){
	printf("Origen: %s ", lista->origen->nombreConsultorio);
	NodoAdyacente* cursor = lista->inicio;
	for (int i = 0; i < lista->numNodosAdyacentes; i++){
		printf("%s (%d) ; ", cursor->consultorio->nombreConsultorio, cursor->tiempo);
		cursor = cursor->siguiente;
	}
}

/*
 * Imprime el grafo completo (lista de listas adyacentes).
 * Entrada: Grafo* g -> grafo a imprimir.
 * Salida: void.
*/
void imprimirGrafo(Grafo* g){
	printf("#################################");
	printf("####### IMPRIMIENDO GRAFO #######");
	printf("#################################\n\n");
	for (int i = 0; i < g->numNodos; i++){
		imprimirAdyacentes(g->matrizAdyacencia[i]);
		printf("\n");
	}
}
