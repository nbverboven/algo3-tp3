#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"

#ifndef STATIC_PLAYER
#define STATIC_PLAYER


/** 
 * Estructura para representar los pesos que se le asignan
 * a las características evaluadas para puntuar un tablero
*/
struct genome
{
	genome() : ball_possession(1),
	           ball_in_oponent_possession(-1),
	           ball_free(-0.1),
	           goal_distance(-0.4),
	           ball_distance(0.46),
	           oponent_with_ball_distance(-0.5) {}

	genome(double ball_possession,
	       double ball_in_oponent_possession,
	       double ball_free,
	       double goal_distance,
	       double ball_distance,
	       double oponent_with_ball_distance) 
    : ball_possession(ball_possession),
      ball_in_oponent_possession(ball_in_oponent_possession),
      ball_free(ball_free),
      goal_distance(goal_distance),
      ball_distance(ball_distance),
      oponent_with_ball_distance(oponent_with_ball_distance) {}

    double ball_possession;
    double ball_in_oponent_possession;
    double ball_free;

    double goal_distance;
    double ball_distance;
    double oponent_with_ball_distance;
};


class static_player
{
protected:

    int columns, rows, steps;
    std::string team, side;
    std::vector<player> players;
    std::vector<player> oponent_players;
    genome _genome;

public:

	static_player() = default;

	static_player(
		int columns,
		int rows,
		int steps,
		std::string side,
		const std::vector<player>& players, // Este no lo usa para nada
		const std::vector<player>& oponent_players, // Este no lo usa para nada
		genome weights = genome()
	) {
		this->columns = columns;
		this->rows = rows;
		this->steps = steps;
		this->side = side;
		this->team = team;
		this->players = players;
		this->oponent_players = oponent_players;
		this->_genome = weights;
	}

	void starting_positions(std::vector<player_status>& positions) {
		int column = this->columns - 1;
		if (this->side == IZQUIERDA) {
			column = 0;
		}

		for (int i = 0; i < 3; i++) {
			positions.emplace_back(i, i, column, false);
		}
	}

	virtual void make_move(const board_status& current_board, std::vector<player_move>& made_moves) {
		made_moves.clear();
		player_move new_move;

		for (auto& p : current_board.team) {
			new_move.player_id = p.id;
			new_move.move_type = MOVIMIENTO;
			new_move.dir = 0;
			made_moves.push_back(new_move);
		}
	}

	void finish(std::string result) { }

	std::vector<player> getPlayers() {
		return this->players;
	}

	std::vector<player> getOponentPlayers() {
		return this->oponent_players;
	}
};

#endif
