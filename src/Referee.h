#include "LogicalBoard.h"
#include "static_player.hpp"

#ifndef REFEREE_H
#define REFEREE_H

/**
 * Referi, para correr partidos sin la interfaz grafica de Elizondo.
 */
class Referee {
public:
    /**
     * Constructor.
     */
    Referee(int columns, int rows, int steps,
        static_player& team_A, static_player &team_B
    );

    /**
     * Destructor.
     */
    ~Referee();

    /**
     * Corre un partido, devuelve un string
     * con el nombre del ganador.
     */
    std::string runPlay(std::string startingTeam);

private:

    /**
     * Funcion auxiliar que devuelve las posiciones iniciales
     * del equipo A, formateadas como las espera LogicalBoard::reset.
     */
    std::vector<std::pair<int, int>> getInitialPositionsA();

    /**
     * Funcion auxiliar que devuelve las posiciones iniciales
     * del equipo B, formateadas como las espera LogicalBoard::reset.
     */
    std::vector<std::pair<int, int>> getInitialPositionsB();

    LogicalBoard logicalBoard_;
    static_player& team_A_;
    static_player& team_B_;
    int steps_;
};

#endif
