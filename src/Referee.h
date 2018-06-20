#include "LogicalBoard.h"
#include "GenericPlayer.hpp"

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
        GenericPlayer& team_A, GenericPlayer &team_B
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

	/**
	 * Devuelve la cantidad de goles que metió el equipo
	 */
	unsigned int getTeamScore(std::string team);

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
    GenericPlayer& team_A_;
    GenericPlayer& team_B_;
    int steps_;
};

#endif
