#include "Referee.h"

/**
 * Constructor.
 */
Referee::Referee(int columns, int rows, int steps,
        static_player& team_A, static_player& team_B): team_A_(team_A), team_B_(team_B) {

    logicalBoard_ = LogicalBoard(columns, rows, team_A.getPlayers(), team_B.getPlayers());
    steps_ = steps;

}

/**
 * Destructor.
 */
Referee::~Referee() {
}

/**
 * Corre un partido, devuelve un string
 * con el nombre del ganador.
 */
std::string Referee::runPlay(std::string startingTeam) {

    // obtiene las posiciones iniciales de los dos equipos
    // y las usa para resetear el tablero logico
    std::vector<std::pair<int, int>> positionsA = this->getInitialPositionsA();
    std::vector<std::pair<int, int>> positionsB = this->getInitialPositionsB();
    this->logicalBoard_.reset(positionsA, positionsB, startingTeam);

    for (int step = 0; step < this->steps_; ++step) {

        // obtiene las proximas jugadas de A y B
        board_status boardStateA = this->logicalBoard_.getState();
        board_status boardStateB = this->logicalBoard_.getState();
        std::vector<player_move> movesA, movesB;
        this->team_A_.make_move(boardStateA, movesA);
        this->team_A_.make_move(boardStateB, movesB);

        // ejecuta las jugadas de los dos equipos
        std::string starter = this->logicalBoard_.makeMove(movesA, movesB);
        if (starter != "NINGUNO") {
            // si alguno metio gol, resetea el tablero
            std::vector<std::pair<int, int>> positionsA = this->getInitialPositionsA();
            std::vector<std::pair<int, int>> positionsB = this->getInitialPositionsB();
            this->logicalBoard_.startingPositions(positionsA, positionsB, starter);
        }

    }
    // devuelve el equipo ganador
    std::string winner = this->logicalBoard_.winner();
    this->team_A_.finish(winner);
    this->team_B_.finish(winner);
    return winner;
}

/**
 * Funcion auxiliar que devuelve las posiciones iniciales
 * del equipo A, formateadas como las espera LogicalBoard::reset.
 */
std::vector<std::pair<int, int>> Referee::getInitialPositionsA() {
    std::vector<player_status> startingPositionsA;
    std::vector<std::pair<int, int>> positionsA;
    this->team_A_.starting_positions(startingPositionsA);
    for (player_status& p: startingPositionsA) {
        positionsA.push_back(std::make_pair(p.i, p.j));
    }
    return positionsA;
}

/**
 * Funcion auxiliar que devuelve las posiciones iniciales
 * del equipo B, formateadas como las espera LogicalBoard::reset.
 */
std::vector<std::pair<int, int>> Referee::getInitialPositionsB() {
    std::vector<player_status> startingPositionsB;
    std::vector<std::pair<int, int>> positionsB;
    this->team_B_.starting_positions(startingPositionsB);
    for (player_status& p: startingPositionsB) {
        positionsB.push_back(std::make_pair(p.i, p.j));
    }
    return positionsB;
}
