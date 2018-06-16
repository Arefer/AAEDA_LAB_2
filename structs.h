#ifndef STRUCTS_H
#define STRUCTS_H

struct nodo{
	struct nodo* padre; 
	char* nombreConsultorio;
	char* especialidad;
	int pacientesMaximos;
	int pacientesActuales;
	int tiempoAcumulado;
};
typedef struct nodo Nodo;

struct nodoAdyacente{
	int tiempo;
	Nodo* consultorio;
	struct nodoAdyacente* siguiente;
};
typedef struct nodoAdyacente NodoAdyacente;

struct listaAdyacencia{
	Nodo* origen;
	NodoAdyacente* inicio;
	NodoAdyacente* final;
	int numNodosAdyacentes;
};
typedef struct listaAdyacencia ListaAdyacencia;

struct grafo{
	ListaAdyacencia** matrizAdyacencia;
	int numNodos;
};
typedef struct grafo Grafo;

struct minPriorityStack{
	ListaAdyacencia* inicio;
	ListaAdyacencia* final;
	int numElementos;
};
typedef struct minPriorityStack MinPrioStack;
#endif /* STRUCTS_H */
