#include "LogicalBoard.h"

LogicalBoard::LogicalBoard(int columns, int rows, const std::vector<Player> &team_A, const std::vector<Player> &team_B)
{
	this->_score[A] = 0;
	this->_score[B] = 0;

	for (const Player& p : team_A)
	{
		this->_team_A.push_back(p);
	}

	for (const Player& p : team_B)
	{
		this->_team_B.push_back(p);
	}

	this->_columns = columns;
	this->_rows = rows;

	// Los arcos están fuera de la cancha
	for (int i = 0; i < 3; ++i)
	{
		int goal_row = floor(this->_rows/2) - 1 + i;
		this->_goal_A.push_back(std::make_pair(goal_row, -1));
		this->_goal_B.push_back(std::make_pair(goal_row, this->_columns));
	}

	this->_free_ball = nullptr;
	// this->_last_state = ??;
}

/* Falta hacer */
// bool LogicalBoard::isValidTeamMove(const std::vector<Player> &team, const std::vector<move_str> &moves)
// {
// }

// void LogicalBoard::maketeamMove(std::vector<Player> team, const std::vector<move_str> &moves)
// {
// }
/* fin Falta hacer */

void LogicalBoard::fightBall(Player &player_with_ball, Player &player_without_ball)
{
	// Probabilidad de defensa
	float p_defend_ball = 1 - player_with_ball.probQuite();

	// Probabilidad de ataque
	float p_take_ball = player_without_ball.probQuite();

	// Normalización de la probabilidad de ataque
	p_take_ball = p_take_ball / (p_take_ball + p_defend_ball);

	/* Genero un número al azar entre 0 y 1 de una distribucíón
       uniforme 
       Nota: puede que sea mejor poner esto en otro lado para
       no tener que crear el generador, inicializarlo, etc.
       cada vez que quiero disputar la pelota */
	std::mt19937 rnd_generator;
	rnd_generator.seed(std::random_device()());
	std::uniform_real_distribution<float> distribution(0, 1);

	if (distribution(rnd_generator) <= p_take_ball)
	{
		player_without_ball.takeBall(*(this->_free_ball));
		player_with_ball.loseBall();
	}

	this->_free_ball = nullptr;
}

void LogicalBoard::fairFightBall(Player &p1, Player &p2)
{
	// Probabilidad de que p2 se quede con la pelota
	float p_p2 = p2.probQuite() / (p1.probQuite() + p2.probQuite());

	/* Genero un número al azar entre 0 y 1 de una distribucíón
       uniforme 
       Nota: ver nota en función fightBall */
	std::mt19937 rnd_generator;
	rnd_generator.seed(std::random_device()());
	std::uniform_real_distribution<float> distribution(0, 1);

	if (distribution(rnd_generator) < p_p2)
	{
		p2.takeBall(*(this->_free_ball));
	}
	else
	{
		p1.takeBall(*(this->_free_ball));
	}

	this->_free_ball = nullptr;
}

/* Falta hacer */
// bool LogicalBoard::intercepted()
// {
// }

// void LogicalBoard::makeMove()
// {
// }

// void LogicalBoard::undoMove(board_state_str *last_state)
// {
// }
/* fin Falta hacer */

std::string LogicalBoard::winner()
{
	std::string winner;

	if (this->_score[A] > this->_score[B])
	{
		winner = A;
	}
	else if (this->_score[A] < this->_score[B])
	{
		winner = B;
	}
	else
	{
		winner = "NINGUNO";
	}

	return winner;
}

std::string LogicalBoard::updateScore()
{
	Ball *ball = this->_free_ball;
	std::string res;

	std::pair<int, int> position = ball->getPosition();
	if (std::find(this->_goal_A.begin(), this->_goal_A.end(), position) != this->_goal_A.end())
	{
		this->_score[B] += 1;
		res = A;
	}
	else if (std::find(this->_goal_B.begin(), this->_goal_B.end(), position) != this->_goal_B.end())
	{
		this->_score[A] += 1;
		res = B;
	}
	else
	{
		res = "NINGUNO";
	}

	return res;
}

void LogicalBoard::reset(const std::vector<std::pair<int, int>> &position_A, 
		                 const std::vector<std::pair<int, int>> &position_B,
		                 std::string starting)
{
	startingPositions(position_A, position_B, starting);
	this->_score[A] = 0;
	this->_score[B] = 0;
}

bool LogicalBoard::positionInBoard(unsigned int i, unsigned int j)
{
	return i < this->_rows && j < this->_columns;
}

void LogicalBoard::startingPositions(const std::vector<std::pair<int, int>> &position_A, 
		                             const std::vector<std::pair<int, int>> &position_B,
		                             std::string starting)
{
	for (int i = 0; i < 3; ++i)
	{
		// Si algún jugador tenía la pelota, se la saco
		if (this->_team_A[i].getBall() != nullptr)
		{
			this->_free_ball = this->_team_A[i].getBall();
			this->_team_A[i].loseBall();
		}

		if (this->_team_B[i].getBall() != nullptr)
		{
			this->_free_ball = this->_team_B[i].getBall();
			this->_team_B[i].loseBall();
		}

		// Coloco a los jugadores en las posiciones correctas
		this->_team_A[i].setPosition(position_A[i].first, position_A[i].second);
		this->_team_B[i].setPosition(position_B[i].first, position_B[i].second);
	}

	delete _free_ball;
	this->_free_ball = nullptr;

	// Creo una pelota nueva en el centro de la cancha
	Ball *new_ball = new Ball();
	new_ball->setPosition(floor(this->_rows/2), (this->_columns/2) - 1);

	// Le doy la pelota al jugador que saca y lo pongo en el centro
	if (starting == A)
	{
		this->_team_A[0].takeBall(*new_ball);
		this->_team_A[0].setPosition(floor(this->_rows/2), (this->_columns/2) - 1);
	}
	else
	{
		this->_team_B[0].takeBall(*new_ball);
		this->_team_B[0].setPosition(floor(this->_rows/2), (this->_columns/2) - 1);
	}
}

/* Falta hacer */
// void LogicalBoard::getState()
// {
// }
/* fin Falta hacer */

const std::vector<std::pair<int, int>>& LogicalBoard::getGoal(std::string team) const
{
	const std::vector<std::pair<int, int>> *goal;

	if (team == A)
	{
		goal = &this->_goal_A;
	}
	else
	{
		goal = &this->_goal_B;
	}

	return *goal;
}

std::vector<Player>& LogicalBoard::getTeam(std::string team)
{
	std::vector<Player> *players;
	
	if (team == A)
	{
		players = &this->_team_A;
	}
	else
	{
		players = &this->_team_B;
	}

	return *players;
}

std::ostream& operator<<(std::ostream &os, const LogicalBoard &lb)
{
	os << lb._team_A[0] << " " << lb._team_A[1] << " " << lb._team_A[2] << " ";
	os << lb._team_B[0] << " " << lb._team_B[1] << " " << lb._team_B[2];
	
	if (lb._free_ball != nullptr)
	{
		os << " " << lb._free_ball;
	}

	return os;
}

int main()
{
	/* code */
	return 0;
}