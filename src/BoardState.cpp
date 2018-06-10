#include "BoardState.h"

BoardState::BoardState(const std::vector<Player> &team_A,
		               const std::vector<Player> &team_B,
		               const Ball &ball)
	: _team_A(team_A), _team_B(team_B)
{
	this->_ball = new Ball(ball);
}

BoardState::~BoardState()
{
	delete this->_ball;
}

std::vector<Player> BoardState::getPlayers(std::string team)
{
	if (team == A) return this->_team_A;
	return this->_team_B;
}

Ball* BoardState::getBall() 
{
	return this->_ball;
}