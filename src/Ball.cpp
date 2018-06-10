#include "Ball.h"

Ball::Ball()
	: _i(0), _j(0)
{
	this->_movement = std::make_pair(-1, -1);
}

Ball::Ball(const Ball &b)
	: _movement(b._movement), _i(b._i), _j(b._j)
{}

void Ball::setMovement(std::pair<int, int> movement)
{
	this->_movement.first = movement.first;
	this->_movement.second = movement.second;	
}

void Ball::move()
{
	// Veo si hay definido algún movimiento
	if (this->_movement.second > -1)
	{
		std::pair<int, int> move = MOVES[this->_movement.first];
		setPosition(this->_i + move.first, this->_j + move.second);
		--this->_movement.second;
	}
}

std::pair<int, int> Ball::finalPosition() const
{
	std::pair<int, int> move = MOVES[this->_movement.first];
	int steps = 2 * this->_movement.second;

	return std::make_pair(this->_i + steps*move.first, 
		                  this->_j + steps*move.second);
}

void Ball::undoMove()
{
	// Veo si hay definido algún movimiento
	if (this->_movement.second > -1)
	{
		std::pair<int, int> move = MOVES[this->_movement.first];
		setPosition(this->_i - move.first, this->_j - move.second);
		++this->_movement.second;		
	}
}

void Ball::setPosition(int i, int j)
{
	this->_i = i;
	this->_j = j;
}

std::pair<int, int> Ball::getPosition() const
{
	return std::make_pair(this->_i, this->_j);
}

std::ostream &operator<<(std::ostream &os, const Ball &b)
{
	os << "(" << b._i << ", " << b._j;

	if (b._movement.first < 0)
	{
		os << ", " << b._movement.first << ", " << b._movement.second;
	}

	os << ")";

	return os;
}