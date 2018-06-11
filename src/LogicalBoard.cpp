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
	this->_last_state = nullptr;
}

bool LogicalBoard::isValidTeamMove(std::vector<Player> &team, TeamMovements &moves)
{
	bool valid = true;

	// Exactamente un movimiento por jugador del equipo
	for (Player &p : team)
	{
		valid = valid && moves.hasMovementAssigned(p.getId());
	}

	// Muevo a los jugadores
	for (Player &p : team)
	{
		if (moves.getMovementType(p.getId()) == MOVIMIENTO)
		{
			p.move(moves.getMovementValue(p.getId()).first);
		}
		else if (p.getBall() == nullptr)
		{
			// Quiere pasar la pelota pero no la tiene
			valid = false;
		}
	}

	std::set<std::pair<int, int>> final_positions;
	
	for (Player &p : team)
	{
		final_positions.insert(p.getPosition());

		// Todos los jugadores deben estar dentro de la cancha
		valid = valid && positionInBoard(p.getPosition());
	}

	// Dos jugadores del mismo equipo no están en la misma posición
	valid = valid && (final_positions.size() == team.size());

	// Deshago los movimientos
	for (Player &p : team)
	{
		p.undoMove();
	}

	return valid;
}

void LogicalBoard::maketeamMove(std::vector<Player> &team, TeamMovements &moves)
{
	for (Player &p : team)
	{
		if (moves.getMovementType(p.getId()) == MOVIMIENTO)
		{
			p.move((moves.getMovementValue(p.getId())).first);
		}

		/* Si el jugador pasó la pelota se setea la dirección y fuerza y se pierde 
		   la posesión, luego el tablero detecta la pelota libre y la mueve en cada paso */
		if (moves.getMovementType(p.getId()) == PASE)
		{
			this->_free_ball = p.getBall();
			this->_free_ball->setMovement(moves.getMovementValue(p.getId()));
			p.loseBall();
		}
	}
}

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

bool LogicalBoard::intercepted(const Player &curr_state_player) const
{
	bool result = true;

	// Buscar el estado anterior del jugador
	Player *prev_state_player = nullptr;

	for (Player &p : (this->_last_state)->getPlayers(A))
	{
		if (p.getId() == curr_state_player.getId())
		{
			prev_state_player = &p;
		}
	}

	for (Player &p : (this->_last_state)->getPlayers(B))
	{
		if (p.getId() == curr_state_player.getId())
		{
			prev_state_player = &p;
		}
	}

	// Si se movio, no la interceptó
	result = result && (prev_state_player->getPosition() == curr_state_player.getPosition());

	// Si está en el camino, la intercepta
	prev_state_player->backwardMove((this->_free_ball)->getMovementDirection());
	result = result && (prev_state_player->getPosition() == (this->_free_ball)->getPosition());
	prev_state_player->undoMove();

	return result;
}

std::string LogicalBoard::makeMove(TeamMovements &moves_A, TeamMovements &moves_B)
{
	delete this->_last_state;
	this->_last_state = getState();
	maketeamMove(this->_team_A, moves_A);
	maketeamMove(this->_team_B, moves_B);

	// El balón se mueve en la dirección indicada por el último pase
	if (this->_free_ball != nullptr)
	{
		// Ver que el pase es válido
		std::pair<int, int> ball_final_position = (this->_free_ball)->finalPosition();
		bool ball_in_board = positionInBoard(ball_final_position);
		bool ball_in_goal_A = find(this->_goal_A.begin(), this->_goal_A.end(), ball_final_position) != this->_goal_A.end();
		bool ball_in_goal_B = find(this->_goal_B.begin(), this->_goal_B.end(), ball_final_position) != this->_goal_B.end();
		assert(ball_in_board || ball_in_goal_A || ball_in_goal_B);

		// Mira si alguien interceptó la pelota
		std::vector<Player*> intercepters;

		for (Player &p : this->_team_A)
		{
			if (intercepted(p))
			{
				Player *p_ptr = &p;
				intercepters.push_back(p_ptr);
			}
		}

		for (Player &p : this->_team_B)
		{
			if (intercepted(p))
			{
				Player *p_ptr = &p;
				intercepters.push_back(p_ptr);
			}
		}

		assert(intercepters.size() < 3);

		if (intercepters.size() == 1)
		{
			intercepters[0]->takeBall(*(this->_free_ball));
			this->_free_ball = nullptr;
		}
		else if (intercepters.size() == 2)
		{
			fairFightBall(*(intercepters[0]), *(intercepters[1]));
		}
		else
		{
			this->_free_ball->move();
			ball_in_goal_A = find(this->_goal_A.begin(), this->_goal_A.end(), (this->_free_ball)->getPosition()) != this->_goal_A.end();
			ball_in_goal_B = find(this->_goal_B.begin(), this->_goal_B.end(), (this->_free_ball)->getPosition()) != this->_goal_B.end();
			assert(positionInBoard((this->_free_ball)->getPosition()) || ball_in_goal_A || ball_in_goal_B);

			/* Si hay jugadores en ese casillero, entonces hay que ver si es uno
			   solo entonces agarra la pelota y si son dos se la disputan */
			std::vector<Player*> players_to_fight;

			for (Player &p : this->_team_A)
			{
				if (p.getPosition() == (this->_free_ball)->getPosition())
				{
					Player *p_ptr = &p;
					players_to_fight.push_back(p_ptr);
				}
			}

			for (Player &p : this->_team_B)
			{
				if (p.getPosition() == (this->_free_ball)->getPosition())
				{
					Player *p_ptr = &p;
					players_to_fight.push_back(p_ptr);
				}
			}

			if (players_to_fight.size() == 1)
			{
				players_to_fight[0]->takeBall(*(this->_free_ball));
				this->_free_ball = nullptr;
			}

			if (players_to_fight.size() == 2)
			{
				fairFightBall(*(players_to_fight[0]), *(players_to_fight[1]));
			}
		}
	}
	else
	{
		/* Si dos jugadores estan en el mismo casillero y uno tiene la pelota
		   Los mismos se disputan quien termina con la posesion. */

		// El equipo A tiene la pelota
		bool already_fight = false;

		for (Player &p1 : this->_team_A)
		{
			if (already_fight) break;

			if (p1.getBall() != nullptr)
			{
				for (Player &p2 : this->_team_B)
				{
					if (p1.getPosition() == p2.getPosition())
					{
						fightBall(p1, p2);
						already_fight = true;
						break;
					}
				}
			}
		}

		if (!already_fight)
		{
			// El equipo B tiene la pelota
			for (Player &p1 : this->_team_B)
			{
				if (already_fight) break;

				if (p1.getBall() != nullptr)
				{
					for (Player &p2 : this->_team_A)
					{
						if (p1.getPosition() == p2.getPosition())
						{
							fightBall(p1, p2);
							already_fight = true;
							break;
						}
					}
				}
			}
		}
	}

	return updateScore();
}

void LogicalBoard::undoMove(BoardState *last_state)
{
	if (last_state == nullptr)
	{
		last_state = this->_last_state;
	}

	if (last_state != nullptr)
	{
		/* TODO: revisar que no pierda memoria cuando
		   sobreescribo al jugador con la pelota */
		this->_team_A = last_state->getPlayers(A);
		this->_team_B = last_state->getPlayers(B);

		delete this->_free_ball;
		this->_free_ball = nullptr;

		if (last_state->getBall() != nullptr)
		{
			this->_free_ball = new Ball(*(last_state->getBall()));
		}
	}
}

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

bool LogicalBoard::positionInBoard(std::pair<int, int> pos) const
{
	return pos.first < this->_rows && pos.second < this->_columns;
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

BoardState* LogicalBoard::getState() const
{
	return new BoardState(this->_team_A, this->_team_B, *(this->_free_ball));
}

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

Ball* LogicalBoard::getBall()
{
	return this->_free_ball;
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