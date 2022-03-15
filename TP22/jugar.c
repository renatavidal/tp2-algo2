#include"jugar.h"
#include "deco.h"
#define FORMATO_POKEMON "%[^;];%i;%i;%i\n"
#define FORMATO_ENTRENADOR "%[^\n]\n"
#define FORMATO_GYM "%[^;];%i;%i \n"
#define FORMATO_INICIAL "%c;"
#define FORMATO_PERSONAJE "%c;%[^\n]\n"
#define NO 'N'
#define SI 'S'
#define MAX_AGREGAR 63
#define MAX_CUALIDAD 128

// CREA UN POKEMON RESERVANDO LA MEMORIA NECESARIA.
pokemon_t* crear_poke(int velocidad, int ataque, int defensa, char nombre[MAX_NOMBRE]){
    pokemon_t* poke = (pokemon_t*)malloc(sizeof(pokemon_t));
    if(poke){
        poke->velocidad = velocidad;
        poke->ritmo = rand()% MAX_CUALIDAD;
        poke->ataque = ataque;
        poke->defensa = defensa;
        poke->altura = rand()% MAX_CUALIDAD;
        poke->puntos_sumados=0;
        strcpy(poke->nombre, nombre);
    }
    return poke;
}

// CREA UN ENTRENADOR RESERVANDO LA MEMORIA NECESARIA.
entrenador_t* crear_entrena(bool es_lider, char nombre[MAX_NOMBRE], lista_t* pokemon){
    if(!pokemon) return NULL;
    entrenador_t* entrenador = (entrenador_t*)malloc(sizeof(entrenador_t));
    if(entrenador){
        entrenador->es_lider = es_lider;
        entrenador->pokemon= pokemon;
        strcpy(entrenador->nombre, nombre);
    }
    return entrenador;
}

// CREA UN GIMNASIO RESERVANDO LA MEMORIA NECESARIA.
gimnasio_t* crear_gym(lista_t* entrenadores, int id_puntero_a_funcion, int dificultad, char nombre[MAX_NOMBRE]){
    if(!entrenadores) return NULL;
    gimnasio_t* gym = (gimnasio_t*)malloc(sizeof(gimnasio_t));
    if(gym){
        gym->entrenadores=entrenadores;
        gym->id_puntero_a_funcion = id_puntero_a_funcion;
        gym->dificultad = dificultad;
        strcpy(gym->nombre, nombre);
    }
    return gym;
}

//COMPRUEBA QUE EL ID SEA UN NUMERO VALIDO (ENTRE 1 Y 5).
int comprobar_id(int id){
    if(id!=1 && id !=2 && id!=3 && id!=4 && id!=5){
        printf("el id no es valido");
        return ERROR;
    } 
    return EXITO;
}
//INICIALIZA LOS CAMPOS DE UN POKEMON.
pokemon_t inicializar_poke(){
    pokemon_t pokemon;
    strcpy(pokemon.nombre, "NULO");
    pokemon.velocidad=0;
    pokemon.ataque=0;
    pokemon.defensa=0;
    return pokemon;
}

//INICIALIZA LOS CAMPOS DE UN GIMNASIO.
gimnasio_t inicializar_gym(){
    gimnasio_t gimnasio;
    strcpy(gimnasio.nombre, "NULO");
    gimnasio.entrenadores=NULL;
    gimnasio.dificultad=0;
    gimnasio.id_puntero_a_funcion=0;
    return gimnasio;
}

//INICIALIZA LOS CAMPOS DE UN ENTRENADOR.
entrenador_t inicializar_entrenador(){
    entrenador_t entrenador;
    entrenador.pokemon= NULL;
    strcpy(entrenador.nombre, "NULO");
    entrenador.es_lider=false;
    return entrenador;
}
// LEE LOS DATOS DEL GIMNASIO Y SI LE SIGUE UN LIDER.
gimnasio_t lee_archivo(FILE* archivo_gym, bool* leyo_mal, char* inicial, int* leidos){
    *leyo_mal= false;
    if(!archivo_gym) *leyo_mal= true;
    gimnasio_t gimnasio;
    int caracteristicas=fscanf(archivo_gym, FORMATO_GYM,gimnasio.nombre, &gimnasio.dificultad, &gimnasio.id_puntero_a_funcion);
    if(caracteristicas!=3) *leyo_mal= true;
    if(comprobar_id(gimnasio.id_puntero_a_funcion)== ERROR) *leyo_mal= true;
    *leidos=fscanf(archivo_gym,FORMATO_INICIAL,inicial);
    if(*inicial != INICIAL_LIDER) *leyo_mal= true;
    return gimnasio;
    
}

// LEE LAS CARACTERISTICAS DE UN ENTRENADOR Y CHEQUEA SI LE SIGUE UN POKEMON.
entrenador_t lee_entrenador(FILE* archivo_gym, bool* leyo_mal, char* inicial, int* leidos){
    *leyo_mal= false;
    if(!archivo_gym) 
        *leyo_mal= true;
    if(*inicial != INICIAL_ENTRENADOR && *inicial != INICIAL_LIDER) *leyo_mal= true;
    entrenador_t entrenador;
    strcpy(entrenador.nombre, "NULO");
    entrenador.es_lider=false;
    int caracteristicas=fscanf(archivo_gym,FORMATO_ENTRENADOR, entrenador.nombre);
    if(caracteristicas!=1)
        *leyo_mal= true;
    *leidos=fscanf(archivo_gym,FORMATO_INICIAL,inicial);
    if(*inicial != INICIAL_POKEMON) *leyo_mal= true;
    return entrenador;
}


// LEE LAS CARACTERISTICAS DE UN POKEMON Y SI EXISTE LO INSERTA EN UNA LISTA.
bool lee_pokemon(FILE* archivo_gym,  lista_t* lista_pokemon){
    if(!archivo_gym || !lista_pokemon) return true;
    pokemon_t pokemon =inicializar_poke();
    int caracteristicas=fscanf(archivo_gym, FORMATO_POKEMON, pokemon.nombre,&pokemon.velocidad,&pokemon.ataque,&pokemon.defensa);
    if(caracteristicas!=4){
        lista_destruir(lista_pokemon);
        return true;
    } 
    void* pokemon_void = (void*)crear_poke(pokemon.velocidad, pokemon.ataque, pokemon.defensa, pokemon.nombre);
    lista_insertar(lista_pokemon, pokemon_void);
    return false;    
}
void liberar_todo(juego_t* juego){
    if(!juego) return;
    if(juego->gimnasios)
        heap_destruir(juego->gimnasios);
    if(juego->personaje.pokemones_obtenidos)
        lista_destruir(juego->personaje.pokemones_obtenidos);
    if(juego->personaje.pokemones_combate)
        lista_destruir(juego->personaje.pokemones_combate);
}

int cargar_gimnasio(juego_t* juego, char* ruta_archivo, comparador compare, destructor_gym destruir, destructor destructor_entrena, destructor destructor_poke){
	if(!juego || !ruta_archivo ) return ERROR;
	entrenador_t entrenador=inicializar_entrenador();
	gimnasio_t gimnasio=inicializar_gym();
    char inicial;
    bool leyo_mal= false;
	FILE* archivo_gym= fopen(ruta_archivo, "r");
	if(!archivo_gym) return ERROR;
    int leidos=fscanf(archivo_gym, FORMATO_INICIAL,&inicial);
    if(leidos!=1 || inicial!= INICIAL_GYM){
    	fclose(archivo_gym);
    	return ERROR;
    }
    if(!juego->gimnasios){
	    heap_t* heap=heap_crear(TOPE_INICIAL,compare, destruir);
	    if(!heap){
            fclose(archivo_gym);
            return ERROR;
        } 
	    juego->gimnasios= heap;
	}
    int n=0;
    int primero=0;
	while(leidos==1 && !leyo_mal && inicial== INICIAL_GYM){
        gimnasio=lee_archivo(archivo_gym, &leyo_mal, &inicial, &leidos);
        lista_t* pila_entrenador= lista_crear(destructor_entrena);
        if(!pila_entrenador) leyo_mal=true;
        if(leyo_mal) lista_destruir(pila_entrenador);
        inicial= INICIAL_ENTRENADOR;
        primero=0;
        while(leidos==1 && !leyo_mal && inicial== INICIAL_ENTRENADOR){
            entrenador= lee_entrenador(archivo_gym, &leyo_mal, &inicial, &leidos);
            if(primero==0) entrenador.es_lider=true;
    		lista_t* lista_pokemon= lista_crear(destructor_poke);
            if(!lista_pokemon) leyo_mal = true;
            if(leyo_mal) lista_destruir(lista_pokemon);
            n=0;
	        while(leidos==1 && !leyo_mal && inicial== INICIAL_POKEMON && n<MAX_COMBATE){
	        	leyo_mal=lee_pokemon(archivo_gym, lista_pokemon);
                if(!leyo_mal){
    	        	leidos=fscanf(archivo_gym,FORMATO_INICIAL,&inicial);
                    juego->cantidad_gym ++;
                    n++;
                }
       		}
            if(!leyo_mal){
    			void* entrenadores= (void*)crear_entrena(entrenador.es_lider, entrenador.nombre, lista_pokemon);
        		lista_apilar(pila_entrenador, entrenadores);  
            }
            primero++;
       	}  
        if(pila_entrenador){
            if(lista_vacia(pila_entrenador)){
                leyo_mal= true;
                lista_destruir(pila_entrenador);
            }
        }
        if(!leyo_mal){
        	void* gym= (void*)crear_gym(pila_entrenador, gimnasio.id_puntero_a_funcion, gimnasio.dificultad, gimnasio.nombre);
    		heap_insertar(juego->gimnasios, gym);     
        }
    }
    if (juego->cantidad_gym==0){
        heap_destruir(juego->gimnasios);
        fclose(archivo_gym);
        return ERROR;
    }
    fclose(archivo_gym);
    return EXITO;
}

int cargar_personaje(juego_t* juego, char* ruta_archivo, destructor destructor){
    if(!juego || !ruta_archivo) return ERROR;
    pokemon_t pokemon= inicializar_poke();
    personaje_t personaje;
    char inicial;
    strcpy(personaje.nombre, "NULO");
	FILE* archivo_pokemones= fopen(ruta_archivo, "r");
	if(!archivo_pokemones) return ERROR;
    int leidos=fscanf(archivo_pokemones,FORMATO_PERSONAJE,&inicial, personaje.nombre);
    if(leidos!=2 || inicial!= INICIAL_ENTRENADOR){
    	fclose(archivo_pokemones);
    	return ERROR;
    }
    int caracteristicas=0;
    bool leyo_mal= false;
    juego->personaje= personaje;
    juego->personaje.pokemones_combate=NULL;
    juego->personaje.pokemones_obtenidos=NULL;
    lista_t* poke_pelea=lista_crear(destructor);
    if(!poke_pelea){
    	fclose(archivo_pokemones);
    	return ERROR;
    }
    juego->personaje.pokemones_combate= poke_pelea;
    lista_t* poke_backup=lista_crear(destructor);
    if(!poke_backup){
        fclose(archivo_pokemones);
        free(juego->personaje.pokemones_combate);
        return ERROR;
    }
    juego->personaje.pokemones_obtenidos= poke_backup;
    int i=0;
    leidos=fscanf(archivo_pokemones,FORMATO_INICIAL,&inicial);
	while(i< MAX_COMBATE && !leyo_mal && inicial== INICIAL_POKEMON){
    	caracteristicas=fscanf(archivo_pokemones, FORMATO_POKEMON, pokemon.nombre,&pokemon.velocidad,&pokemon.ataque,&pokemon.defensa);
    	if(caracteristicas==4){ 
    	   void* pokemon_void = crear_poke(pokemon.velocidad, pokemon.ataque, pokemon.defensa, pokemon.nombre);
           void* pokemon_void2 = crear_poke(pokemon.velocidad, pokemon.ataque, pokemon.defensa, pokemon.nombre);
	       lista_insertar(juego->personaje.pokemones_combate, pokemon_void);
           lista_insertar(juego->personaje.pokemones_obtenidos, pokemon_void2);
        }
        else leyo_mal= true;
	    i++;
        leidos=fscanf(archivo_pokemones,FORMATO_INICIAL,&inicial);
	}
	while(!leyo_mal && inicial== INICIAL_POKEMON){
    	leidos=fscanf(archivo_pokemones, FORMATO_POKEMON, pokemon.nombre,&pokemon.velocidad,&pokemon.ataque,&pokemon.defensa);
    	if(leidos==4){
        	void* pokemon_void = crear_poke(pokemon.velocidad, pokemon.ataque, pokemon.defensa, pokemon.nombre);
    	    lista_insertar(juego->personaje.pokemones_obtenidos, pokemon_void);
        } 
        else leyo_mal=true;
        caracteristicas=fscanf(archivo_pokemones,FORMATO_INICIAL,&inicial);
	}
	fclose(archivo_pokemones);
	return EXITO;
}

// DECIDE QUE BATALLA SE REALIZA SEGUN EL ID DEL GYM
int batallen(int num_batalla, void* poke_mio, void* poke_rival){
    if(!poke_mio || !poke_rival) return ERROR;
    if(num_batalla==5)
        return funcion_batalla_5(poke_mio, poke_rival);
    else if(num_batalla==4)
        return funcion_batalla_4(poke_mio, poke_rival);
    else if(num_batalla==3)
        return funcion_batalla_3(poke_mio, poke_rival);
    else if(num_batalla==2)
        return funcion_batalla_2(poke_mio, poke_rival);
    else if(num_batalla==1)
        return funcion_batalla_1(poke_mio, poke_rival);
    else return ERROR;
}
//IMPRIME UN POKEMON Y SUS CARACTERISTICAS.
void imprimir_poke(pokemon_t* pokemon){
    if(!pokemon) return;
    printf("nombre %s, altura %i, ritmo %i, ataque, %i, defensa %i, velocidad %i \n", pokemon->nombre, pokemon->altura, pokemon-> ritmo, pokemon->ataque, pokemon->defensa, pokemon->velocidad);
}
//PREGUNTA SI EL JUGADOR QUIERE SEGUIR JUGANDO.
int sigue_jugando(){
    char respuesta;
    printf("TODAVIA TENES POKEMONES PARA BATALLAR, NO TE RINDAS! QUERES SEGUIR? PRESIONA S PARA SI O N PARA NO\n");
    scanf(" %c", &respuesta);
    while(respuesta != SI && respuesta != NO){
        printf("PRESIONA S PARA SI O N PARA NO EN MAYUSCULA\n");
        scanf(" %c", &respuesta);
    }
    if (respuesta== SI) return EXITO;
    else return ERROR;
}
// SI NO HA LLEGADO A 63 SUMA UN PUNTO A CADA HABILIDAD DEL POKEMON.
void sumar_puntos_poke(pokemon_t* pokemon){
    if(!pokemon) return;
    if((pokemon->puntos_sumados >= MAX_AGREGAR) || !pokemon) return;
    pokemon->velocidad ++; 
    pokemon->ritmo ++; 
    pokemon->ataque ++; 
    pokemon->defensa ++; 
    pokemon->altura ++; 
    pokemon->puntos_sumados++;
}
// CHEQUEA QUE LAS ESTRUCTURAS TENGAN CONTENIDO.
int atajarse(juego_t* juego, gimnasio_t* gym){
    if(!juego || !gym) return ERROR;
    if(!gym->entrenadores)return ERROR;
    if(!juego->personaje.pokemones_combate) return ERROR;
    if(lista_vacia(juego->personaje.pokemones_combate)== true) return ERROR;
    return EXITO;
}
//CAMBIO EL POKEMON QUE USO PARA LA BATALLA E IMPRIMO POR PANTALLA CUAL ES.
void cambio_mi_poke(juego_t* juego,bool* error, bool* perdio, size_t* n, void** poke_mio, size_t cant_batalla ){\
    imprimir_pierde_batalla();
    if((*n)<cant_batalla){
        if(sigue_jugando()==ERROR){
            *perdio=true;
            *n= cant_batalla;
        }
        *poke_mio = lista_elemento_en_posicion(juego->personaje.pokemones_combate, *n);
        if(!poke_mio)  *error=true;
        printf("TU POKEMON\n");
        imprimir_poke((pokemon_t*)*poke_mio);
    }
    (*n)++;
}

int jugar(juego_t* juego,gimnasio_t* gym){
    if(atajarse(juego, gym)== ERROR) return ERROR;
    bool perdio=false;
    bool error= false;
    int cant_rivales=0;
    entrenador_t* entrenador;
    void* poke_rival;
    void* poke_mio;
    void* ultimo_poke;
    void* ultimo_entrenador;
    size_t cant_batalla=lista_elementos(juego->personaje.pokemones_combate);
    if(cant_batalla>MAX_COMBATE || cant_batalla<0) return ERROR;
    if(comprobar_id(gym->id_puntero_a_funcion)== ERROR) return ERROR;
    poke_mio=lista_elemento_en_posicion(juego->personaje.pokemones_combate, 0);
    printf("TU POKEMON\n");
    imprimir_poke((pokemon_t*)poke_mio);
    while(gym->entrenadores && !error && !perdio){
        size_t n=1;
        ultimo_entrenador= lista_primero(gym->entrenadores);
        entrenador= (entrenador_t*)lista_ultimo(gym->entrenadores);
        cant_rivales=(int)lista_elementos(entrenador->pokemon);
        if(!poke_mio || !entrenador || !entrenador->pokemon|| cant_rivales>MAX_COMBATE || cant_rivales<0) error=true;
        for(int i=0; i< cant_rivales; i++){
            if(!error){
                ultimo_poke= lista_ultimo(entrenador->pokemon);
                poke_rival=lista_elemento_en_posicion(entrenador->pokemon, (size_t)i);
                if(poke_rival){
                    printf("POKEMON RIVAL\n");
                    imprimir_poke((pokemon_t*)poke_rival);
                    while(batallen(gym->id_puntero_a_funcion, poke_mio, poke_rival)!=1 && n<=cant_batalla && !error){
                       cambio_mi_poke(juego, &error, &perdio, &n, &poke_mio, cant_batalla);
                    }
                    if(n <= cant_batalla){
                        imprimir_gana_batalla();
                        sumar_puntos_poke((pokemon_t*)poke_mio);
                    }
                    else if (n>cant_batalla){
                        juego->gym_actual= gym;
                        error=true;
                        perdio= true;
                    }
                    if(!error){
                        if(poke_rival== ultimo_poke && entrenador== ultimo_entrenador)
                            error=true;
                    }
                    if(!error){
                        if(sigue_jugando()==ERROR){
                            imprimir_perder();
                            perdio=true;
                            juego->gym_actual= gym;
                            error=true;
                        }
                    }
                }
            }
        }
        if(!perdio ){
            if(entrenador!= ultimo_entrenador){
                lista_desapilar(gym->entrenadores);
                if(lista_vacia(gym->entrenadores)){
                    free(gym->entrenadores);
                    gym->entrenadores=NULL;
                } 
            }
        }
    }
    juego->gym_actual= gym;
    if(entrenador!= ultimo_entrenador){
        if(!perdio){
            juego->gimnasios->destruir(gym);
            gym=NULL;
        }
        if(heap_vacio(juego->gimnasios)) juego->gimnasios=NULL;
    }
    if (!perdio) return EXITO;
    return ERROR;
}


int simular_jugada(juego_t* juego){
    if(!juego || !juego->gimnasios) return ERROR;
    if(!juego->personaje.pokemones_combate) return ERROR;
    if(lista_vacia(juego->personaje.pokemones_combate)== true) return ERROR;
    bool perdio=false;
    bool error= false;
    int cant_rivales=0;
    entrenador_t* entrenador;
    void* poke_rival;
    void* poke_mio;
    size_t cant_batalla=lista_elementos(juego->personaje.pokemones_combate);
    if(cant_batalla>MAX_COMBATE || cant_batalla<0) return ERROR;
    gimnasio_t* gym= (gimnasio_t*)heap_extraer_raiz(juego->gimnasios);
    if(!gym) return ERROR;
    if(comprobar_id(gym->id_puntero_a_funcion)== ERROR) return ERROR;
    poke_mio=lista_elemento_en_posicion(juego->personaje.pokemones_combate, 0);
    while(gym && !error){
        while(gym->entrenadores && !error){
            size_t n=1;
            entrenador= (entrenador_t*)lista_ultimo(gym->entrenadores);
            cant_rivales=(int)lista_elementos(entrenador->pokemon);
            if(!poke_mio || !entrenador || !entrenador->pokemon|| cant_rivales>MAX_COMBATE || cant_rivales<0) error=true;
            for(int i=0; i< cant_rivales; i++){
                if(!error){
                    poke_rival=lista_elemento_en_posicion(entrenador->pokemon, (size_t)i);
                    if(poke_rival){
                        while(batallen(gym->id_puntero_a_funcion, poke_mio, poke_rival)!=1 && n<=cant_batalla && !error){
                           if((n)<cant_batalla){
                                poke_mio = lista_elemento_en_posicion(juego->personaje.pokemones_combate, n);
                                if(!poke_mio)  error=true;
                            }
                            n++;
                        }
                        if(n <= cant_batalla){
                            sumar_puntos_poke((pokemon_t*)poke_mio);
                        }
                        else if (n>cant_batalla){
                            imprimir_perder();
                            error=true;
                        }
                    }
                }
            }
            lista_desapilar(gym->entrenadores);
            if(lista_vacia(gym->entrenadores)){
                free(gym->entrenadores);
                gym->entrenadores=NULL;
            }
        }
        if(heap_vacio(juego->gimnasios)){
            free(gym);
            gym=NULL;
        }  
        else{
            free(gym);
            if(!error){
                gym= (gimnasio_t*)heap_extraer_raiz(juego->gimnasios);
                if(!gym) error=true;
            }
        } 
    }
    if(!error) imprimir_ganador();
    liberar_todo(juego);
    if (!perdio) return EXITO;
    return ERROR;
}


//IMPRIME POR PANTALLA LOS NOMBRES DE LOS POKEMONES
bool mostrar_elemento(void* poke, void* contador){
    if(poke && contador)
        printf("pokemon %i: %s \n", (*(int*)contador)++, ((pokemon_t*)poke)->nombre);
    return true;
}

void mostrar_personaje(juego_t juego){
    if(!juego.personaje.pokemones_combate || !juego.personaje.pokemones_obtenidos){
        printf("no hay archivos cargados\n");
        return;
    }
    if(lista_vacia(juego.personaje.pokemones_combate) || lista_vacia(juego.personaje.pokemones_obtenidos)){
        printf("no hay archivos cargados\n");
        return;
    }
    int contador=0;
    printf("POKEMONES DE COMBATE\n");
    lista_con_cada_elemento(juego.personaje.pokemones_combate,mostrar_elemento, (void*)&contador);
    contador=0;
    printf("POKEMONES OBTENIDOS\n");
    lista_con_cada_elemento(juego.personaje.pokemones_obtenidos,mostrar_elemento, (void*)&contador);
}

void informacion_gym(juego_t juego){
    if(heap_vacio(juego.gimnasios)){
        printf("no hay archivos cargados\n");
        return;
    }
    gimnasio_t* gym = (gimnasio_t*)heap_raiz(juego.gimnasios);
    printf("nombre del gym %s, dificultad %i, batalla que se realiza %i \n", gym->nombre, gym->dificultad, gym->id_puntero_a_funcion);
}

void tomar_pokemon(juego_t* juego){
    if(!juego) return;
    char respuesta;
    if(heap_vacio(juego->gimnasios)){
        printf("no hay archivos cargados\n");
        return;
    }
    gimnasio_t* gym = (gimnasio_t*)heap_raiz(juego->gimnasios);
    if(!gym){
        printf("NO HAY MAS GYM!\n");
        return;
    } 
    if(lista_vacia(gym->entrenadores) || lista_vacia(juego->personaje.pokemones_obtenidos)){
        printf("no hay archivos cargados\n");
        return;
    }
    entrenador_t* entrenador= lista_ultimo(gym->entrenadores);
    if(!entrenador) return;
    int cantidad=(int)lista_elementos(entrenador->pokemon) -1;
    if(cantidad<0) return;
    printf("POKEMONES DEL RIVAL\n");
    int contador=0;
    lista_con_cada_elemento(entrenador->pokemon,mostrar_elemento, (void*)&contador);
    printf("DESEA TOMAR ALGUN POKEMON? INGRESE S PARA SI O N PARA NO \n");
    scanf(" %c",&respuesta);
    while(respuesta != NO && respuesta != SI){
        printf("INGRESE S PARA SI O N PARA NO, EN MAYUSCULA Y LUEGO UN ENTER\n");
        scanf(" %c",&respuesta);
    }
    if (respuesta== NO) return;
    else{
        size_t ubi_sacar=0;
        printf("INGRESE EL NUMERO DEL POKEMON A TOMAR\n");
        scanf("%zu", &ubi_sacar);

        while(ubi_sacar <0 || ubi_sacar> cantidad){
            printf("INGRESE EL NUMERO VALIDO DEL POKEMON A TOMAR, NO PUEDE SER MAYOR QUE LA CANTIDAD %i NI MENOR QUE 0\n", cantidad);
            scanf("%zu", &ubi_sacar);
        }
        pokemon_t* poke_sacar=(pokemon_t*)lista_elemento_en_posicion(entrenador->pokemon, ubi_sacar);
        void* poke_insertar=crear_poke(poke_sacar->velocidad, poke_sacar->ataque, poke_sacar->defensa, poke_sacar->nombre);
        lista_insertar(juego->personaje.pokemones_obtenidos, poke_insertar);
        mostrar_personaje(*juego);
    }
}

//PREGUNTA SI SE QUIERE CAMBIAR UN POKEMON Y VERIFICA QUE LA RESPUESTA SEA S O N.
void preguntar_cambio(char* respuesta){
    printf("DESEA CAMBIAR ALGUN POKEMON? INGRESE S PARA SI O N PARA NO \n");
    scanf(" %c",respuesta);
    while(*respuesta != NO && *respuesta != SI){
        printf("INGRESE S PARA SI O N PARA NO, EN MAYUSCULA Y LUEGO UN ENTER\n");
        scanf(" %c",respuesta);
    }
}

void cambiar_pokemon(juego_t* juego){
    if(!juego) return;
    if(!juego->personaje.pokemones_combate || !juego->personaje.pokemones_obtenidos){
        printf("no hay archivos cargados\n");
        return;
    }
    if(lista_vacia(juego->personaje.pokemones_combate) || lista_vacia(juego->personaje.pokemones_obtenidos)){
        printf("no hay archivos cargados\n");
        return;
    }
    mostrar_personaje(*juego);
    char respuesta;
    int cantidad_sacar=(int)lista_elementos(juego->personaje.pokemones_combate) -1;
    int cantidad_poner=(int)lista_elementos(juego->personaje.pokemones_obtenidos) -1;
    preguntar_cambio(&respuesta);
    if (respuesta== NO) return;
    else{
        size_t ubi_sacar=0;
        size_t ubi_poner=0;
        printf("INGRESE EL NUMERO DEL POKEMON DE BATALLA A CAMBIAR\n");
        scanf("%zu", &ubi_sacar);
        while(ubi_sacar <0 || ubi_sacar> cantidad_sacar){
            printf("INGRESE EL NUMERO VALIDO DEL POKEMON A CAMBIAR, NO PUEDE SER MAYOR QUE LA CANTIDAD (POR EJEMPLO SI VA DE 0 A 30 ES 30 LA CANTIDAD) NI MENOR QUE 0\n");
            scanf("%zu", &ubi_sacar);
        }
        printf("INGRESE EL NUMERO DEL POKEMON A AGREGAR\n");
        scanf("%zu", &ubi_poner);
        while(ubi_poner <=cantidad_sacar || ubi_poner> cantidad_poner){
            printf("INGRESE EL NUMERO VALIDO DEL POKEMON A AGREGAR, NO PUEDE SER MAYOR QUE LA CANTIDAD (POR EJEMPLO SI VA DE 6 A 30 ES 30 LA CANTIDAD) NI MENOR QUE %i (QUE ES LA CANTIDAD DE POKEMONES DE BATALLA)\n", cantidad_sacar);
            scanf("%zu", &ubi_poner);
        }
        pokemon_t* poke_agregar=(pokemon_t*)lista_elemento_en_posicion(juego->personaje.pokemones_obtenidos, ubi_poner);
        printf("pokemon a agregar %s \n", (poke_agregar)->nombre);
        pokemon_t* poke_insertar=crear_poke(poke_agregar->velocidad, poke_agregar->ataque, poke_agregar->defensa, poke_agregar->nombre);
        pokemon_t* poke_insertar2=crear_poke(poke_agregar->velocidad, poke_agregar->ataque, poke_agregar->defensa, poke_agregar->nombre);
        pokemon_t* poke_sacar=(pokemon_t*)lista_elemento_en_posicion(juego->personaje.pokemones_combate, ubi_sacar);
        printf("pokemon a sacar %s \n", (poke_sacar)->nombre);
        pokemon_t* poke_eliminar=crear_poke(poke_sacar->velocidad, poke_sacar->ataque, poke_sacar->defensa, poke_sacar->nombre);
        if(lista_borrar_de_posicion(juego->personaje.pokemones_combate, ubi_sacar)==-1) return;
        if(lista_borrar_de_posicion(juego->personaje.pokemones_obtenidos, ubi_poner)==-1) return;
        if(lista_borrar_de_posicion(juego->personaje.pokemones_obtenidos, ubi_sacar)==-1) return;// ACA ESTA EL TEMA
        if(lista_insertar_en_posicion(juego->personaje.pokemones_combate, poke_insertar, ubi_sacar)==-1) return;
        if(lista_insertar_en_posicion(juego->personaje.pokemones_obtenidos, poke_eliminar, ubi_poner-1)==-1) return;
        if(lista_insertar_en_posicion(juego->personaje.pokemones_obtenidos, poke_insertar2, ubi_sacar)==-1) return;
        mostrar_personaje(*juego);
    }
    preguntar_cambio(&respuesta);
    if(respuesta== SI) cambiar_pokemon(juego);
}