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
		fscanf(archivoAdj, "%99[^\n]", buffer);
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
 * Salida: int -> 1 si el valor de V fue reemplazado, 0 en caso contrario.
*/
int relax(Nodo* u, Nodo* v, int w){
	int r = 0;
	if (v->tiempoAcumulado > u->tiempoAcumulado + w){
		v->tiempoAcumulado =  u->tiempoAcumulado + w;
		v->padre = u;
		r = 1;
		printf("Tiempo reemplazado: %d\n", u->tiempoAcumulado + w);
	}
	return r;
}
/* Crea un stack de prioridad minima a partir del grafo.
 * Entrada: Grafo* g -> red de consultorios cargada.
 * Salida: MinPrioStack* de ListaAdyacencia*.
*/
MinPrioStack* crearStack(Grafo* g){
	MinPrioStack* stack = (MinPrioStack*)malloc(sizeof(MinPrioStack));
	stack->numElementos = 0;
	for (int i = 0; i < g->numNodos; i++){
		addToStack(stack, g->matrizAdyacencia[i]);
	}
	return stack;
}

void printStack(MinPrioStack* s){
	ListaAdyacencia* cursor = s->inicio;
	while (cursor != NULL){
		printf("Consultorio = '%s' ; TiempoAcumulado = '%d'\n", cursor->origen->nombreConsultorio, cursor->origen->tiempoAcumulado);
		if (cursor == s->final){
			cursor->siguiente = NULL;
			cursor = cursor->siguiente;
		} else {
			cursor = cursor->siguiente;
		}
	} 
}
/* Extrae el elemento minimo del stack (el primero en la pila).
 * Entrada: MinPrioStack stack -> stack a extraer el elemento.
 * Salida: ListaAdyacencia* -> el elemento minimo
*/
ListaAdyacencia* extractMin(MinPrioStack* stack){
	ListaAdyacencia* min = stack->inicio;
	stack->inicio = min->siguiente;
	stack->numElementos -= 1;
	return min;
}
/* Agrega un elemento al stack de prioridad minima.
 * Entradas: MinPrioStack* stack-> stack de minima prioridad.
 * 			 ListaAdyacencia* elem -> elemento a agregar al stack.
 * Salida: void.
*/
void addToStack(MinPrioStack* stack, ListaAdyacencia* elem){
	
	// Si el stack esta vacio
	if (stack->numElementos == 0){
		stack->inicio = elem; // Elemento minimo (en lo mas alto del stack) 
		stack->final = elem; // Elemento maximo (en la base del stack)
		stack->numElementos += 1;
	} else { // Si no esta vacio
		// Caso donde el elemento a agregar es mayor o igual al final
		if (elem->origen->tiempoAcumulado >= stack->final->origen->tiempoAcumulado){
			stack->final->siguiente = elem;
			stack->final = elem;
			stack->numElementos += 1;
		}
		// Caso donde el elemento a agregar es menor o igual al principio
		else if (elem->origen->tiempoAcumulado <= stack->inicio->origen->tiempoAcumulado){
			elem->siguiente = stack->inicio;
			stack->inicio = elem;
			stack->numElementos += 1;
		} else {
			// Caso en donde el elemento se agrega no en los extremos
			ListaAdyacencia* cursor = stack->inicio;
			int aux = 0; // Indica si se añadio el elemento al stack
			while (aux == 0) {
				int top = cursor->origen->tiempoAcumulado;
				int bottom = cursor->siguiente->origen->tiempoAcumulado;
				ListaAdyacencia* bottomList = cursor->siguiente;
				// Si el tiempo de elem esta entre top y bottom
				if (elem->origen->tiempoAcumulado >= top && elem->origen->tiempoAcumulado <= bottom){
					cursor->siguiente = elem;
					elem->siguiente = bottomList;
					stack->numElementos += 1;
					aux = 1; // Indicamos que se añadio el elemento
				}
				cursor = cursor->siguiente;
			}
		}
	}
}

/* Reordena el Stack, luego de haber actualizado el tiempo de un Nodo.
 * Entradas: MinPrioStack* s -> stack de ListaAdyacencia*.
 * 			 Nodo* elem -> Nodo que fue modificado (tiempoAcumulado).
 * Salidas: void. (El stack se modifica por referencia).
*/
void reOrderStack(MinPrioStack* s, Nodo* elem){
	ListaAdyacencia* cursor = s->inicio;
	ListaAdyacencia* eliminado;
	// Si el elemento a eliminar esta en el top del stack
	if (elem == s->inicio->origen){
		eliminado = s->inicio;
		s->inicio = s->inicio->siguiente;
		s->numElementos -= 1;
	} else { // Sino
		while(cursor != NULL && cursor->siguiente != NULL){
			if (cursor->siguiente->origen == elem){
				eliminado = cursor->siguiente;
				ListaAdyacencia* nuevoSiguiente;
				// Vemos el caso en que el elem a eliminar sea el ultimo
				if (eliminado == s->final){
					s->final = cursor;
					nuevoSiguiente = NULL;
					cursor->siguiente = nuevoSiguiente;
					s->numElementos -= 1;
				// En cualquier otro caso
				} else {
					nuevoSiguiente = cursor->siguiente->siguiente;
					cursor->siguiente = nuevoSiguiente;
					s->numElementos -= 1;
				}
			}
			cursor = cursor->siguiente;
		}
	}
	eliminado->siguiente = NULL;
	
	// Ahora añadimos nuevamente el elemento eliminado, pero en orden
	addToStack(s, eliminado);
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
				// Asignamos su costo como el maximo valor que puede adoptar un int
				g->matrizAdyacencia[i]->origen->tiempoAcumulado = 2147483647;
				// Desenlazamos el stack en caso de que se haya creado
				g->matrizAdyacencia[i]->siguiente = NULL;
		} else if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio,
			s->nombreConsultorio) == 0){
			// Este es el caso del nodo origen
			g->matrizAdyacencia[i]->origen->padre = NULL;
			g->matrizAdyacencia[i]->origen->tiempoAcumulado = 0;
			// Desenlazamos el stack en caso de que se haya creado
			g->matrizAdyacencia[i]->siguiente = NULL;
			printf("El nodo origen es: %s\n", g->matrizAdyacencia[i]->origen->nombreConsultorio);
		}
	}
	MinPrioStack* nodos = crearStack(g);
	printf("\n\n----------STACK----------\n\n");
	printStack(nodos);
	while (nodos->numElementos != 0) {
		ListaAdyacencia* minim = extractMin(nodos);
		printf("Extraido del stack '%s'", minim->origen->nombreConsultorio);
		printf("\n\n----------STACK----------\n\n");
		printStack(nodos);
		// Comprobamos si el minimo es un consultorio de la especialidad
		// buscada y ademas tiene cupo
		if (strcmp(minim->origen->especialidad, especialidad) == 0 &&
			minim->origen->pacientesActuales < minim->origen->pacientesMaximos){
			minim->origen->pacientesActuales += 1;
			// En este caso la busqueda ha concluido
			free(nodos);
			return minim->origen;
		// Sino seguimos buscando
		} else {
			// En este caso el minimo no es el destino o no tenia cupo
			printf("El minimo (%s) no es el destino\n", minim->origen->nombreConsultorio);
			// Para cada nodo adyacente al minimo
			NodoAdyacente* cursor = minim->inicio;
			while (cursor != NULL){
				Nodo* u = minim->origen;
				Nodo* v = cursor->consultorio;
				int w = cursor->tiempo;
				printf("Relax con v='%s ; tiempoAcum='%d'\n", v->nombreConsultorio, v->tiempoAcumulado);
				// Comprobamos si el camino para llegar a v puede ser mejorado
				int r = relax(u, v, w);
				// Si el valor de v es reemplazado, reordenamos el stack
				if (r == 1){
					reOrderStack(nodos, v);
				}
				cursor = cursor->siguiente;
			}
			printf("Stack luego de relax para todos los nodos adyacentes de %s", minim->origen->nombreConsultorio);
			printf("\n\n----------STACK----------\n\n");
			printStack(nodos);
		}
	}
	free(nodos);
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
	Nodo* cursor = destino;
	int contador = 0; // Aqui contaremos la longitud del camino
	while (cursor != NULL){
		contador++;
		cursor = cursor->padre;
	}
	// Definiremos un arreglo de nodos
	Nodo** camino = (Nodo**)malloc(sizeof(Nodo*)*contador);
	// Agregamos los nodos del camino de forma inversa
	cursor = destino;
	int aux = contador-1;
	while(cursor != NULL){
		camino[aux] = cursor;
		cursor = cursor->padre;
		aux -= 1;
	}
	
	// Imprimir camino
	// Abrimos el archivo
	FILE* out = fopen(path, "w");
	if (out == NULL){
		printf("Error al crear/acceder al archivo\n");
		exit(-1);
	}
	printf("\n\n------ IMPRIMIENDO CAMINO ------\n\n");
	for (int i = 0; i < contador-1; i++){
		printf("%s ", camino[i]->nombreConsultorio);
		fprintf(out, "%s ", camino[i]->nombreConsultorio);
		printf("%s ", camino[i+1]->nombreConsultorio);
		fprintf(out, "%s ", camino[i+1]->nombreConsultorio);
		printf("%d\n", camino[i+1]->tiempoAcumulado - camino[i]->tiempoAcumulado);
		fprintf(out, "%d\n", camino[i+1]->tiempoAcumulado - camino[i]->tiempoAcumulado);
	}
	printf("\nTiempo total: %d minutos.\n", camino[contador-1]->tiempoAcumulado);
	fprintf(out, "\nTiempo total: %d minutos.", camino[contador-1]->tiempoAcumulado);
	printf("Camino escrito en '%s'\n", path);
	fclose(out);
	// Liberar el camino
	free(camino);
}

/* Dado el nombre de un consultorio, libera un cupo en dicho consultorio.
 * Entrada: Grafo* g -> red de consultorios.
 * 			char* consultorio -> nombre del consultorio.
 * Salida: void. (el registro se modifica por referencia) 
*/
void darDeAlta(Grafo* g, char* consultorio){
	// Recorremos el grafo en busca del consultorio especificado
	for (int i = 0; i < g->numNodos; i++){
		// Consultamos el nombre del consultorio
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, consultorio) == 0){
			// Si el nombre coincide, pero no hay pacientes
			if (g->matrizAdyacencia[i]->origen->pacientesActuales == 0){
				printf("Ningun paciente es atendido aqui\n");
			} else { // Si hay pacientes, eliminamos a uno del registro
				printf("Cupo liberado correctamente\n");
				g->matrizAdyacencia[i]->origen->pacientesActuales -= 1;
			}
			
		}
	}
}
void liberarGrafo(Grafo* g){
	for (int i = 0; i < g->numNodos; i++){
		// Liberar el nodo origen
		free(g->matrizAdyacencia[i]->origen->nombreConsultorio);
		free(g->matrizAdyacencia[i]->origen->especialidad);
		free(g->matrizAdyacencia[i]->origen);
		// Liberar la lista de adyacencia
		NodoAdyacente* cursor = g->matrizAdyacencia[i]->inicio;
		int j = 0;
		while (j < g->matrizAdyacencia[i]->numNodosAdyacentes){
			NodoAdyacente* siguiente = cursor->siguiente;
			free(cursor);
			cursor = siguiente;
			j++;
		}
		free(g->matrizAdyacencia[i]);
	}
	free(g);
}
