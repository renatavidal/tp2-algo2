#ifndef __HEAP_H__
#define __HEAP_H__
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ERROR -1
#define EXITO 1

/*
 * Comparador de elementos. Recibe dos elementos y devuelve
 * 0 en caso de ser iguales, 1 si el primer elemento es mayor al
 * segundo o -1 si el primer elemento es menor al segundo.
 */
typedef int (*comparador)(void*, void*);
/*
 * Destructor de elementos. Cada vez que un elemento deja el heap
 * se invoca al destructor pasandole
 * el elemento.
 */
typedef void (*destructor_gym)(void*);

typedef struct heap{
	void** vector;
	int tope;
	comparador compara;
	destructor_gym destruir;
}heap_t;

 // crea un heap guardando lugar en el heap
heap_t* heap_crear( int tope, comparador compara, destructor_gym destruye);

// borra el heap y sus elementos
void heap_destruir(heap_t* heap);

//devuleve el ultimo elemento del heap.
void* heap_ultimo(heap_t* heap);

// devuelve true si el heap esta vacio y false si no.
bool heap_vacio(heap_t* heap);

// elimina la raiz y devuelve su contenido
void* heap_extraer_raiz(heap_t* heap);

// devuelve la raiz 
void* heap_raiz(heap_t* heap);

// intercambia el lugar de los elementos para abajo hasta que se haya acomodado el heap
void shift_down(heap_t* heap, int tope);

//devuelve la posicion del hijo derecho de la posicion deseada
int pos_hijo_derecho(int tope);

//devuelve la posicion del hijo izquierdo de la posicion deseada
int pos_hijo_izquierdo(int tope);

// inserta un elemento al heap
int heap_insertar(heap_t* heap, void* elemento);

// intercambia el lugar de los elementos para arriba hasta que se haya acomodado el heap
void shift_up(heap_t* heap, int tope);

// intercambia el lugar de dos elementos
void swap(void** vector, int i, int j);

//devuelve la posicion del padre de la posicion deseada
int posicion_padre(int tope);

#endif /* __HEAP_H__ */
