#include "Player.h"

Player::Player(unsigned int player_id, float p_quite)
	: _id(player_id), _p_quite(p_quite), _i(-1), _j(-1), _ball(nullptr)
{
	this->_old_position = std::make_pair(-1, -1);
}

Player::~Player()
{
	delete this->_ball;
}

Player::Player(const Player &p)
	: _id(p._id), _p_quite(p._p_quite), _old_position(p._old_position), 
	  _i(p._i), _j(p._j)
{
	this->_ball = new Ball(*(p._ball));
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

unsigned int Player::getId() const
{
	return this->_id;
}

std::pair<int, int> Player::getPosition() const
{
	return std::make_pair(this->_i, this->_j);
}

Ball* Player::getBall()
{
	return this->_ball;
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