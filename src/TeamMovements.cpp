#include "TeamMovements.h"

TeamMovements::TeamMovements()
{}

void TeamMovements::assignMovement(unsigned int player_id, std::string type, std::pair<int, int> value)
{
	move_str new_move;
	new_move.type = type;
	new_move.direction = value.first;
	new_move.steps = value.second;
	this->_moves[player_id] = new_move;
}

std::string TeamMovements::getMovementType(unsigned int player_id)
{
	return (this->_moves[player_id]).type;
}

std::pair<int, int> TeamMovements::getMovementValue(unsigned int player_id)
{
	int i = (this->_moves[player_id]).direction;
	int j = (this->_moves[player_id]).steps;

	return std::make_pair(i, j);
}