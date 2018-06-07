#ifndef DEFINES_H
#define DEFINES_H

#define LOSE "PERDISTE"
#define WIN "GANASTE"
#define TIE "EMPATARON"
#define EXIT "SALIR"
#define LISTO "LISTO"

#define MOVIMIENTO "MOVIMIENTO"
#define PASE "PASE"

#define A "A"
#define B "B"
#define IN_POSSESSION "CON_PELOTA"
#define FREE_PLAYER "SIN_PELOTA"

#define LEFT "IZQUIERDA"
#define RIGHT "DERECHA"
// #define SIDE { A: LEFT, B: RIGHT }

const std::vector<float> DEFAULT_QUITES = {0.5, 0.5, 0.5};
const std::vector<float> DEFAULT_GENOME = {1.0, -1.0, 0.5, -0.5, 1.0, -0.8, 0.9, 0.95};
const std::vector<std::pair<int,int>> MOVES = {{0, 0}, {-1, -1}, {-1, 0}, {-1, 1}, {0, 1}, {1, 1}, {1, 0}, {1, -1}, {0, 1}};

#endif // DEFINES_H