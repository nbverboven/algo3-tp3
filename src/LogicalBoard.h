#ifndef LOGICAL_BOARD_H
#define LOGICAL_BOARD_H

#include <algorithm>
#include <iostream>
#include <cassert>
#include <memory>
#include <random>
#include <string>
#include <map>
#include <set>
#include <vector>

#include "constants.hpp"
#include "board_status.hpp"

extern const std::vector<move> MOVES;

class LogicalBoard
{
public:

	LogicalBoard() = default;

	/**
	 * Constructor
	 */
	LogicalBoard(int columns, int rows, const std::vector<player> &team_A, const std::vector<player> &team_B);

	/**
	 * Modifica los equipos y la pelota a la nueva instancia
	 */
	void updateBoard(const board_status& board, const std::string& team);

	/**
	 * Realiza los movimientos de un equipo
	 */
	void makeTeamMove(std::vector<player_status> &team, std::vector<player_move> &moves);

	/**
	 * player_status_without_ball le disputa la posesión de la pelota a player_status_with_ball
	 */
	void fightBall(player_status &p_with_ball, player_status &player_status_without_ball);

	/**
	 * p1 y p2 se disputan la posesión de una pelota que está libre en el campo de juego
	 */
	void fairFightBall(player_status &p1, player_status &p2);

	/**
	 * Devuelve si el jugador curr_state_player_status interceptó la pelota o no
	 * Asume que la pelota está libre y todavia no fue actualizada a su nueva posicion
	 */
	bool intercepted(const player_status &curr_state_player_status, std::string team) const;

	/**
	 * Realiza los movimientos de los equipos
	 */
	std::string makeMove(std::vector<player_move> &moves_A, std::vector<player_move> &team_B);

	/**
	 * Deshace los últimos movimientos realizados
	 */
	void undoMove(board_status last_state);

	/**
	 * Devuelve el nombre del ganador o "NINGUNO" en caso de empate
	 */
	std::string winner();

	/**
	 * Actualiza los puntajes dependiendo de si la pelota está en
	 * alguno de los dos arcos o no. 
	 * Asume que los jugadores no pueden entrar al arco con la pelota
	 */
	std::string updateScore();

	/**
	 * Reinicia el estado del tablero tablero
	 */
	void reset(const std::vector<std::pair<int, int>> &position_A, 
			   const std::vector<std::pair<int, int>> &position_B,
			   std::string starting);

	/**
	 * Ubica a los jugadores en las posiciones iniciales y le da la pelota al equipo que empieza.
	 * Asume que las posiciones son válidas
	 */
	void startingPositions(const std::vector<std::pair<int, int>> &position_A, 
						   const std::vector<std::pair<int, int>> &position_B,
						   std::string starting);

	/**
	 * Devuelve true si el par (i, j) corresponde a una posición válida dentro del campo 
	 * de juego y false en caso contraro
	 */
	bool positionInBoard(int i, int j) const;

	/**
	 * Devuelve el estado actual del tablero
	 */
	board_status getState() const;

	/**
	 * Devuelve las posiciones correspondientes al arco del equipo que se pasa por parámetro
	 */
	const std::vector<std::pair<int, int>>& getGoal(std::string team) const;

	/**
	 * Devuelve la lista de jugadores del equipo pasado como parámetro
	 */
	std::vector<player_status>& getTeam(std::string team);

	/**
	 * Si la pelota está libre, devuelve un puntero a la misma. Si no, devuelve nullptr
	 */
	ball_status getBall();

	friend std::ostream& operator<<(std::ostream &os, const LogicalBoard &lb);

private:
	/** Par (puntaje equipo A, puntaje equipo B) */
	std::map<std::string, unsigned int> _score;

	/** Equipos */
	std::vector<player_status> _team_A;
	std::vector<player_status> _team_B;

	/** Dimensiones del campo de juego */
	int _columns;
	int _rows;

	/** Ubicación de los arcos */
	std::vector<std::pair<int, int>> _goal_A;
	std::vector<std::pair<int, int>> _goal_B;

	/** Estado de la pelota */
	ball_status _ball;

	board_status _last_state;
};

#endif // LOGICAL_BOARD_H
