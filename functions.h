#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"
Nodo* crearNodo(Nodo* padre, char* nombreConsultorio, char* especialidad,
	int pacientesMaximos, int pacientesActuales, int tiempoAcumulado);
Nodo* buscarNodo(char* nombreNodo, Grafo* g);
void agregarAdyacente(Grafo* g, char* consultorioOrigen, char* consultorioAdyacente, int tiempoTrayecto);
Grafo* leerGrafo(char* path, char* pathAristas);
#endif /* FUNCTIONS_H */