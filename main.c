/*
 * main.c
 * 
 * Copyright 2018 Kevin Arévalo <Kevin Arévalo@DESKTOP-R01G6B6>
 * 
 * This program is free software; you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation; either version 2 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston,
 * MA 02110-1301, USA.
 * 
 * 
 */

#include "functions.h"

int main(int argc, char **argv)
{
	/*
	char* path1 = (char*)malloc(sizeof(char)*16);
	char* path2 = (char*)malloc(sizeof(char)*22);
	strcpy(path1, "Consultorios.in");
	strcpy(path2, "DondeLlevarAlBulto.in");
	Grafo* g = leerGrafo(path1, path2);
	imprimirGrafo(g);
	
	Nodo* destino = ingresarPaciente(g, g->matrizAdyacencia[0]->origen, "Especialidad_2");
	escribirRuta(g, destino, "asd");
	
	guardarConsultorios(g, "NuevoRegistro.out");
	printf("\n\n------Registro actualizado------\n\n");
	free(path1);
	free(path2);
	liberarGrafo(g);
	*/
	menu();
	return 0;
}
