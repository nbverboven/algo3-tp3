#ifndef PLAYER_H
#define PLAYER_H

#include <iostream>
#include <memory>

#include "Ball.h"
#include "Defines.h"

extern const std::vector<std::pair<int,int>> MOVES;

class Player
{
public:

	/**
	 * Constructores
	 */
	Player(unsigned int player_id, float p_quite);
	Player(const Player &p);

	/**
	 * Destructor
	 */
	~Player();

	/**
	 * Ubica la pelota en alguna posición de la cancha. i indica la fila y j la columna
	 */
	void setPosition(int i, int j);

	/**
	 * Avanza en la dirección indicada por move
	 */
	void move(int move);

	/*
	 * Avanza en la dirección opuesta a la indicada por move
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
	 * Toma posesión de la pelota, i.e. la ubica en la posición del jugador
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
	 * Devuelve el id del jugador
	 */
	unsigned int getId() const;

	/**
	 * Devuelve la posición actual del jugador
	 */
	std::pair<int, int> getPosition() const;

	/**
	 * Devuelve un puntero a la pelota
	 */
	Ball* getBall();

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