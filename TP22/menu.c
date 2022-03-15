#include "batallas.h"
#include "deco.h"
#include "jugar.h"
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#define INGRESAR_PERSONAJE 'E'
#define AGREGAR_GYM 'A'
#define COMENZAR_PARTIDA 'I'
#define SIMULAR_PARTIDA 'S'
#define SALIR 'S'
#define MOSTRAR_PERSONAJE 'E'
#define INFO_GYM 'G'
#define CAMBIAR_POKE 'C'
#define PROXIMA_BATALLA 'B'
#define PROXIMO_COMBATE 'N'
#define TOMAR_POKEMON 'T'
#define REINTENTAR_GYM 'R'
#define FINALIZAR_PARTIDA 'F'
#define MAX_ARCHIVO 75

void menu_batalla(juego_t* juego); 

void destruir_p(pokemon_t* pokemon){
    free(pokemon);
}
void destructor_poke(void* elemento){
    if(!elemento)
        return;
    destruir_p((pokemon_t*)elemento);
}
void destruir_e(entrenador_t* entrenador){
    free(entrenador);
}
void destructor_entrenador(void* elemento){
	entrenador_t* aux= (entrenador_t*)elemento;
    if(!aux || !aux->pokemon)
        return;
    lista_destruir(aux->pokemon);
    free(aux);
}

void destructor_gimnasio(void* gym){
	if(!gym) return;
	gimnasio_t* aux= (gimnasio_t*)gym;
    if(!aux)
    	return;
    else if(!aux->entrenadores)
        return;
    lista_destruir(aux->entrenadores);
    free(aux);
}

int compare(void* elemento1, void* elemento2){
    if(!elemento1 || !elemento2)
        return 0;
    if(((gimnasio_t*)elemento1)->dificultad>((gimnasio_t*)elemento2)->dificultad)
        return 1;
    if(((gimnasio_t*)elemento1)->dificultad<((gimnasio_t*)elemento2)->dificultad)
        return -1;
    return 0;
}

//VERIFICA QUE LETRA SEA UNA DE LAS 4 SOLICITADAS
void verificar_letra(char* letra, char primera, char segunda, char tercera, char cuarta){
	while (*letra != primera && *letra != segunda && *letra != tercera && *letra !=cuarta){
	printf("INGRESAR %c, %c, %c, O %c EN MAYUSCULAS \n", primera, segunda, tercera, cuarta);
	scanf(" %c", letra);
	}
}

//VERIFICA QUE LETRA SEA UNA DE LAS 3 SOLICITADAS
void verificar_letra3(char* letra, char primera, char segunda, char tercera){
	while (*letra != primera && *letra != segunda && *letra != tercera){
	printf("INGRESAR %c, %c, O %c EN MAYUSCULAS \n", primera, segunda, tercera);
	scanf(" %c", letra);
	}
}

//MUESTRA LAS OPCIONES DE MOSTRAR PERSONAJE, DAR INFO DEL GYM, CAMBIAR LOS POKEMONES O IR A LA PROXIMA BATALLA.
void menu_gym(juego_t* juego){
	char letra;
	printf("MOSTRAR_PERSONAJE = E, INFO_GYM= G, CAMBIAR_POKE= C, PROXIMA_BATALLA= B \n");
	scanf(" %c", &letra);
	verificar_letra(&letra, MOSTRAR_PERSONAJE, INFO_GYM, CAMBIAR_POKE, PROXIMA_BATALLA);
	if(letra== MOSTRAR_PERSONAJE){
		if(juego){
			printf("%s\n", juego->personaje.nombre);
			mostrar_personaje(*juego);
		}
		menu_gym(juego);
	}
	else if(letra== INFO_GYM){
		informacion_gym(*juego);
		menu_gym(juego);
	}
	else if(letra== CAMBIAR_POKE){
		cambiar_pokemon(juego);
		menu_gym(juego);
	}
	else if (letra== PROXIMA_BATALLA){
		menu_batalla(juego);
	}
}

//MUESTRA LAS OPCIONES DE INGRESAR LOS DATOS DEL PERSONAJE, INGRESAR UN GIMNASIO NUEVO, COMENZAR LA PARTIDA, O SIMULAR UNA PARTIDA COMPLETA.
void menu_inicio(juego_t* juego, comparador compare, destructor_gym destruir_gym,destructor destructor_poke, destructor destructor_entrenador){
	char letra;
	printf("INGRESAR_PERSONAJE = E, AGREGAR_GYM= A, COMENZAR_PARTIDA= I, SIMULAR_PARTIDA= S \n");
	scanf(" %c", &letra);
	verificar_letra(&letra, INGRESAR_PERSONAJE, AGREGAR_GYM, COMENZAR_PARTIDA, SIMULAR_PARTIDA);
	char ruta_archivo[MAX_ARCHIVO];
	if(letra== INGRESAR_PERSONAJE){
		printf("INGRESE EL NOMBRE DEL ARCHIVO A LEER \n");
		scanf("%s", ruta_archivo);
		if(cargar_personaje(juego, ruta_archivo, destructor_poke)==EXITO)
			menu_inicio(juego,compare,  destruir_gym, destructor_poke, destructor_entrenador);
	}
	else if(letra== AGREGAR_GYM){
		printf("INGRESE EL NOMBRE DEL ARCHIVO A LEER \n");
		scanf("%s", ruta_archivo);
		if(cargar_gimnasio(juego, ruta_archivo, compare,  destruir_gym, destructor_entrenador, destructor_poke)==EXITO)
			menu_inicio(juego,compare,  destruir_gym, destructor_poke, destructor_entrenador);
	}
	else if(letra== COMENZAR_PARTIDA){
		menu_gym(juego);
	}
	else if (letra== SIMULAR_PARTIDA){
		simular_jugada(juego);
	}
}

//DESAPILA EL ULTIMO ENTRENADOR BORRA ESA PILA Y EL GYM VACIO.
void desapilar_ultimo(juego_t* juego){
	gimnasio_t* gym = juego->gym_actual;
	if(!gym) return;
    juego->gimnasios->destruir(gym);
    gym=NULL;
    if(heap_vacio(juego->gimnasios))  gym=NULL;
}

//CUANDO SE HA USADO LA POSIBILIDAD DE TOMAR PRESTADO UN POKEMON SE MUESTRA ESTE MENU QUE PERMITE
// CAMBIAR LOS POKEMONES DE BATALLA, O IR AL PROXIMO COMBATE.
void menu_victoria_modif(juego_t* juego){
	char letra;
	printf("CAMBIAR_POKE= C, PROXIMO_COMBATE= N \n");
	scanf(" %c", &letra);
	while (letra != PROXIMO_COMBATE && letra != CAMBIAR_POKE){
		printf(" INGRESE N PARA IR A LA PROXIMA BATALLA O C PARA CAMBIAR AL POKEMON\n");
		scanf("%c", &letra);
	}
	if(letra== CAMBIAR_POKE){
		cambiar_pokemon(juego);
		menu_victoria_modif(juego);
	}
	else if(letra== PROXIMO_COMBATE){
		desapilar_ultimo(juego);
		if(juego->gimnasios)
			menu_gym(juego);
		else{
			imprimir_ganador();
			liberar_todo(juego);
		}
	}
}

//CUANDO SE GANA MUESTRA LAS OPCIONES DE TOMAR UN POKEMON RIVAL, CAMBIAR LOS POKEMONES DE BATALLA, O IR AL PROXIMO COMBATE.
void menu_victoria(juego_t* juego){
	char letra;
	printf("TOMAR_POKEMON = T, CAMBIAR_POKE= C, PROXIMO_COMBATE= N \n");
	scanf(" %c", &letra);
	verificar_letra3(&letra, TOMAR_POKEMON, CAMBIAR_POKE, PROXIMO_COMBATE);
	if(letra== TOMAR_POKEMON){
		tomar_pokemon(juego);
		menu_victoria_modif(juego);
	}
	else if(letra== CAMBIAR_POKE){
		cambiar_pokemon(juego);
		menu_victoria(juego);
	}
	else if(letra== PROXIMO_COMBATE){
		desapilar_ultimo(juego);
		if(juego->gimnasios)
			menu_gym(juego);
		else{
			liberar_todo(juego);
		 	imprimir_ganador();
		}
	}
}

//CUANDO SE PIERDE MUESTRA LAS OPCIONES DE REINTENTAR EL GIMNASIO, FINALIZAR LA PARTIDA O CAMBIAR LOS POKEMONES DE COMBATE.
void menu_derrota(juego_t* juego){
	char letra;
	printf("REINTENTAR_GYM = R, FINALIZAR_PARTIDA= F,  CAMBIAR_POKE= C \n");
	scanf(" %c", &letra);
	verificar_letra3(&letra, REINTENTAR_GYM, CAMBIAR_POKE, FINALIZAR_PARTIDA);
	if(letra== REINTENTAR_GYM){
		if(jugar(juego, juego->gym_actual)==EXITO) 
			menu_gym(juego);
		else menu_derrota(juego);
	}
	else if(letra== FINALIZAR_PARTIDA){
		juego->gimnasios->destruir(juego->gym_actual);
		liberar_todo(juego);
		//imprimir_perder();
	}
	else if(letra== CAMBIAR_POKE){
		cambiar_pokemon(juego);
		menu_gym(juego);
	}
}

//MUESTRA LA OPCION DE IR A LA PROXIMA BATALLA Y LA INFO DE ESTA.
void menu_batalla(juego_t* juego){
	char letra;
	if(heap_vacio(juego->gimnasios)){
		liberar_todo(juego);
		return;
	}
	printf(" PROXIMO_COMBATE= N O S PARA SALIR\n");
	scanf("%c", &letra);
	while (letra != PROXIMO_COMBATE && letra != SALIR){
		printf(" INGRESE N PARA IR A LA PROXIMA BATALLA O S PARA SALIR EN MAYUSCULAS\n");
		scanf("%c", &letra);
	}
	if(letra== PROXIMO_COMBATE){
		imprimir_batalla();
		gimnasio_t* gym = (gimnasio_t*)heap_extraer_raiz(juego->gimnasios);
		if(!gym){
			imprimir_ganador();
			return;
		}
		int jugada= jugar(juego, gym);
		if(jugada ==1 &&  juego->gimnasios)
			menu_victoria(juego);
		else if(jugada ==1 && !juego->gimnasios){
			imprimir_ganador();
			liberar_todo(juego);
		}
		else if(jugada ==-1)
			menu_derrota(juego);
	}
	else
		liberar_todo(juego);

}

int main(){
	srand((unsigned)time(NULL));
	juego_t juego;
	juego.gimnasios=NULL;
	juego.cantidad_gym=0;
	juego.gym_actual=NULL;
	juego.personaje.pokemones_combate=NULL;
	juego.personaje.pokemones_obtenidos=NULL;
	imprimir_bienvenida();
	printf("BIENVENIDE A LA BATALLA POKEMON, QUE DESEA HACER?\n");
	menu_inicio(&juego, compare, destructor_gimnasio, destructor_poke, destructor_entrenador);
	//cargar_personaje(&juego, "pokemon.txt", destructor_poke);
	//cargar_gimnasio(&juego, "gym_normal.txt", compare,  destructor_gimnasio, destructor_entrenador, destructor_poke);
	//gimnasio_t* gym = (gimnasio_t*)heap_raiz(juego.gimnasios);
	//jugar(&juego, gym);
	//simular_jugada(&juego);
	//tomar_pokemon(&juego);
	//liberar_todo(&juego);
	return 0;
}