#ifndef BOARD_STATE_H
#define BOARD_STATE_H

#include <vector>
#include <string>
#include "Player.h"
#include "Ball.h"

class BoardState
{
public:
	BoardState(const std::vector<Player> &team_A,
		       const std::vector<Player> &team_B,
		       const Ball &ball);
	~BoardState();

	std::vector<Player> getPlayers(std::string team);

	Ball* getBall();
	
private:
	std::vector<Player> _team_A;
	std::vector<Player> _team_B;
	Ball *_ball;
};

#endif // BOARD_STATE_H