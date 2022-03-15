#ifndef __JUGAR_H_
#define __JUGAR_H_

#include "lista.h"
#include "heap.h"
#define MAX_NOMBRE 75
#define MAX_COMBATE 6
#define MAX_OBTENIDOS 70
#define MAX_ENTRENADORES 30
#define MAX_POKEMONES 50

typedef struct juego{
	heap_t* gimnasios;
}juego_t;

typedef struct pokemon{
	int velocidad;
	int defensa;
	int ataque;
	int altura;
	int ritmo;
	char nombre[MAX_NOMBRE];
} pokemon_t;

typedef struct entrenador{
	char nombre[MAX_NOMBRE];
	bool es_lider;
	lista_t* pokemon;
} entrenador_t;


typedef struct gimnasio{
	char nombre[MAX_NOMBRE];
	int dificultad;
	int id_puntero_a_funcion;
	lista_t* entrenadores;
} gimnasio_t;


typedef struct personaje{
	char nombre[MAX_NOMBRE];
	lista_t* pokemones_combate[MAX_COMBATE];
	lista_t* pokemones_obtenidos[MAX_COMBATE];
} personaje_t;

#endif /* __JUGAR_H__ */
