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
	printf("############# LEYENDO GRAFO #############\n\n");
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
		valor = strtok(NULL, " ");
		strcpy(consultorioAdyacente, valor);
		valor = strtok(NULL, " ");
		tiempoTrayecto = atoi(valor);
		agregarAdyacente(g, nombreConsultorio, consultorioAdyacente, tiempoTrayecto);
		i++;
	}
	fclose(archivoAdj);
	free(buffer);
	free(valor);
	free(nombreConsultorio);
	free(especialidad);
	free(consultorioAdyacente);
	printf("\n############# LECTURA DEL GRAFO COMPLETADA #############\n\n");
	return g;
}
/*
 * Imprime la lista de adyacencia de un nodo con el formato nodoOrigen consultorioAdyaycente (tiempo) ; ...
 * Entrada: ListaAdyacencia* lista -> la lista a imprimir.
 * Salida: void.
*/
void imprimirAdyacentes(ListaAdyacencia* lista){
	printf("Origen: %s -> ", lista->origen->nombreConsultorio);
	NodoAdyacente* cursor = lista->inicio;
	for (int i = 0; i < lista->numNodosAdyacentes; i++){
		printf("%s (%d)", cursor->consultorio->nombreConsultorio, cursor->tiempo);
		if (i != lista->numNodosAdyacentes - 1) printf(" ; ");
		cursor = cursor->siguiente;
	}
}

/*
 * Imprime el grafo completo (lista de listas adyacentes).
 * Entrada: Grafo* g -> grafo a imprimir.
 * Salida: void.
*/
void imprimirGrafo(Grafo* g){
	printf("#################################\n");
	printf("####### IMPRIMIENDO GRAFO #######\n");
	printf("#################################\n\n");
	for (int i = 0; i < g->numNodos; i++){
		imprimirAdyacentes(g->matrizAdyacencia[i]);
		if (i != g->numNodos) printf("\n");
	}
}

/*
 * Evalua el camino mas corto hasta v, lo reemplaza por u-v en caso de ser mejorado.
 * Entradas: Nodo* u -> nodo inicio.
 * 			 Nodo* v -> nodo destino.
 * 		     int w -> tiempo de trayecto entre u y v.
 * Salida: void.
*/
void relax(Nodo* u, Nodo* v, int w){
	if (v->tiempoAcumulado == -1){
		v->tiempoAcumulado = u->tiempoAcumulado + w;
	}
	else if (v->tiempoAcumulado > u->tiempoAcumulado + w){
		v->tiempoAcumulado =  u->tiempoAcumulado + w;
		v->padre = u;
	}
}

/*
 * Encuentra el nodo con 'tiempoAcumulado' minimo del grafo
 * Entrada: Grafo* g -> red de consultorios cargada.
 * Salida: ListaAdyacencia* -> Lista correspondiente al nodo encontrado
*/
ListaAdyacencia* min(Grafo* g){
	ListaAdyacencia* temp = g->matrizAdyacencia[0];
	// En busqueda del nodo con tiempo minimo
	for (int i = 1; i < g->numNodos; i++){
		// Si el nodo tiene tiempo minimo menor al candidato actual
		if (g->matrizAdyacencia[i]->origen->tiempoAcumulado <
			temp->origen->tiempoAcumulado){
			temp = g->matrizAdyacencia[i];
		}
	}
	return temp;
}
/*
 * Ingresa un paciente al sistema, buscando la ruta mas corta entre un 
 * consultorio origen y otro con una especialidad buscada.
 * Entradas: Grafo* g -> Red de consultorios cargada.
 * 			 Nodo* s -> consultorio origen.
 * 			 char* especialidad -> especialidad buscada.
 * Salida: Nodo* -> Destino encontrado.
*/
Nodo* ingresarPaciente(Grafo* g, Nodo* s, char* especialidad){
	// Inicializar todas las distancias como inaccesibles, excepto el origen
	// Para cada nodo del grafo
	for (int i = 0; i < g->numNodos; i++){
		// Si el nodo del grafo no es el nodo origen
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio,
			s->nombreConsultorio) != 0){
				g->matrizAdyacencia[i]->origen->padre = NULL;
				g->matrizAdyacencia[i]->origen->tiempoAcumulado = -1;
		} else {
			// Este es el caso del nodo origen
			g->matrizAdyacencia[i]->origen->padre = NULL;
			g->matrizAdyacencia[i]->origen->tiempoAcumulado = 0;
		}
	}
	int aux = 0;
	while (aux == 0) {
		ListaAdyacencia* minim = min(g);
		// Comprobamos si el minimo es un consultorio de la especialidad
		// buscada y ademas tiene cupo
		if (strcmp(minim->origen->especialidad, especialidad) == 0 &&
			minim->origen->pacientesActuales < minim->origen->pacientesMaximos){
			minim->origen->pacientesActuales += 1;
			aux = 1;
			return minim->origen;
		// Sino seguimos buscando
		} else {
			// Para cada nodo adyacente al minimo
			NodoAdyacente* cursor = minim->inicio;
			while (cursor->siguiente != NULL){
				Nodo* u = minim->origen;
				Nodo* v = cursor->consultorio;
				int w = cursor->tiempo;
				relax(u, v, w);
				cursor = cursor->siguiente;
			}
		}
	}
	return NULL;
}

/*
 * Escribe la ruta mas corta encontrada en un archivo.
 * Entradas: Grafo* g -> red de consultorios cargada.
 * 			 Nodo* destino -> destino encontrado luego de ingresar a un paciente.
 * 			 char* path -> ubicacion del archivo a escribir.
 * Salida: void
*/
void escribirRuta(Grafo* g, Nodo* destino, char* path){
	/*
	NodoAdyacente* inicio = (NodoAdyacente*)malloc(sizeof(NodoAdyacente));
	inicio->consultorio = destino
	NodoAdyacente* final = inicio;
	Nodo* cursor = destino;
	while(cursor->padre != NULL){
		if (final != inicio){
			final = (NodoAdyacente*)malloc(sizeof(NodoAdyacente));
			final->consultorio = cursor
		}
	}
	*/
}
