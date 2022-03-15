#include <stdio.h>
#include <stdlib.h>
#include "heap.h"


heap_t* heap_crear(int tope, comparador compara, destructor_gym destruir){
	if(!destruir || !compara) return NULL;
	heap_t* heap = calloc(1, sizeof(heap_t));
	if(!heap) return NULL;
	heap->compara= compara;
	heap->destruir = destruir;
	heap->tope = tope;
	heap->vector= NULL;
	return heap;
}

int posicion_padre(int tope){
	return (tope-1)/2;
}

void swap(void** vector, int i, int j){
	void* aux= vector[i];
	vector[i]= vector[j];
	vector[j]=aux;
}

void shift_up(heap_t* heap, int tope){
	if(tope==0)
		return;
	int padre= posicion_padre(tope);
	if(heap->compara(heap->vector[tope],heap->vector[padre])==-1){
		swap(heap->vector, tope, padre);
		shift_up(heap, padre);
	}
}

int heap_insertar(heap_t* heap, void* elemento){
	if(!heap) return ERROR;
	void** vector= realloc(heap->vector, sizeof(elemento)*((unsigned int)heap->tope+1));
	if(!vector)
		return ERROR;
	heap->vector= vector;
	heap->vector[heap->tope]= elemento;
	shift_up(heap, heap->tope);
	heap->tope++;
	return EXITO;
}

int pos_hijo_derecho(int tope){
	return ((2*tope)+2);
}

int pos_hijo_izquierdo(int tope){
	return ((2*tope)+1);
}

void shift_down(heap_t* heap, int tope){
	int derecha= pos_hijo_derecho(tope);
	int izquierda= pos_hijo_izquierdo(tope);
	if(derecha> heap->tope && izquierda> heap->tope)
		return;
	else if(derecha> heap->tope){
		if(heap->compara(heap->vector[heap->tope], heap->vector[izquierda])==1)
			swap(heap->vector, tope, izquierda);
		return;
	}
	else if(izquierda> heap->tope){
		if(heap->compara(heap->vector[heap->tope], heap->vector[derecha])==1)
			swap(heap->vector, tope, derecha);
		return;
	}
	int pos_candidata= -1;
	if(heap->compara(heap->vector[heap->tope], heap->vector[izquierda])==-1) return;
	if(heap->compara(heap->vector[heap->tope], heap->vector[derecha])==1 && heap->compara(heap->vector[derecha],heap->vector[izquierda])==1)
		pos_candidata= derecha;
	else pos_candidata= izquierda;
	if(heap->compara(heap->vector[pos_candidata], heap->vector[tope])==-1){
		swap(heap->vector, tope, pos_candidata);
		shift_down(heap, pos_candidata);
	}
}

void* heap_extraer_raiz(heap_t* heap){
	if(!heap  || !heap->vector) return NULL;
	void* aux= heap->vector[0];
	heap->vector[0]= heap->vector[heap->tope-1];
	void** vector= realloc(heap->vector, sizeof(void*)*((unsigned int)heap->tope));
	if(!vector) return NULL;
	heap->vector= vector;
	heap->tope--;
	shift_down(heap,0);
	if(!heap->vector[0]) heap->vector=NULL;
	return aux;
}

bool heap_vacio(heap_t* heap){
	if(!heap) return true;
	if(heap->tope==0) return true;
	return false;
}

void* heap_ultimo(heap_t* heap){
	if(!heap || !heap->vector) return NULL;
	return heap->vector[heap->tope-1];
}


void* heap_raiz(heap_t* heap){
	if(!heap || !heap->vector) return NULL;
	return heap->vector[0];
}

void heap_destruir(heap_t* heap){
	if(!heap) return;
	if(heap->destruir){
		for(int i=0; i<heap->tope; i++){
			heap->destruir(heap->vector[i]);
		}
	}
	free(heap->vector);
	free(heap);
}
