--------------------- TRABAJO PRACTICO NUMERO 2 ----------------------------
En el correspondiente ZIP se adjuntaran la consigna, los TDAS utilizados (lista y heap), la biblioteca utilizada para decoracion, la biblioteca para batallas, la bibilioteca del juego, un pokemon.txt de entrenador con sus pokemones, un gym_normal.txt de gimnasio y el trabajo. 
Para compilarlo usamos la siguiente linea:
    gcc *.c -o juego -g -std=c99 -Wall -Wconversion -Wtype-limits -pedantic -Werror -O0
Para ejecutarlo usamos las siguiente: la primera si solo queremos correr el juego; la segunda si deseamos corroborar que no este perdiendo memoria:
    ./juego
    valgrind --leak-check=full --track-origins=yes --show-reachable=yes ./juego
A continuacion explicare el funcionamiento del trabajo. Principalmente es una busqueda de ser el nuevo maestro pokemon, en esta busqueda se implementan dos TDAS, lista y pila, y lectura de archivos.
Para almacenar los datos de los entrenadores pokemon utilize una pila, ya que se almacena desde el lider hasta el primer entrenador, y luego de debe luchar contra el primer entrenador hasta el lider,
entonces al necesitar algo de tipo LIFO la pila es conveniente.
Para almacenar los pokemones utilice una lista simplemente enlazada, ya que necesito recorrerlos para mostrarle al usuario que pokemones tiene o incluso para mostrar 
los pokemones rivales y considerar la posibilidad de tomar alguno prestado en caso de una victoria. Tanto para los pokemones del personaje principal como para los del
entrenador/lider rival utilice la lista simplemente enlazada.
Para el personaje por comodidad utilice dos listas, los pokemones de combate y los pokemones obtenidos, aunque en los obtenidos esten los de combate, es util tener los de combate aparte
para poder hacerlos luchar mas facilmente.
Para el gimnasio se utiliza el TDA Heap, principalmente por que se establecio esa convencion, y ademas lo encuentro conveniente ya que puedo ordenarlos en nivel de dificultad y 
acceder a la raiz (el mas facil) cuando lo ordeno de forma minimal.
Aclaracion: 
1) en las batallas si hay un empate gana el primero.
2) si lee un entrenador sin pokemones no lo agrega.
3) si lee un gimnasio sin entrenadores no lo agrega.


