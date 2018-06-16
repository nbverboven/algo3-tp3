#include "GlottonusPlayer.h"
#include <iostream>
#include "board_status.hpp"
#include "constants.hpp"

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

    if(this->side == IZQUIERDA)
        this->team = A;
    else
        this->team = B;       
}

GlottonusPlayer::~GlottonusPlayer(){}

void GlottonusPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves){
    made_moves.clear();
    player_move new_move;
    

    board_status test_board = current_board;
    test_board.team.clear();

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

    //std::cout << "BoardPoints: " << this->EvaluateBoard(test_board) << std::endl;
}


int GlottonusPlayer::EvaluateBoard(const board_status& board){
    int boardPoints = 0;

    bool inPosetion = false;

    //for (auto& p : board.team) {

    //}

    //if(board.ball.is_free){

    //}
    
    return boardPoints;
};