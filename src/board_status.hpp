#ifndef BOARD_STATUS
#define BOARD_STATUS

#include <string>
#include <vector>
#include "constants.hpp"
#include <iostream>

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

const std::vector<move> MOVES = {{0, 0, 0}, {1, -1, -1}, {2, -1, 0}, {3, -1, 1}, {4, 0, 1}, {5, 1, 1}, {6, 1, 0}, {7, 1, -1}, {8, 0, -1}};

struct player_move {
	int player_id;
	std::string move_type;
	int dir;
	// Solo sirve para cuando el tipo de movimiento es un pase e indica cuanta
	// fuerza tiene el pase (i.e. cuan lejos llegará el balón si nadie lo intercepta)
	int steps;

	/**
	 * Construye un player move sin setear ninguno de los campos.
	 */
	player_move() {
	}

	/**
	 * Construye un player move con los parametros indicados.
	 */
	player_move(int playerId, std::string moveType, int direction, int stepsCount=-1){
		this->player_id = playerId;
		this->move_type = moveType;
		this->dir = direction;
		this->steps = stepsCount;
	}
};

struct player_status {
	int id;
	int i;
	int j;
	double probability;
	bool in_possession = false;
	std::pair<int, int> old_position;

	player_status() {}
	player_status(int id) : id(id) {}
	player_status(int id, double probability) : id(id), probability(probability) {}
	player_status(int id, int i, int j, bool in_possession)
		: id(id), i(i), j(j), in_possession(in_possession) {}

	player_status(int id, int i, int j, double probability, bool in_possession)
		: id(id), i(i), j(j), probability(probability), in_possession(in_possession) {}
	player_status(const player_status &otro)
		: id(otro.id), i(otro.i), j(otro.j), probability(otro.probability), in_possession(otro.in_possession) {}

	void move(player_move pm) {
		if (pm.player_id == id) {
			if (pm.move_type == PASE) {
				in_possession = false;
			} else {
				old_position = std::make_pair(i, j);
				struct move m(MOVES[pm.dir]);
				i += m.i;
				j += m.j;
			}
		}
	}

	void undoMove() {
		i = old_position.first;
		j = old_position.second;
		old_position = std::make_pair(-1, -1);
	}
};

struct ball_status {
	int i;
	int j;
	int dir;
	int steps;
	bool is_free = false;

	void move(const move& m) {
		if (is_free && steps > 0) {
			i += 2*(m.i);
			j += 2*(m.j);
			--steps;
		}
	}

	std::vector<std::pair<int,int> > trajectory() {
		struct move m(MOVES[dir]);
		std::vector<std::pair<int,int> > ret;
		int actualSteps = 2*steps;
		// este <= en el for esta a proposito!!!1
		for(int k=0; k <= actualSteps; k++) {
			ret.push_back({this->i + k*m.i, this->j + k*m.j});
		}
		return ret;
	}
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
