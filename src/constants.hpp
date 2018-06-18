#ifndef CONSTANTS
#define CONSTANTS

#include <string>
#include <vector>

const std::string A = "A";
const std::string B = "B";

const std::string IZQUIERDA = "IZQUIERDA";
const std::string DERECHA = "DERECHA";

const std::string MOVIMIENTO = "MOVIMIENTO";
const std::string PASE = "PASE";
const std::string IN_POSETION = "CON_PELOTA";
const std::string FREE_PLAYER = "SIN_PELOTA";
const std::string EXIT = "SALIR";
const std::string DONE = "LISTO";

const std::string LOSE = "PERDISTE";
const std::string WIN = "GANASTE";
const std::string TIE = "EMPATARON";

/**
 * Variables de puntaje para evaluar tableros
 */
namespace POINTS {
    //Recordar que mientras menos puntos, mejor

    const int BALL_POSSESSION = 0;
    const int BALL_OPONENT_POSSESSION = 10;
    const int BALL_FREE = 5;

    const int GOAL_DISTANCE = 1;
    const int BALL_DISTANCE = 2;
    const int OPONENT_wBALL_DISTANCE = 3;
}

#endif //CONSTANTS
