#include "GlottonusPlayer.h"
#include <iostream>
#include "board_status.hpp"
#include "constants.hpp"
#include "LogicalBoard.cpp"

GlottonusPlayer::GlottonusPlayer(
    int columns,
    int rows,
    int steps,
    std::string side,
    const std::vector<player>& players, // Este no lo usa para nada
    const std::vector<player>& oponent_players // Este no lo usa para nada
){
    this->columns = columns;
    this->rows = rows;
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

GlottonusPlayer::~GlottonusPlayer(){}

void GlottonusPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves){   
    //Update del tablero lógico con los movimientos del contrario
    this->logicalBoard.updateBoard(current_board, this->team);

    //Acá voy a utilizar el tablero lógico para ver que los movimientos sean correctos y validos
    //También mi función de evaluar tablero, para saber si me sirve el movimiento
    //Voy a evaluar el tablero actual y ver si el movimiento que hago me da mejor resultado

    board_status test_board = current_board;
    int currentBoardPoints = this->EvaluateBoard(test_board);
    test_board.team.clear();
    int newBoardPoints = 0;

    made_moves.clear();
    player_move new_move;

    int LIMITE_TESTEO_TABLEROS = 1;

    for(int i=0; i<LIMITE_TESTEO_TABLEROS || newBoardPoints > currentBoardPoints; i++){
        made_moves.clear();

        //Realizo mis movimientos
        for (auto& p : current_board.team) {
            new_move.player_id = p.id;
            new_move.move_type = MOVIMIENTO;
            new_move.dir = 4;
            made_moves.push_back(new_move);

            player_status jg(p.id);
            jg = p;
            jg.move(new_move);
            test_board.team.push_back(p);
        }

        //IMPORTANTE: ver que la juagda sea válida con el LogicalBoard!

        newBoardPoints = this->EvaluateBoard(test_board);
    }

}


int distance(const std::tuple<int,int> t1, const std::tuple<int,int> t2){
    int x = (std::get<0>(t2) - std::get<0>(t1))^2;
    int y = (std::get<1>(t2) - std::get<1>(t1))^2;
    int distance = sqrt(x - y);
    return distance;
}

int GlottonusPlayer::EvaluateBoard(const board_status& board){
    int boardPoints = 0;

    bool inPossession = false;

    for (auto& p : board.team) {
        
    }

    //if(board.ball.is_free){

    //}
    
    return boardPoints;
};