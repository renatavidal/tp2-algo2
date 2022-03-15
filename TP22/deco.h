#ifndef __DECO_H__
#define __DECO_H__
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <unistd.h>
#define COLOR_MAGENTA "\x1b[45m\x1b[1m"
#define COLOR_CYAN "\x1b[46m\x1b[1m"
#define COLOR_AMARILLO "\x1b[47m\x1b[1m"
#define COLOR_ROJO "\x1b[41m\x1b[1m"
#define COLOR_VERDE "\x1b[42m\x1b[1m"
#define COLOR_RANDOM "\x1b[43m\x1b[1m"
#define COLOR_RANDOM2 "\x1b[44m\x1b[1m"
#define COLOR_NORMAL "\x1b[0m"
#define VERDE "\033[0m\033[32m"
#define CELESTE "\033[1m\033[36m"
#define FIN "\033[0m"
#define AZUL "\033[0m\033[34m"
void imprimir_bienvenida();
void imprimir_ganador();
void imprimir_perder();
void imprimir_batalla();
void imprimir_gana_batalla();
void imprimir_pierde_batalla();

#endif /* __DECO_H__ */