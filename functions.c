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
 * T(n) = 10 ; O(n) = 1
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
 * T(n) = 2n+1 ; O(n) = n
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
 * T(n) = n^2+2n+5 ; O(n) = n^2
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
 * Salida: Grafo* -> un puntero al grafo creado a través de la lectura.
 * T(n) = 32+9n+10n^2+n^3+n^4 ; O(n) = n^4
*/
Grafo* leerGrafo(char* path, char* pathAristas){
	#pragma region Grafo y Variables auxiliares
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
	#pragma endregion
	printf("\n############# LEYENDO REGISTRO #############\n\n");
	#pragma region Lectura de archivo y validacion de memoria
	FILE* archivo = fopen(path, "rb");
	if (archivo ==  NULL){
		printf("No se pudo abrir el archivo de consultorios\n");
		exit(-1);
	}
	fscanf(archivo, "%s", buffer);
	#pragma endregion
	g->numNodos = atoi(buffer);
	printf("Num consultorios: %d\n", g->numNodos);
	g->matrizAdyacencia = (ListaAdyacencia**)malloc(sizeof(ListaAdyacencia*)*(g->numNodos));
	// Leer el salto de linea pendiente
	fgets(buffer, sizeof(buffer), archivo);
	// Este ciclo es para leer el archivo Consultorios.in y agregarlos al grafo
	while (i < g->numNodos){
		#pragma region Leer datos, crear un nodo y agregarlo al grafo
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
		#pragma endregion
	}
	fclose(archivo);
	#pragma region Lectura segundo archivo y validacion de memoria
	FILE* archivoAdj = fopen(pathAristas, "rb");
	if (archivoAdj == NULL){
		printf("No se pudo abrir el archivo de consultorios adyacentes\n");
		exit(-1);
	}
	fscanf(archivoAdj, "%s", valor);
	#pragma endregion
	numAdyacentes = atoi(valor);
	fgets(valor, sizeof(valor), archivoAdj);
	i = 0;
	// Este ciclo es para agregar los consultorios adyacentes a cada consultorio
	while (i < numAdyacentes){
		#pragma region Leer datos, crear nodo adyacente y agregarlo al grafo
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
		#pragma endregion
	}
	#pragma region Liberando memoria
	fclose(archivoAdj);
	free(buffer);
	free(nombreConsultorio);
	free(especialidad);
	free(consultorioAdyacente);
	printf("\n############# LECTURA DEL REGISTRO COMPLETADA #############\n\n");
	#pragma endregion
	return g;
}
/*
 * Imprime la lista de adyacencia de un nodo con el formato nodoOrigen consultorioAdyaycente (tiempo) ; ...
 * Entrada: ListaAdyacencia* lista -> la lista a imprimir.
 * Salida: void.
 * T(n) = 3n+2 ; O(n) = n
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
 * T(n) = n^2-n+4 ; O(n) = n^2
*/
void imprimirGrafo(Grafo* g){
	printf("\n#################################\n");
	printf("####### IMPRIMIENDO GRAFO #######\n");
	printf("#################################\n\n");
	for (int i = 0; i < g->numNodos; i++){
		imprimirAdyacentes(g->matrizAdyacencia[i]);
		if (i != g->numNodos) printf("\n");
	}
}

/*
 * Imprime la lista de consultorios en memoria.
 * Entrada: Grafo* g -> grafo de consultorios en memoria.
 * Salida: void.
 * T(n) = ; O(n) = 
*/
void imprimirConsultorios(Grafo* g){
	printf("\n###### MOSTRANDO REGISTRO ######\n");
	for (int i = 0; i < g->numNodos; i++){
		char* nombre = g->matrizAdyacencia[i]->origen->nombreConsultorio;
		char* especialidad = g->matrizAdyacencia[i]->origen->especialidad;
		int pacientesMaximos = g->matrizAdyacencia[i]->origen->pacientesMaximos;
		int pacientesActuales = g->matrizAdyacencia[i]->origen->pacientesActuales;
		printf("Nombre: %s ; Especialidad: %s ; Capacidad maxima: %d ; Pacientes actuales %d", 
			nombre, especialidad, pacientesMaximos, pacientesActuales);
		printf("\n");
	}
}
/*
 * Evalua el camino mas corto hasta v, lo reemplaza por u-v en caso de ser mejorado.
 * Entradas: Nodo* u -> nodo inicio.
 * 			 Nodo* v -> nodo destino.
 * 		     int w -> tiempo de trayecto entre u y v.
 * Salida: void.
 * T(n) = 4 ; O(n) = 1
*/
void relax(Nodo* u, Nodo* v, int w){
	if (v->tiempoAcumulado > u->tiempoAcumulado + w){
		v->tiempoAcumulado =  u->tiempoAcumulado + w;
		v->padre = u;
	}
}
/*
 * Extrae la ListaAdyacencia* cuyo nodo origen posee el tiempo minimo.
 * Entradas: ListaAdyacencia** matrizAdyacencia -> Arreglo de ListaAdyacencia* al que se desea extraer el minimo.
 * 			 int largo -> largo del arreglo.
 * Salida: ListaAdyacencia* -> ListaAdyacencia* cuyo origenn posee el tiempo minimo. 
 * T(n) = 6n+6 ; O(n) = n
*/ 
ListaAdyacencia* extractMin(ListaAdyacencia** matrizAdyacencia, int largo){
	#pragma region Buscar el indice del elemento minimo
	// Asumimos que el mimino es el primer elemento
	ListaAdyacencia* min = matrizAdyacencia[0];
	int indexMin = 0;
	int i = 1;
	while(i < largo){
		// Si el tiempo del nodo es menor al minimo actual, reemplazar el minimo
		ListaAdyacencia* actual = matrizAdyacencia[i];

		if (matrizAdyacencia[i]->origen->tiempoAcumulado < min->origen->tiempoAcumulado){
			min = matrizAdyacencia[i];
			indexMin = i;
		}
		i++;
	}
	#pragma endregion
	// Reordenamos el arreglo

	for (i = indexMin; i < largo-1; i++){
		matrizAdyacencia[i] = &*matrizAdyacencia[i+1];
		//matrizAdyacencia[i] = matrizAdyacencia[i+1];
	}

	// Si el arreglo poseía un elemento, liberar la memoria y devolver ese elemento
	if (largo == 1) {
		free(matrizAdyacencia);
		return min;
	}

	// Sino, realocar la memoria
	//ListaAdyacencia** temp = realloc(matrizAdyacencia, sizeof(ListaAdyacencia*)*(largo-1));
	//matrizAdyacencia = temp;
	return min;
}

/*
 * Ingresa un paciente al sistema, buscando la ruta mas corta entre un 
 * consultorio origen y otro con una especialidad buscada.
 * Entradas: Grafo* g -> Red de consultorios cargada.
 * 			 Nodo* s -> consultorio origen.
 * 			 char* especialidad -> especialidad buscada.
 * Salida: Nodo* -> Destino encontrado.
 * T(n) = 14n^2+3n+2 ; O(n) = n^2
*/
Nodo* ingresarPaciente(Grafo* g, Nodo* s, char* especialidad){
	#pragma region Inicializar nodos como inaccesibles a excepción del origen
	// Para cada nodo del grafo
	for (int i = 0; i < g->numNodos; i++){
		// Si el nodo del grafo no es el nodo origen
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio,
			s->nombreConsultorio) != 0){
				g->matrizAdyacencia[i]->origen->padre = NULL;
				// Asignamos su costo como el maximo valor que puede adoptar un int
				g->matrizAdyacencia[i]->origen->tiempoAcumulado = 2147483647;
		} else if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio,
			s->nombreConsultorio) == 0){
			// Este es el caso del nodo origen
			g->matrizAdyacencia[i]->origen->padre = NULL;
			g->matrizAdyacencia[i]->origen->tiempoAcumulado = 0;
		}
	}
	#pragma endregion
	
	// Copia del arreglo de ListaAdyacencia* del grafo
	ListaAdyacencia** nodosSinOptimizar = (ListaAdyacencia**)malloc(sizeof(ListaAdyacencia*)*g->numNodos);
	// Asignando los punteros del grafo al arreglo
	for (int k = 0; k < g->numNodos; k++){
		nodosSinOptimizar[k] = g->matrizAdyacencia[k];
	}
	int largoArreglo = g->numNodos;
	// Mientras queden elementos en el arreglo
	while (largoArreglo > 0) {
		ListaAdyacencia* minim = extractMin(nodosSinOptimizar, largoArreglo);
		largoArreglo -= 1;
		// Comprobamos si el minimo es un consultorio de la especialidad
		// buscada y ademas tiene cupo
		if (strcmp(minim->origen->especialidad, especialidad) == 0 &&
			minim->origen->pacientesActuales < minim->origen->pacientesMaximos){
			minim->origen->pacientesActuales += 1;
			// En este caso la busqueda ha concluido
			if (largoArreglo > 0)
				free(nodosSinOptimizar);
			return minim->origen;
		// Sino seguimos buscando
		} else {
			// En este caso el minimo no es el destino o no tenia cupo
			NodoAdyacente* cursor = minim->inicio;
			while (cursor != NULL){ // Para cada nodo adyacente al minimo
				Nodo* u = minim->origen;
				Nodo* v = cursor->consultorio;
				int w = cursor->tiempo;
				// Comprobamos si el camino para llegar a v puede ser mejorado
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
 * T(n) = 11n+12 ; O(n) = n
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
	
	#pragma region Escribir camino en archivo
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
	#pragma endregion
	// Liberar el camino
	free(camino);
}

/* Dado el nombre de un consultorio, libera un cupo en dicho consultorio.
 * Entrada: Grafo* g -> red de consultorios.
 * 			char* consultorio -> nombre del consultorio.
 * Salida: void. (el registro se modifica por referencia) 
 * T(n) = 5n+1 ; O(n) = n 
*/
void darDeAlta(Grafo* g, char* consultorio){
	// Recorremos el grafo en busca del consultorio especificado
	int aux = 0; // Indicador para verificar que existe el consultorio
	for (int i = 0; i < g->numNodos; i++){
		// Consultamos el nombre del consultorio
		if (strcmp(g->matrizAdyacencia[i]->origen->nombreConsultorio, consultorio) == 0){
			// Si el nombre coincide, pero no hay pacientes
			if (g->matrizAdyacencia[i]->origen->pacientesActuales == 0){
				printf("Ningun paciente es atendido aqui\n");
				aux = 1;
				i = g->numNodos;
			} else { // Si hay pacientes, eliminamos a uno del registro
				printf("Cupo liberado correctamente\n");
				g->matrizAdyacencia[i]->origen->pacientesActuales -= 1;
				aux = 1;
				i = g->numNodos;
			}
		}
	}
	if (aux == 0){
		printf("Nombre de consultorio no valido (No se encuentra en el registro)\n");
	}
}

/* Actualiza el archivo con la informacion cargada en el programa
 * Entradas: Grafo* g -> Red de consultorios cargada.
 * 			 char* path -> Ruta del archivo.
 * Salida: void
 * T(n) = 5n+3 ; O(n) = n
*/
void guardarConsultorios(Grafo* g, char* path){
		// Inicializamos el archivo en modo escritura
		FILE* archivo = fopen(path, "w");
		if (archivo == NULL){
			printf("Error al abrir el archivo\n");
			exit(-1);
		}
		// Escribimos el numero de consultorios en la primera linea
		fprintf(archivo, "%d\n", g->numNodos);
		// Para cada consultorio en el registro (grafo)
		for (int i = 0; i < g->numNodos; i++){
			// Consultorio especialidad pacientes_maximos pacientes_actuales 
			fprintf(archivo, "%s ", g->matrizAdyacencia[i]->origen->nombreConsultorio);
			fprintf(archivo, "%s ", g->matrizAdyacencia[i]->origen->especialidad);
			fprintf(archivo, "%d ", g->matrizAdyacencia[i]->origen->pacientesMaximos);
			fprintf(archivo, "%d", g->matrizAdyacencia[i]->origen->pacientesActuales);
			// Escribimos salto de linea en caso de que no sea la ultima
			if (i != g->numNodos - 1){
				fprintf(archivo, "\n");
			}
		}
		fclose(archivo);
}

void menu(){
    printf("##### BIENVENIDO/A #####\n\n");
    int menu = 0;
    int registroCargado = 0;
    char* pathConsultorios = (char*)malloc(sizeof(char)*16);
    char* pathRutas = (char*)malloc(sizeof(char)*22);
    char* pathCamino = (char*)malloc(sizeof(char)*15);
    Grafo* g;
    int option = 0;
    strcpy(pathConsultorios, "Consultorios.in");
    strcpy(pathRutas, "DondeLlevarAlBulto.in");
    strcpy(pathCamino, "WiiuuWiiuu.out");
    
    while (menu >= 0){
        printf("1.- Cargar registro de Consultorios\n");
        printf("2.- Mostrar registro\n");
        printf("3.- Agregar a un paciente\n");
        printf("4.- Dar de alta a un paciente\n");
        printf("5.- Actualizar fichero de Consultorios\n");
        printf("6.- Salir\n\n");
        printf("Ingresa una opcion: ");
        scanf("%d", &option);
        switch(option){
            case 1: if (registroCargado == 1) {
                        printf("Ya hay un archivo de registro cargado\n\n");
                    }else{
                        g = leerGrafo(pathConsultorios, pathRutas);
                        registroCargado = 1;
                    }
                    break;

            case 2: if (registroCargado == 1){
                        imprimirConsultorios(g);
						printf("\n");
                    } else {
                        printf("No existe registro en el programa. Por favor cargue uno.\n\n");
                    }
                    
                    break;

            case 3: if (registroCargado == 1){
						char* nombreConsultorio = (char*)malloc(sizeof(char)*30);
						char* especialidadRequerida = (char*)malloc(sizeof(char)*30);
						Nodo* source;
						Nodo* destino;
                        printf("Ingrese el nombre del consultorio origen: ");
                        scanf("%s", nombreConsultorio);
                        printf("Ingrese la especialidad requerida: ");
                        scanf("%s", especialidadRequerida);
                        source = buscarNodo(nombreConsultorio, g);
                        if (source == NULL){
							printf("Consultorio origen no existe en el registro\n\n");
							free(nombreConsultorio);
							free(especialidadRequerida);
						}else{
							destino = ingresarPaciente(g, source, especialidadRequerida);
							if (destino == NULL){
								printf("No es posible atender al cliente, no hay cupos o especialidad requerida\n\n");
								free(nombreConsultorio);
								free(especialidadRequerida);
							} else {
								escribirRuta(g, destino, pathCamino);
								printf("\n");
								free(nombreConsultorio);
								free(especialidadRequerida);
							}
						}
						
                    } else {
                        printf("No existe registro en el programa. Por favor cargue uno.\n\n");
                    }
                    break;

            case 4: if (registroCargado == 1) {
                        char* nombreConsultorio = (char*)malloc(sizeof(char)*30);
                        printf("Ingrese el nombre del consultorio en el cual fue atendido el paciente: ");
                        darDeAlta(g, nombreConsultorio);
                        free(nombreConsultorio);
                    } else {
                        printf("No existe registro en el programa. Por favor cargue uno.\n\n");
                    }
                    break;
            
            case 5: if (registroCargado == 1) {
                        printf("\n\n------ ACTUALIZANDO FICHERO ------\n\n");
                        guardarConsultorios(g, pathConsultorios);
                        printf("Fichero actualizado exitosamente\n");
                    } else {
                        printf("No existe registro en el programa. Por favor cargue uno.\n\n");
                    }
                    break;
            
            case 6: printf("Saliendo...\n");
                    if (registroCargado == 1) liberarGrafo(g);
                    free(pathConsultorios);
                    free(pathRutas);
                    free(pathCamino);
                    menu = -1;
                    break;
                    
            default: printf("Opcion no valida\n\n");
        }
    }
}

/*
 * Libera la memoria ocupada por el grafo.
 * Entradas: Grafo* g -> Grafo a liberar.
 * Salida: void.
 * T(n) = 4n^2+n+1 ; O(n) = n^2
*/
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
	free(g->matrizAdyacencia);
	free(g);
}