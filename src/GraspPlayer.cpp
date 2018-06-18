#include "GraspPlayer.h"
#include <iostream>
#include "board_status.hpp"
#include "constants.hpp"

GraspPlayer::GraspPlayer(
    int columns,
    int rows,
    int steps,
    std::string side,
    const std::vector<player>& players, // Este no lo usa para nada
    const std::vector<player>& oponent_players, // Este no lo usa para nada
    genome weights
){
    this->steps = steps;
    this->side = side;

    if(this->side == IZQUIERDA){
        this->team = A;
        this->logicalBoard = LogicalBoard(columns, rows, players, oponent_players);
    }else{
        this->team = B;
        this->logicalBoard = LogicalBoard(columns, rows, oponent_players, players);
    }
}

GraspPlayer::~GraspPlayer(){}

std::random_device rd;
std::mt19937 generator(rd());
static std::uniform_int_distribution<int> uid(0,8); // random dice

void GraspPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves){

    //Update del tablero lógico con los movimientos del contrario
    this->logicalBoard.updateBoard(current_board, this->team);

    //Acá voy a utilizar el tablero lógico para ver que los movimientos sean correctos y validos
    //También mi función de evaluar tablero, para saber si me sirve el movimiento
    //Voy a evaluar el tablero actual y ver si el movimiento que hago me da mejor resultado

    //Tablero
    board_status test_board = current_board;
    int currentBoardPoints = this->EvaluateBoard(test_board);
    int newBoardPoints = 0;
    //Movimientos
    std::vector<player_move> oponent_moves;
    this->setOponentMoves(test_board, oponent_moves);
    //Inicializo
    made_moves.clear();

    int LIMITE_TESTEO_TABLEROS = 10;

    for(int i=0; i<LIMITE_TESTEO_TABLEROS || newBoardPoints < currentBoardPoints; i++){

        test_board.ball = current_board.ball;

        while (true) {
            made_moves.clear();
            //Realizo mis movimientos
            for (auto& p : current_board.team) {
                player_move new_move;
                new_move.player_id = p.id;
                new_move.move_type = MOVIMIENTO;
                new_move.dir =  uid(generator); // use rng as a generator  ; //4;
                made_moves.push_back(new_move);
            }

            if(this->logicalBoard.isValidTeamMove(test_board.team, made_moves)) {
                for (int i = 0; i < 3; ++i) {
                    //Si el jugador tenia la pelota, la muevo con él
                    player_status jg = current_board.team[i];
                    jg.move(made_moves[i]);

                    //if(p.in_possession){
                    if(jg.in_possession){
                        if(made_moves[i].move_type == PASE){
                            test_board.ball.move(MOVES[made_moves[i].dir]);
                        }else{
                            test_board.ball.i = jg.i;
                            test_board.ball.j = jg.j;
                        }
                    }
                }
                break;
            }
        }

        //IMPORTANTE: ver que la juagda sea válida con el LogicalBoard
        if(this->team == A)
            this->logicalBoard.makeMove(made_moves, oponent_moves);
        else
            this->logicalBoard.makeMove(oponent_moves, made_moves);

        newBoardPoints = this->EvaluateBoard(test_board);
    }

}

/**
 * Setea los movimientos del oponente.
 * Por ahora, no se mueven.
 */
void GraspPlayer::setOponentMoves(const board_status& current_board, std::vector<player_move>& oponent_moves){
    oponent_moves.clear();
    player_move new_move;
    for (auto& p : current_board.oponent_team) {
        new_move.player_id = p.id;
        new_move.move_type = MOVIMIENTO;
        new_move.dir = 0;
        oponent_moves.push_back(new_move);
    }
}
