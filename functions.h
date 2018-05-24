#ifndef FUNCTIONS_H
#define FUNCTIONS_H
#include "structs.h"
Nodo* crearNodo(Nodo* padre, char* nombreConsultorio, char* especialidad,
	int pacientesMaximos, int pacientesActuales, int tiempoAcumulado);
Nodo* buscarNodo(char* nombreNodo, Grafo* g);
void agregarAdyacente(Grafo* g, char* consultorioOrigen, char* consultorioAdyacente, int tiempoTrayecto);
Grafo* leerGrafo(char* path, char* pathAristas);
void imprimirAdyacentes(ListaAdyacencia* lista);
void imprimirGrafo(Grafo* g);
void relax(Nodo* u, Nodo* v, int tiempo);
ListaAdyacencia* min(Grafo* g);
Nodo* ingresarPaciente(Grafo* g, Nodo* s, char* especialidad);
void escribirRuta(Grafo* g, Nodo* destino, char* path);
#endif /* FUNCTIONS_H */