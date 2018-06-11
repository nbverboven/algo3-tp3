#include "Referee.h"

/**
 * Constructor.
 */
Referee::Referee(int columns, int rows, int steps,
  TeamInterface& team_A, TeamInterface& team_B):team_A_(team_A), team_B_(team_B), steps_(steps) {

    this->logicalBoard_ = LogicalBoard(columns, rows, team_A.getPlayers(), team_B.getPlayers());
}

/**
 * Destructor.
 */
Referee::~Referee() {
}

 /**
  * Corre el partido. Devuelve el ganador.
  */
std::string Referee::runPlay(std::string starting_team) {

    // obtiene las posiciones de los jugadores de A
    std::vector<std::pair<int, int>> positions_A;
    std::vector<Player> players_A = team_A_.getPlayers();
    foreach (Player& p: players_A) {
        positions_A.push_back(p.getPosition());
    }

    // obtiene las posiciones de los jugadores de B
    std::vector<std::pair<int, int>> positions_B;
    std::vector<Player> players_B = team_B_.getPlayers();
    foreach (Player& p: players_B) {
        positions_B.push_back(p.getPosition());
    }

    // reinicia el tablero
    this->logicalBoard_.reset(positions_A, positions_B, starting_team);

    for (int i = 0; i < this->steps_; ++i) {
        // TODO: completar la logica de: ejecutar las jugadas de ambos equipos
        // en base al board_state del tablero y aplicar los dos conjuntos de
        // jugadas realizadas en el tablero logico
    }

    // devuelve el ganador
    return this->logicalBoard_.winner();
}
