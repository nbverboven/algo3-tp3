#ifndef LOGICAL_BOARD_H
#define LOGICAL_BOARD_H

#include <iostream>
#include <vector>
#include <map>
#include <set>
#include <memory>
#include <string>
#include <random>
#include <algorithm>
#include "Ball.h"
#include "Player.h"
#include "TeamMovements.h"
#include "BoardState.h"
#include "Defines.h"

extern const std::vector<std::pair<int,int>> MOVES;

class LogicalBoard
{
public:
	/**
	 * Constructor
	 */
	LogicalBoard(int columns, int rows, const std::vector<Player> &team_A, const std::vector<Player> &team_B);

	bool isValidTeamMove(std::vector<Player> &team, TeamMovements &moves);

	void maketeamMove(std::vector<Player> &team, TeamMovements &moves);

	/**
	 * player_without_ball le disputa la posesión de la pelota a
	 * player_with_ball
	 */
	void fightBall(Player &p_with_ball, Player &player_without_ball);

	/**
	 * p1 y p2 se disputan la posesión de una pelota que está
	 * libre en el campo de juego
	 */
	void fairFightBall(Player &p1, Player &p2);

	/* Este metodo asume fuertemente que la pelota todavia no fue actualizada a su nueva posicion
	y que la pelota esta libre. */
	bool intercepted(const Player &curr_state_player) const;

	/* Falta hacer */
	// void makeMove();
	/* fin Falta hacer */

	void undoMove(BoardState *last_state);

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
	 * Reinicia el estado del tablero tablero.
	 * El i-ésimo elemento de position_A y position_B contiene la
	 * posición de i-ésimo jugador del equipo A y B respectivamente.
	 */
	void reset(const std::vector<std::pair<int, int>> &position_A, 
	           const std::vector<std::pair<int, int>> &position_B,
	           std::string starting);

	/**
	 * Devuelve true si el par (i, j) corresponde a una 
	 * posición válida dentro del campo de juego y false en 
	 * caso contraro
	 */
	bool positionInBoard(std::pair<int, int> pos) const;

	/**
	 * Ubica a los jugadores en las posiciones iniciales y le da la
	 * pelota al equipo que empieza.
	 * El i-ésimo elemento de position_A y position_B contiene la
	 * posición de i-ésimo jugador del equipo A y B respectivamente.
	 * Asume que las posiciones son válidas
	 */
	void startingPositions(const std::vector<std::pair<int, int>> &position_A, 
	                       const std::vector<std::pair<int, int>> &position_B,
	                       std::string starting);

	BoardState* getState() const;

	/**
	 * Devuelve las posiciones correspondientes al arco del equipo 
	 * que se pasa por parámetro
	 */
	const std::vector<std::pair<int, int>>& getGoal(std::string team) const;

	/**
	 * Devuelve la lista de jugadores del equipo pasado como parámetro
	 */
	std::vector<Player>& getTeam(std::string team);

	/**
	 * Si la pelota está libre, devuelve un puntero a la misma.
	 * Si no, devuelve nullptr
	 */
	Ball* getBall();

	friend std::ostream& operator<<(std::ostream &os, const LogicalBoard &lb);

private:
	// Par (puntaje equipo A, puntaje equipo B)
	std::map<std::string, unsigned int> _score;

	// Equipos
	std::vector<Player> _team_A;
	std::vector<Player> _team_B;

	// Dimensiones del campo de juego
	unsigned int _columns;
	unsigned int _rows;

	// Ubicación de los arcos
	std::vector<std::pair<int, int>> _goal_A;
	std::vector<std::pair<int, int>> _goal_B;

	// Puntero a la pelota
	Ball *_free_ball;

	BoardState *_last_state;

};

#endif // LOGICAL_BOARD_H