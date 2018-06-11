#include "Referee.h"
#include "TeamMovements.h"

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

        BoardState* board_state_A = this->logicalBoard_.getState();
        BoardState* board_state_B = this->logicalBoard_.getState();
        TeamMovements moves_A;
        TeamMovements moves_B;
        this->team_A_.move(board_state_A, moves_A);
        this->team_B_.move(board_state_B, moves_B);

        // TODO: esta funcion makeMove del LogicalBoard todavia no esta hecha.
        // aparentemente deberia devolver un 'indicador' de que la jugada termino
        // en gol, para poder resetear a las posiciones iniciales a los jugadores
        this->logicalBoard_.makeMove(moves_A, moves_B);

        delete board_state_A;
        delete board_state_B;
    }

    std::string winner = this.logicalBoard_.winner();
    this.team_A_.finish(winner);
    this.team_A_.finish(winner);

    // devuelve el ganador
    return winner;
}
