#include "heap.h"
#include "pa2mm.h"
#include <stdio.h>
#include <stdbool.h>

typedef struct cosa{
    int clave;
    char contenido[10];
}cosa;

cosa* crear_cosa(int clave){
    cosa* c = (cosa*)malloc(sizeof(cosa));
    if(c)
        c->clave = clave;
    return c;
}

void destructe(void* elemento){
    if(elemento)
   		free(elemento);
}

void imprimir(heap_t* heap){
	if(heap->tope>0)
		printf("     %i\n",*(int*)heap->vector[0]);
	printf("     |\n" );
	printf("    ___\n" );
	printf("   |   |\n" );
	if(heap->tope>1)
		printf("   %i",*(int*)heap->vector[1]);
	if(heap->tope>2)
		printf("   %i\n",*(int*)heap->vector[2]);
	printf("   |    |\n" );
	printf("  ___  ___\n" );
	printf(" |   | |  |\n" );
	if(heap->tope>3)
		printf(" %i",*(int*)heap->vector[3]);
	if(heap->tope>4)
		printf("   %i",*(int*)heap->vector[4]);
	if(heap->tope>5)
		printf(" %i",*(int*)heap->vector[5]);
	if(heap->tope>6)
		printf("  %i\n",*(int*)heap->vector[6]);

	for (int i = 0; i < heap->tope; ++i){
		printf("\n%i\n",*(int*)heap->vector[i]);
	}
}

int compare(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;
    if(((cosa*)elemento1)->clave>((cosa*)elemento2)->clave)
        return 1;
    if(((cosa*)elemento1)->clave<((cosa*)elemento2)->clave)
        return -1;
    return 0;
}


void pruebas_crear(){
	heap_t* _no_nulo= heap_crear( 0, compare, destructe);
	pa2m_afirmar(_no_nulo != NULL, "Se crea ");
	pa2m_afirmar(_no_nulo->tope == 0, "tope correcto");
	pa2m_afirmar((_no_nulo)->compara== compare, "EL iterador es correcto");
	pa2m_afirmar((_no_nulo)->destruye == destructe, "El destructe es correcto");
	destruir(_no_nulo);
}

void pruebas_insertar(){
	cosa* c1= crear_cosa(1);
	cosa* c2= crear_cosa(2);
	cosa* c3= crear_cosa(3);
	cosa* c5= crear_cosa(5);
	cosa* c7= crear_cosa(7);
	cosa* c8= crear_cosa(7);
	heap_t* _nulo = NULL;
	heap_t* _no_nulo= heap_crear(0,compare, destructe);
	pa2m_afirmar(heap_insertar(_nulo, c1)== ERROR, "No inserta en un  nulo");
	pa2m_afirmar(heap_insertar(_no_nulo, c2)== EXITO, "inserta en un  valido");
	pa2m_afirmar(heap_insertar(_no_nulo, c3)== EXITO && heap_insertar(_no_nulo, c1)== EXITO && heap_insertar(_no_nulo, c5)== EXITO && heap_insertar(_no_nulo, c7)== EXITO && heap_insertar(_no_nulo, c8)== EXITO, "inserta varios elementos en un  valido");
	pa2m_afirmar(_no_nulo->tope==6 , "El tope es correcto");
	imprimir(_no_nulo);
	destruir(_no_nulo);
}


void pruebas_raiz(){
	heap_t* _nulo = NULL;
	pa2m_afirmar(extraer_raiz(_nulo)== NULL, "No hay raiz en  nulo");
	heap_t* _no_nulo= heap_crear(0, compare, destructe);
	cosa* c1= crear_cosa(1);
	cosa* c2= crear_cosa(2);
	cosa* c3= crear_cosa(3);
	heap_insertar(_no_nulo, c1);
	heap_insertar(_no_nulo, c2);
	heap_insertar(_no_nulo, c3);
	extraer_raiz(_no_nulo);
	//pa2m_afirmar(extraer_raiz(_no_nulo)== c3, "La raiz es correcta");
	imprimir(_no_nulo);
	_no_nulo->destruye(_no_nulo->vector[0]);
	destruir(_no_nulo);
}

int main(){
	pa2m_nuevo_grupo("PRUEBAS CREAR");
	pruebas_crear();
	pa2m_nuevo_grupo("PRUEBAS INSERTAR");
	pruebas_insertar();
	pa2m_nuevo_grupo("PRUEBAS RAIZ");
	pruebas_raiz();
	pa2m_mostrar_reporte();
	printf("\n");
	return 0;
}

