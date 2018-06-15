#ifndef BALL_H
#define BALL_H

#include <iostream>
#include <utility>
#include <vector>

#include "Defines.h"

extern const std::vector<std::pair<int,int>> MOVES;

class Ball
{
public:

	/**
	 * Constructores
	 */
	Ball();
	Ball(const Ball &b);

	/**
	 * Indica el próximo movimiento a realizar.
	 * movement es un par (dirección, cantidad de pasos).
	 * Observación: asume que los valores son no negativos y que
	 * 0 <= dirección <= 8.
	 */
	void setMovement(std::pair<int, int> movement);

	/**
	 * Mueve la pelota hacia la nueva posición
	 */
	void move();

	/**
	 * Devuelve la posición en la que quedará la pelota
	 * luego de finalizar su movimiento
	 */
	std::pair<int,int> finalPosition() const;

	/**
	 * Deshace el último movimiento realizado
	 */
	void undoMove();

	/**
	 * Ubica la pelota en alguna posición de la cancha.
	 * i indica la fila y j la columna. Invalida cualquier
	 * movimiento que estuviera realizando
	 */
	void setPosition(int i, int j);

	/**
	 * Devuelve la posición de la pelota como un par (fila, columna)
	 */
	std::pair<int, int> getPosition() const;

	/**
	 * Devuelve la dirección del movimiento que realiza la pelota
	 */
	int getMovementDirection() const;

	friend std::ostream &operator<<(std::ostream &os, const Ball &b);


private:
	// (dirección del movimiento, número de pasos)
	std::pair<int, int> _movement;
	int _i;
	int _j;
};

#endif // BALL_H