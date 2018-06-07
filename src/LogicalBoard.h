#ifndef LOGICAL_BOARD_H
#define LOGICAL_BOARD_H

#include <iostream>
#include <vector>
#include <map>
#include <memory>
#include <string>
#include <random>
#include <algorithm>
#include "Ball.h"
#include "Player.h"
#include "Defines.h"

extern const std::vector<std::pair<int,int>> MOVES;

class LogicalBoard
{
public:

	/**
	 * Constructor
	 */
	LogicalBoard(int columns, int rows, const std::vector<Player> &team_A, const std::vector<Player> &team_B);

	/* Falta hacer */
	// bool isValidTeamMove(const std::vector<Player> &team, const std::vector<move_str> &moves);
	// void maketeamMove(std::vector<Player> team, const std::vector<move_str> &moves);
	/* fin Falta hacer */	

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

	/* Falta hacer */
	// bool intercepted();
	// void makeMove();
	// void undoMove(board_state_str *last_state);
	/* fin Falta hacer */

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
	bool positionInBoard(unsigned int i, unsigned int j);

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

	/* Falta hacer */
	// void getState();
	/* fin Falta hacer */

	/**
	 * Devuelve las posiciones correspondientes al arco del equipo 
	 * que se pasa por parámetro
	 */
	const std::vector<std::pair<int, int>>& getGoal(std::string team) const;

	/**
	 * Devuelve la lista de jugadores del equipo pasado como parámetro
	 */
	std::vector<Player>& getTeam(std::string team);

	friend std::ostream& operator<<(std::ostream &os, const LogicalBoard &lb);


	/* Ideas para las funciones que involucran mover a los equipos.
	   Puede ser así o pueden ser clases, no sé */
	// struct move_str
	// {
	// 	unsigned int p_id;
	// 	std::string move_type;
	// 	int move;
	// } move_str;

	// struct board_state_str
	// {
		
	// } move_str;

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

	/* TODO: ver cómo hacer
	   Vendría a ser algo para guardar el estado anterior del
	   tablero */
	// _last_state;

};

#endif // LOGICAL_BOARD_H