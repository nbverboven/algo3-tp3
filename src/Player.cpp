#include "Player.h"

Player::Player(unsigned int player_id, float p_quite)
	: _id(player_id), _p_quite(p_quite), _i(-1), _j(-1), _ball(nullptr)
{
	this->_old_position = std::make_pair(-1, -1);
}

void Player::setPosition(int i, int j)
{
	this->_i = i;
	this->_j = j;
}

void Player::move(int move)
{
	this->_old_position.first = this->_i;
	this->_old_position.second = this->_j;
	setPosition(this->_i + MOVES[move].first, this->_j + MOVES[move].second);
	moveBall();
}

void Player::backwardMove(int move)
{
	this->_old_position.first = this->_i;
	this->_old_position.second = this->_j;
	setPosition(this->_i - MOVES[move].first, this->_j - MOVES[move].second);
	moveBall();
}

void Player::undoMove()
{
	this->_i = this->_old_position.first;
	this->_j = this->_old_position.second;
	moveBall();

	/* solo guarda una jugada, si quieren deshacer mas, 
	   se rompe a proposito */
	this->_old_position = std::make_pair(-1, -1);
}

void Player::moveBall()
{
	if (this->_ball != nullptr)
	{
		(this->_ball)->setPosition(this->_i, this->_j);
	}
}

void Player::takeBall(Ball &ball)
{
	this->_ball = &ball;
	(this->_ball)->setMovement(std::make_pair(-1, -1));
	moveBall();
}

void Player::loseBall()
{
	this->_ball = nullptr;
}

float Player::probQuite() const
{
	return this->_p_quite;
}

std::ostream &operator<<(std::ostream &os, const Player &p)
{
	os << "(" << p._id << ", " << p._i << ", " << p._j;

	if (p._ball != nullptr)
	{
		os << ", " << IN_POSSESSION;
	}
	else
	{
		os << ", " << FREE_PLAYER;
	}

	os << ")";

	return os;
}