#include "batallas.h"
#include "jugar.h"
#include <stdbool.h>
#include <string.h>

int funcion_batalla_1(void* pkm_1, void* pkm_2){
	if(!pkm_1 || !pkm_2)
        return 0;
    if(((pokemon_t*)pkm_1)->velocidad>((pokemon_t*)pkm_2)->velocidad)
        return 1;
    if(((pokemon_t*)pkm_1)->velocidad<((pokemon_t*)pkm_2)->velocidad)
        return -1;
    else return 1; 
}

int funcion_batalla_2(void* pkm_1, void* pkm_2){
	if(!pkm_1 || !pkm_2)
        return 0;
    if(((pokemon_t*)pkm_1)->defensa>((pokemon_t*)pkm_2)->defensa)
        return 1;
    if(((pokemon_t*)pkm_1)->defensa<((pokemon_t*)pkm_2)->defensa)
        return -1;
    else return 1; 
}

int funcion_batalla_3(void* pkm_1, void* pkm_2){
	if(!pkm_1 || !pkm_2)
        return 0;
    if(((pokemon_t*)pkm_1)->ataque>((pokemon_t*)pkm_2)->ataque)
        return 1;
    if(((pokemon_t*)pkm_1)->ataque<((pokemon_t*)pkm_2)->ataque)
        return -1;
    else return 1; 
}

int funcion_batalla_4(void* pkm_1, void* pkm_2){
	if(!pkm_1 || !pkm_2)
        return 0;
    if(((pokemon_t*)pkm_1)->altura>((pokemon_t*)pkm_2)->altura)
        return 1;
    if(((pokemon_t*)pkm_1)->altura<((pokemon_t*)pkm_2)->altura)
        return -1;
    else return 1; 
}

int funcion_batalla_5(void* pkm_1, void* pkm_2){
	if(!pkm_1 || !pkm_2)
        return 0;
    if(((pokemon_t*)pkm_1)->ritmo>((pokemon_t*)pkm_2)->ritmo)
        return 1;
    if(((pokemon_t*)pkm_1)->ritmo<((pokemon_t*)pkm_2)->ritmo)
        return -1;
    else return 1; 
}


