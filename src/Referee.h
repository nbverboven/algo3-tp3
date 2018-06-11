#include "LogicalBoard.h"

#ifndef REFEREE_H
#define REFEREE_H

/**
 * Clase que representa un 'arbitro' que centraliza
 * la toma de decisiones de los movimientos de ambos
 * equipos y la pelota.
 */
class Referee {
public:

    /**
     * Constructor.
     */
    Referee(int columns, int rows, int steps, TeamInterface& team_A, TeamInterface& team_B);

    /**
     * Destructor.
     */
    ~Referee();

    /**
     * Corre el partido. Devuelve el ganador.
     */
    std::string runPlay(std::string starting_team);

protected:
    /** Equipos A y B */
    TeamInterface& team_A_;
    TeamInterface& team_B_;

    /** Tablero logico del referi */
    LogicalBoard logicalBoard_;

    /** Cantidad de pasos del partido */
    int steps_;
};

#endif
