#ifndef BOARD_STATUS
#define BOARD_STATUS

#include "constants.hpp"

struct player {
    int id;
    double probability;

    player(int id, double probability) : id(id), probability(probability) {}
};

struct move {
    int number;
    int i;
    int j;

    move(std::initializer_list<int> params) {
        auto it = params.begin();
        this->number = *it; ++it;
        this->i = *it; ++it;
        this->j = *it;
    }
};

const std::vector<move> moves = {{0, -1, -1}, {1, -1, 0}, {2, -1, 1}, {3, 0, 1}, {4, 1, 1}, {5, 1, 0}, {6, 1, -1}, {7, 0, -1}, {8, 0, 0}};

struct player_move {
    int player_id;
    std::string move_type;
    int dir;
    // Solo sirve para cuando el tipo de movimiento es un pase e indica cuanta
    // fuerza tiene el pase (i.e. cuan lejos llegará el balón si nadie lo intercepta)
    int steps; 
};

struct player_status {
    int id;
    int i;
    int j;
    bool in_posetion = false;

    player_status(int id) : id(id) {}
    player_status(int id, int i, int j, bool in_posetion) : id(id), i(i), j(j), in_posetion(in_posetion) {};

    void move(player_move pm){
        if(pm.player_id == id){
            if(pm.move_type == PASE)
                in_posetion = false;

            struct move m(moves[pm.dir]);
            i += m.i;
            j += m.j;
        }
    }

};

struct ball_status {
    int i;
    int j;
    int dir;
    int steps;
    bool is_free = false;
};

struct board_status {
    std::vector<player_status> team;
    std::vector<player_status> oponent_team;
    ball_status ball;

    void clear() {
        team.clear();
        oponent_team.clear();
        ball.is_free = false;
    }
};

#endif //BOARD_STATUS