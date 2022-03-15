#ifndef __JUGAR_H_
#define __JUGAR_H_

#include "lista.h"
#include "heap.h"
#include "batallas.h"
#include <stdbool.h>
#include <string.h>
#define MAX_NOMBRE 75
#define MAX_COMBATE 6
#define MAX_OBTENIDOS 70
#define MAX_ENTRENADORES 30
#define MAX_POKEMONES 50
#define INICIAL_GYM 'G'
#define INICIAL_LIDER 'L'
#define INICIAL_ENTRENADOR 'E'
#define INICIAL_POKEMON 'P'
#define TOPE_INICIAL 0
#define MAX_COMBATE 6

typedef struct personaje{
	char nombre[MAX_NOMBRE];
	lista_t* pokemones_combate;
	lista_t* pokemones_obtenidos;
} personaje_t;

typedef struct gimnasio{
	char nombre[MAX_NOMBRE];
	int dificultad;
	int id_puntero_a_funcion;
	lista_t* entrenadores;
} gimnasio_t;


typedef struct juego{
	heap_t* gimnasios;
	int cantidad_gym;
	personaje_t personaje;
	gimnasio_t* gym_actual;
}juego_t;

typedef struct pokemon{
	int velocidad;
	int defensa;
	int ataque;
	int altura;
	int ritmo;
	int puntos_sumados;
	char nombre[MAX_NOMBRE];
} pokemon_t;

typedef struct entrenador{
	char nombre[MAX_NOMBRE];
	bool es_lider;
	lista_t* pokemon;
} entrenador_t;



// RECIBE UN ARCHIVO DE GIMNASIOS Y LO LEE PARA GUARDAR SU INFORMACION EN LAS ESTRUCTURAS 
//ADECUADAS (UN HEAP DE GIMNASIOS CON UNA PILA DE ENTRENADORES QUE TIENEN UNA LISTA DE POKEMONES CADA UNO).
int cargar_gimnasio(juego_t* juego, char* ruta_archivo, comparador compare, destructor destructe, destructor destruye_entrena, destructor destructor_poke);


//RECIBE UN ARCHIV ODE PERSONAJE Y LO LEE PARA GUARDAR SU INFORMACION EN LAS ESTRUCTURAS ADECUADAS (DOS LISTAS DE POKEMONES).
int cargar_personaje(juego_t* juego, char* ruta_archivo, destructor destruye);

//JUEGA UNA PARTIDA
int jugar(juego_t* juego, gimnasio_t* gimnasio);

//SIMULA UNA JUGADA HASTA EL FINAL SIN DEJAR QUE EL USUARIO DECIDA
int simular_jugada(juego_t* juego);

//REPITE LA JUGADA DESDE EL GIMNASIO ACTUAL.
int jugar_denuevo(juego_t* juego, gimnasio_t* gimnasio);

//IMPRIME POR PANTALLA EL PERSONAJE PRINCIPAL Y SUS POKEMONES.
void mostrar_personaje(juego_t juego);

//IMPRIME POR PANTALLA LA INFORMACION DEL GYM.
void informacion_gym(juego_t juego);

//PERMITE AL USUARIO TOMAR ALGUN POKEMON DEL RIVAL VENCIDO.
void tomar_pokemon(juego_t* juego);

//PERMITE AL USUARIO CAMBIAR SUS POKEMON DE PELEA POR LOS QUE DESEE DE COMBATE.
void cambiar_pokemon(juego_t* juego);

//LIBERA TODA LA MEMORIA RESERVADA DE UN GIMNASIO Y SU PILA DE ENTRENADORES Y LISTAS DE POKEMONES.
void liberar_todo(juego_t* juego);


#endif /* __JUGAR_H__ */
