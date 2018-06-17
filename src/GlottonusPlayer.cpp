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

std::random_device rd;
std::mt19937 generator(rd());
static std::uniform_int_distribution<int> uid(1,9); // random dice



void GlottonusPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves){   
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
    player_move new_move;
    //Inicializo
    test_board.team.clear();
    made_moves.clear();

    int LIMITE_TESTEO_TABLEROS = 1;

    for(int i=0; i<LIMITE_TESTEO_TABLEROS || newBoardPoints < currentBoardPoints; i++){
        made_moves.clear();

        test_board.ball = current_board.ball;

        //Realizo mis movimientos
        for (auto& p : current_board.team) {
            new_move.player_id = p.id;
            new_move.move_type = MOVIMIENTO;
            new_move.dir =  uid(generator); // use rng as a generator  ; //4;
            made_moves.push_back(new_move);

            player_status jg(p.id);
            jg = p;
            jg.move(new_move);

            //Si el jugador tenia la pelota, la muevo con él
            if(p.in_possession){
                if(new_move.move_type == PASE){
                    test_board.ball.move(MOVES[new_move.dir]);
                }else{
                    test_board.ball.i = jg.i; 
                    test_board.ball.j = jg.j; 
                }
            }
            
            test_board.team.push_back(jg);
        }
        

        //IMPORTANTE: ver que la juagda sea válida con el LogicalBoard
        if(this->team == A)
            this->logicalBoard.makeMove(made_moves, oponent_moves);
        else
            this->logicalBoard.makeMove(oponent_moves, made_moves);
        //TODO: ver en que casos desacemos el movimiento.

        //if(isValidMove){
            newBoardPoints = this->EvaluateBoard(test_board);
        //}
    }

}

/**
 * Setea los movimientos del oponente. 
 * Por ahora, no se mueven.
 */
void GlottonusPlayer::setOponentMoves(const board_status& current_board, std::vector<player_move>& oponent_moves){
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
 * Calcula la distancia entre dos puntos
 */
int distance(const std::tuple<int,int>& t1, const std::tuple<int,int>& t2){
    int x = (std::get<0>(t2) - std::get<0>(t1))^2;
    int y = (std::get<1>(t2) - std::get<1>(t1))^2;
    int distance = sqrt(x - y);
    return distance;
}

/**
 * Evalua el estado del tablero, mientras menos puntos tenga mejor,
 * ( Ej: Si el jugador tiene la pelota y se esta acercando al arco
 * va a tener menos puntos cuando mas se acerque )
 */
int GlottonusPlayer::EvaluateBoard(const board_status& board){
    int boardPoints = 0;

    int dist,points;
    bool inPossession = false;
    std::tuple<int,int> ballPoss(board.ball.i, board.ball.j);
    std::vector<std::pair<int, int>>  goalPoss = this->logicalBoard.getGoal(this->team);

    std::string opTeam = (this->team==A)?B:A;
    std::vector<std::pair<int, int>>  oponentGoalPoss = this->logicalBoard.getGoal(opTeam);

    //Evaluo mi equipo
    for (const player_status& p : board.team) {
        inPossession = inPossession || p.in_possession;
        std::tuple<int,int> playerPoss(p.i, p.j);

        dist = distance(ballPoss, playerPoss);
        boardPoints += dist*POINTS::BALL_DISTANCE; //Notar que si tiene la pelota es 0;

        if(p.in_possession){

            int mejor_dist = -1;
            for(std::tuple<int,int>t : goalPoss){
                dist = distance(t, playerPoss);
                if(mejor_dist == -1 || dist < mejor_dist)
                    mejor_dist = dist;
            }
            boardPoints += mejor_dist*POINTS::GOAL_DISTANCE; //Notar que si entro al arco es 0;

        }else{
            //Evaluo el equipo contrario
            for (const player_status& op : board.oponent_team) {
                if(p.in_possession){
                    std::tuple<int,int> opPlayerPoss(op.i, op.j);

                    dist = distance(opPlayerPoss, playerPoss);
                    boardPoints += dist*POINTS::OPONENT_wBALL_DISTANCE;
                    //Notar que si estoy con mi oponente es 0;
                }
            }
        }
    }

    if(inPossession){
        boardPoints += POINTS::BALL_POSSESSION;
    }else if(board.ball.is_free){
        boardPoints += POINTS::BALL_FREE;
    }else{
        boardPoints += POINTS::BALL_OPONENT_POSSESSION;
    }
    
    return boardPoints;
};