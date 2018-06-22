#include <string>
#include <random>
#include <vector>
#include <map>

#include "board_status.hpp"
#include "constants.hpp"
#include "LogicalBoard.h"
#include "Util.h"

#ifndef GENERIC_PLAYER
#define GENERIC_PLAYER

class GenericPlayer
{
protected:

    int columns, rows, steps;
    std::string team, side;
    std::vector<player> players;
    std::vector<player> oponent_players;
    genome _genome;
	LogicalBoard logicalBoard;

public:

	GenericPlayer() = default;

	GenericPlayer(
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
		this->logicalBoard = LogicalBoard(columns, rows, players, oponent_players);
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

	void finish(std::string result) { std::string i=result; i.push_back('C'); }

	std::vector<player> getPlayers() {
		return this->players;
	}

	std::vector<player> getOponentPlayers() {
		return this->oponent_players;
	}

    /**
	 * Evalua el estado del tablero, mientras menos puntos tenga mejor,
	 * ( Ej: Si el jugador tiene la pelota y se esta acercando al arco
	 * va a tener menos puntos cuando mas se acerque )
	 */
	double EvaluateBoard(const board_status& board){

		double maxDistance = sqrt(pow(this->columns, 2) + pow(this->rows, 2));
		double boardPoints = 0;

		double dist;
		bool inPossession = false;

		// si la pelota la tiene algun jugador, Elizondo.py no manda
		// la posicion de la pelota en (board.ball.i, board.ball.j),
		// sino que hay que obtenerla de la posicion del jugador que la tiene
		bool someoneHasIt = false;
		std::pair<int,int> ballPoss;
		for (const player_status& p : board.team) {
			if (p.in_possession) {
				ballPoss.first = p.i;
				ballPoss.second = p.j;
				someoneHasIt = true;
			}
		}
		for (const player_status& p : board.oponent_team) {
			if (p.in_possession) {
				ballPoss.first = p.i;
				ballPoss.second = p.j;
				someoneHasIt = true;
			}
		}
		if (!someoneHasIt) {
			ballPoss.first = board.ball.i;
			ballPoss.second = board.ball.j;
		}

		std::vector<std::pair<int, int>> goalPoss = this->logicalBoard.getGoal(this->team);

		std::string opTeam = (this->team==A) ? B : A;
		std::vector<std::pair<int, int>> oponentGoalPoss = this->logicalBoard.getGoal(opTeam);

		//Evaluo mi equipo
		for (const player_status& p : board.team) {
			inPossession = inPossession || p.in_possession;
			std::pair<int,int> playerPoss(p.i, p.j);

			dist = distance(ballPoss, playerPoss);
			dist = (maxDistance - dist) / maxDistance;
			boardPoints += dist*(this->_genome).genic_values[4]; // .ball_distance //Notar que si tiene la pelota es 0;

			if(p.in_possession){

				double mejor_dist = -1;
				for(std::pair<int,int> t : goalPoss) {
					dist = distance(t, playerPoss);
					if(mejor_dist == -1 || dist < mejor_dist) {
						mejor_dist = dist;
					}
				}
				mejor_dist = (maxDistance - mejor_dist) / maxDistance;
				boardPoints += mejor_dist*(this->_genome).genic_values[3]; // .goal_distance; //Notar que si entro al arco es 0;

			}else{
				//Evaluo el equipo contrario
				for (const player_status& op : board.oponent_team) {
					if(op.in_possession){
						std::pair<int,int> opPlayerPoss(op.i, op.j);

						dist = distance(opPlayerPoss, playerPoss);
						dist = (maxDistance - dist) / maxDistance;
						boardPoints += dist*(this->_genome).genic_values[5]; // .oponent_with_ball_distance;
						//Notar que si estoy con mi oponente es 0;
					}
				}
			}
		}

		if(inPossession){
			boardPoints += (this->_genome).genic_values[0];// .ball_possession;
		}else if(board.ball.is_free){
			boardPoints += (this->_genome).genic_values[2];// .ball_free;
		}else{
			boardPoints += (this->_genome).genic_values[1];// .ball_in_oponent_possession;
		}

		// agrego la dispersion como parametro
		double dispersion = 0;
		for (int i = 0; i < 3; ++i) {
			const player_status& p1 = board.team[i];
			std::pair<int, int> p1Poss(p1.i, p1.j);
			for (int j = i+1; j<3; ++j) {
				const player_status& p2 = board.team[j];
				std::pair<int, int> p2Poss(p2.i, p2.j);
				dispersion += distance(p1Poss, p2Poss);
			}
		}
		dispersion = dispersion / (3*maxDistance);

		boardPoints += (this->_genome).genic_values[6] * dispersion;// .dispersion

		// agrego la distancia de la pelota al arco rival (si la tenemos nosotros)
		bool ourBall = false;
		for (const player_status& p: board.team) {
			if (p.in_possession) {
				ourBall = true;
			}
		}
		if (ourBall) {
			double ballDistanceToRivalGoal = 0;
			ballDistanceToRivalGoal = distance(ballPoss, oponentGoalPoss[0]);
			ballDistanceToRivalGoal = std::min(ballDistanceToRivalGoal, distance(ballPoss, oponentGoalPoss[1]));
			ballDistanceToRivalGoal = std::min(ballDistanceToRivalGoal, distance(ballPoss, oponentGoalPoss[2]));
			ballDistanceToRivalGoal = (maxDistance - ballDistanceToRivalGoal) / maxDistance;
			boardPoints += (this->_genome).genic_values[7] * ballDistanceToRivalGoal; //.distance_ball_oponent_goal
		}

		// agrego la distancia de la pelota al arco propio (si la tienen ellos)
		bool theirBall = false;
		for (const player_status& p: board.oponent_team) {
			if (p.in_possession) {
				theirBall = true;
			}
		}
		if (theirBall) {
			double ballDistanceToOurGoal = 0;
			ballDistanceToOurGoal = distance(ballPoss, goalPoss[0]);
			ballDistanceToOurGoal = std::min(ballDistanceToOurGoal, distance(ballPoss, goalPoss[1]));
			ballDistanceToOurGoal = std::min(ballDistanceToOurGoal, distance(ballPoss, goalPoss[2]));
			ballDistanceToOurGoal = (maxDistance - ballDistanceToOurGoal) / maxDistance;
			boardPoints += (this->_genome).genic_values[8] * ballDistanceToOurGoal; //.distance_ball_our_goal
		}
		return boardPoints;
	}
};

#endif
