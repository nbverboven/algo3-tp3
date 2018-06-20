#include "GreedyPlayer.h"
#include <iostream>
#include "board_status.hpp"
#include "constants.hpp"
#include <iostream>

GreedyPlayer::GreedyPlayer(
    int columns,
    int rows,
    int steps,
    std::string side,
    const std::vector<player>& players, // Este no lo usa para nada
    const std::vector<player>& oponent_players, // Este no lo usa para nada
    genome weights
): GenericPlayer(columns, rows, steps, side, players, oponent_players, weights) {

    if(this->side == IZQUIERDA){
        this->team = A;
        this->logicalBoard = LogicalBoard(columns, rows, players, oponent_players);
    }else{
        this->team = B;
        this->logicalBoard = LogicalBoard(columns, rows, oponent_players, players);
    }

}

GreedyPlayer::~GreedyPlayer(){}

std::random_device rd;
std::mt19937 generator(rd());
static std::uniform_int_distribution<int> uid(0,8); // random dice


void GreedyPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves){

    //Update del tablero lógico con los ultimos movimientos
    this->logicalBoard.updateBoard(current_board, this->team);

    //Acá voy a utilizar el tablero lógico para ver que los movimientos sean correctos y validos
    //También mi función de evaluar tablero, para saber si me sirve el movimiento
    //Voy a evaluar el tablero actual y ver si el movimiento que hago me da mejor resultado

    // guarda una copia del tablero original para restaurarlo
    board_status original_board = current_board;

    // guarda la jugada a ejecutar por el rival en 'oponent_moves'
    std::vector<player_move> oponent_moves;
    this->setOponentMoves(original_board, oponent_moves);

    //Inicializo
    made_moves.clear();

    // busco el maximo puntaje entre todos los tableros, incluyendo
    // el actual
    std::vector<player_move> nextMoves;
    for (int i = 0; i < 3; ++i) {
        player_move nullMove;
        nullMove.player_id = i;
        nullMove.move_type = MOVIMIENTO;
        nullMove.dir = 0;
        nextMoves.push_back(nullMove);
    }

    // obtengo los puntos del estado actual
    double maxScore = this->EvaluateBoard(original_board);

    // obtengo la lista de movimientos posibles a ejecutar
    std::vector<std::vector<player_move>> potentialMoves = this->generateMoves(current_board);

    for(std::vector<player_move> currentMoves: potentialMoves) {

        if(this->logicalBoard.isValidTeamMove(original_board.team, currentMoves)) {
            // si es una jugada valida, la ejecuto y calculo el puntaje
            // del tablero resultante
            this->logicalBoard.makeMove(currentMoves, oponent_moves);

            // obtengo el puntaje en el estado resultante de haber ejecutado la jugada
            double currentScore = this->EvaluateBoard(this->logicalBoard.getState());
            if (currentScore > maxScore) {
                // si esta jugada me dio mejor puntaje que la ultima,
                // actualizo el puntaje maximo y la jugada que lo hizo
                maxScore = currentScore;
                nextMoves = currentMoves;
            }

            // al final, deshago la jugada que hice en el tablero logico
            this->logicalBoard.undoMove(original_board);
        }
    }

    // devuelvo como proxima jugada 'nextMoves'
    made_moves = nextMoves;
}

/**
 * Setea los movimientos del oponente.
 * Por ahora, no se mueven.
 */
void GreedyPlayer::setOponentMoves(const board_status& current_board, std::vector<player_move>& oponent_moves){
    oponent_moves.clear();
    player_move new_move;
    for (auto& p : current_board.oponent_team) {
        new_move.player_id = p.id;
        new_move.move_type = MOVIMIENTO;
        new_move.dir = 0;
        oponent_moves.push_back(new_move);
    }
}

/**
 * Genera todos los movimientos posibles a partir del estado
 * actual del equipo.
 */
std::vector<std::vector<player_move>> GreedyPlayer::generateMoves(const board_status& current_board) {
    std::vector<std::vector<player_move>> nextMoves;

    // genera todas las combinaciones de desplazamientos
    // por el tablero
    int movesSize = MOVES.size();

    for (int d1 = 0; d1 < movesSize; ++d1) {
        for (int d2 = 0; d2 < movesSize; ++d2) {
            for (int d3 = 0; d3 < movesSize; ++d3) {
                std::vector<player_move> currentTeamMove;
                currentTeamMove.push_back(player_move(0, MOVIMIENTO, d1));
                currentTeamMove.push_back(player_move(1, MOVIMIENTO, d2));
                currentTeamMove.push_back(player_move(2, MOVIMIENTO, d3));
                nextMoves.push_back(currentTeamMove);
            }
        }
    }

    // arma las jugadas de la forma 'un pase + dos desplazamientos'
    std::vector<int> freePlayers = {0, 1, 2};
    for (const player_status& player: current_board.team) {
        if (player.in_possession) {
            // elimina especificamente el indice "player.id" del vector
            // para hacer que los otros dos jugadores se desplacen
            freePlayers.erase(freePlayers.begin() + player.id);
            for (int stepSize = 1; stepSize < rows/2; ++stepSize) {
                for (int direccionTiro = 0; direccionTiro < movesSize; ++direccionTiro) {
                    for (int d1 = 0; d1 < movesSize; ++d1) {
                        for (int d2 = 0; d2 < movesSize; ++d2) {
                            std::vector<player_move> currentTeamMove;
                            currentTeamMove.push_back(player_move(player.id, PASE, direccionTiro, stepSize));
                            currentTeamMove.push_back(player_move(freePlayers[0], MOVIMIENTO, d1));
                            currentTeamMove.push_back(player_move(freePlayers[1], MOVIMIENTO, d2));
                            nextMoves.push_back(currentTeamMove);
                        }
                    }
                }
            }
        }
    }
    return nextMoves;
}
