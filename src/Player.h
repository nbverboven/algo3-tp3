#ifndef PLAYER_H
#define PLAYER_H

#include <memory>
#include <iostream>
#include "Ball.h"
#include "Defines.h"

extern const std::vector<std::pair<int,int>> MOVES;

class Player
{
public:
	/**
	 * Constructor
	 */
	Player(unsigned int player_id, float p_quite);

	/**
	 * Ubica la pelota en alguna posición de la cancha.
	 * i indica la fila y j la columna
	 */
	void setPosition(int i, int j);

	/**
	 * Avanza en la dirección indicada por move.
	 * Los valores están en el rango [0, 1,..., 8]
	 */
	void move(int move);

	/*
	 * Avanza en la dirección opuesta a la indicada por move.
	 * Los valores están en el rango [0, 1,..., 8]
	 */
	void backwardMove(int move);

	/**
	 * Deshace el último movimiento realizado
	 */
	void undoMove();

	/**
	 * El jugador mueve la pelota con él
	 */
	void moveBall();

	/**
	 * Toma posesión de la pelota, i.e. la ubica en la posición
	 * del jugador
	 */
	void takeBall(Ball &ball);

	/**
	 * Indica que el jugador ya no tiene la pelota.
	 * Observación: para evitar la pérdida de la referencia
	 * a la pelota, otro jugador debe tomarla antes o debe 
	 * indicarse que está libre en el campo de juego
	 */
	void loseBall();

	/**
	 * Devuelve la probabilidad de quite
	 */
	float probQuite() const;

	/**
	 * Devuelve un puntero a la pelota. Si el jugador no está 
	 * en posesión de ella, devuelve nullptr
	 */
	Ball* getBall() const;

	friend std::ostream &operator<<(std::ostream &os, const Player &p);

private:
	unsigned int _id;
	float _p_quite;
	std::pair<int, int> _old_position;
	int _i;
	int _j;
	Ball *_ball;	
};

#endif // PLAYER_H