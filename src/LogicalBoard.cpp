#include "LogicalBoard.h"

LogicalBoard::LogicalBoard(int columns, int rows, const std::vector<player> &team_A, const std::vector<player> &team_B)
	: _columns(columns), _rows(rows)
{
	this->_score[A] = 0;
	this->_score[B] = 0;

	for (const player &p : team_A)
	{
		(this->_team_A).push_back(player_status(p.id, p.probability));
	}

	for (const player &p : team_B)
	{
		(this->_team_B).push_back(player_status(p.id, p.probability));
	}
}

void LogicalBoard::updateBoard(const board_status& board, const std::string& team)
{
	this->_ball = board.ball;

	//El tablero no sabe si es el equipo A o B ..
	std::vector<player_status> _team_old;
	std::vector<player_status> _oponent_team_old;

	if(team == A){
		_team_old = this->_team_A; 
		_oponent_team_old = this->_team_B; 
	}else{
		_oponent_team_old = this->_team_A; 
		_team_old = this->_team_B;
	}

	std::vector<player_status> _team;
	std::vector<player_status> _oponent_team;
	
	for (const player_status &p : board.team) {
		//Lamentablemente tengo que actualizar las probabilidades porque no vienen en el board..
		double _proba = 0;
		for (const player_status &p2 : _team_old){
			if(p.id == p2.id){
				_proba = p2.probability; break;
			}
		}
		player_status jg(p);
		jg.probability = _proba;
		_team.push_back(jg);
	}

	for (const player_status &p : board.oponent_team) {
		//Acá tambien tengo que actualizar..
		double _proba = 0;
		for (const player_status &p2 : _oponent_team_old){
			if(p.id == p2.id){
				_proba = p2.probability; break;
			}
		}
		player_status jg(p);
		jg.probability = _proba;
		_oponent_team.push_back(jg);
	}
	
	if(team == A){
		this->_team_A = _team;
		this->_team_B = _oponent_team;
	}else{
		this->_team_A = _oponent_team;
		this->_team_B = _team;
	}

}

// Asumo que moves está indexado por el id de los jugadores de team
void LogicalBoard::makeTeamMove(std::vector<player_status> &team, std::vector<player_move> &moves)
{
	for (player_status &p : team)
	{
		player_move movim = moves[p.id];

		if (movim.move_type == MOVIMIENTO)
		{
			p.i += MOVES[movim.dir].i;
			p.j += MOVES[movim.dir].j;

			if (p.in_possession)
			{
				(this->_ball).i = p.i;
				(this->_ball).j = p.j;
			}
		}

		/* Si el jugador pasó la pelota se setea la dirección y fuerza y se pierde
		   la posesión, luego el tablero detecta la pelota libre y la mueve en cada paso */
		if (movim.move_type == PASE)
		{
			(this->_ball).is_free = true;
			(this->_ball).dir = movim.dir;
			(this->_ball).steps = movim.steps;
			p.in_possession = false;
		}
	}
}

void LogicalBoard::fightBall(player_status &player_with_ball, player_status &player_without_ball)
{
	// Probabilidad de defensa
	float p_defend_ball = 1 - player_with_ball.probability;

	// Probabilidad de ataque
	float p_take_ball = player_without_ball.probability;

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
		player_without_ball.in_possession = true;
		player_with_ball.in_possession = false;
	}
}

void LogicalBoard::fairFightBall(player_status &p1, player_status &p2)
{
	// Probabilidad de que p2 se quede con la pelota
	float p_p2 = p2.probability / (p1.probability + p2.probability);

	/* Genero un número al azar entre 0 y 1 de una distribucíón
       uniforme
       Nota: ver nota en función fightBall */
	std::mt19937 rnd_generator;
	rnd_generator.seed(std::random_device()());
	std::uniform_real_distribution<float> distribution(0, 1);

	if (distribution(rnd_generator) < p_p2)
	{
		p2.in_possession = true;
	}
	else
	{
		p1.in_possession = true;
	}

	this->_ball.steps = 0;
	this->_ball.is_free = false;
}

bool LogicalBoard::intercepted(const player_status &curr_state_player, std::string team) const
{
	bool result = true;

	// Buscar el estado anterior del jugador
	player_status prev_state_player;
	std::vector<player_status> players = (team == A) ? this->_team_A : this->_team_B;

	for (player_status &p : players)
	{
		if (p.id == curr_state_player.id)
		{
			prev_state_player = p;
		}
	}

	// Si se movio, no la interceptó
	bool in_same_position = (prev_state_player.i == curr_state_player.i) && (prev_state_player.j == curr_state_player.j);
	result = result && in_same_position;

	// Si está en el camino, la intercepta
	prev_state_player.i -= MOVES[this->_ball.dir].i;
	prev_state_player.j -= MOVES[this->_ball.dir].j;
	result = result && ((prev_state_player.i == (this->_ball).i) && (prev_state_player.j == (this->_ball).j));
	prev_state_player.i += MOVES[this->_ball.dir].i;
	prev_state_player.j += MOVES[this->_ball.dir].j;

	return result;
}

std::string LogicalBoard::makeMove(std::vector<player_move> &moves_A, std::vector<player_move> &moves_B)
{
	this->_last_state = getState();
	makeTeamMove(this->_team_A, moves_A);
	makeTeamMove(this->_team_B, moves_B);

	// El balón se mueve en la dirección indicada por el último pase
	if ((this->_ball).is_free) {

		// Mira si alguien interceptó la pelota
		std::vector<std::pair<player_status, std::string>> intercepters;

		for (player_status &p : this->_team_A) {
			if (intercepted(p, A)) {
				intercepters.push_back(std::make_pair(p, A));
			}
		}

		for (player_status &p : this->_team_B) {
			if (intercepted(p, B)) {
				intercepters.push_back(std::make_pair(p, B));
			}
		}

		assert(intercepters.size() < 3);

		if (intercepters.size() == 1) {
			if (intercepters[0].second == A) this->_team_A[intercepters[0].first.id].in_possession = true;
			if (intercepters[0].second == B) this->_team_B[intercepters[0].first.id].in_possession = true;
			(this->_ball).is_free = false;
		}
		else if (intercepters.size() == 2) {
			player_status *p1 = nullptr;
			player_status *p2 = nullptr;

			if (intercepters[0].second == A){
				p1 = &(this->_team_A[intercepters[0].first.id]);
			}
			else {
				p1 = &(this->_team_B[intercepters[0].first.id]);
			}

			if (intercepters[1].second == A){
				p2 = &(this->_team_A[intercepters[1].first.id]);
			}
			else {
				p2 = &(this->_team_B[intercepters[1].first.id]);
			}

			fairFightBall(*p1, *p2);
		}
		else {
			move ball_movement = MOVES[(this->_ball).dir];
			(this->_ball).move(ball_movement);

			if (positionInBoard((this->_ball).i, (this->_ball).j)) {
				/* Si hay jugadores en ese casillero, entonces hay que ver si es uno
				   solo entonces agarra la pelota y si son dos se la disputan */
				std::vector<std::pair<player_status, std::string>> players_to_fight;

				for (player_status &p : this->_team_A) {
					if (p.i == (this->_ball).i && p.j == (this->_ball).j) {
						players_to_fight.push_back(std::make_pair(p, A));
					}
				}

				for (player_status &p : this->_team_B) {
					if (p.i == (this->_ball).i && p.j == (this->_ball).j) {
						players_to_fight.push_back(std::make_pair(p, B));
					}
				}

				if (players_to_fight.size() == 1) {
					player_status *p1 = nullptr;

					if (players_to_fight[0].second == A){
						p1 = &(this->_team_A[players_to_fight[0].first.id]);
					}
					else {
						p1 = &(this->_team_B[players_to_fight[0].first.id]);
					}

					p1->in_possession = true;
					(this->_ball).is_free = false;
					(this->_ball).steps = 0;
				}

				if (players_to_fight.size() == 2) {
					player_status *p1 = nullptr;
					player_status *p2 = nullptr;

					if (players_to_fight[0].second == A){
						p1 = &(this->_team_A[players_to_fight[0].first.id]);
					}
					else {
						p1 = &(this->_team_B[players_to_fight[0].first.id]);
					}

					if (players_to_fight[1].second == A){
						p2 = &(this->_team_A[players_to_fight[1].first.id]);
					}
					else {
						p2 = &(this->_team_B[players_to_fight[1].first.id]);
					}

					fairFightBall(*p1, *p2);
				}
			}
		}
	}
	else
	{
		/* Si dos jugadores estan en el mismo casillero y uno tiene la pelota
		   Los mismos se disputan quien termina con la posesion. */

		// El equipo A tiene la pelota
		bool already_fight = false;

		player_status *p_A = nullptr;
		player_status *p_B = nullptr;

		for (player_status &p1 : this->_team_A) {
			if (already_fight) break;

			if (p1.in_possession == true) {
				p_A = &p1;
				for (player_status &p2 : this->_team_B) {
					if (p1.i == p2.i && p1.j == p2.j) {
						p_B = &p2;
						fightBall(*p_A, *p_B);
						already_fight = true;
						break;
					}
				}
			}
		}

		if (!already_fight)
		{
			// El equipo B tiene la pelota
			for (player_status &p1 : this->_team_B) {
				if (already_fight) break;

				if (p1.in_possession == true) {
					p_A = &p1;
					for (player_status &p2 : this->_team_A) {
						if (p1.i == p2.i && p1.j == p2.j) {
							p_B = &p2;
							fightBall(*p_A, *p_B);
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

void LogicalBoard::undoMove(board_status last_state)
{
	this->_team_A = last_state.team;
	this->_team_B = last_state.oponent_team;
	this->_ball = last_state.ball;
}

std::string LogicalBoard::winner()
{
	if ((this->_score)[A] < (this->_score)[B]) return B;
	if ((this->_score)[A] > (this->_score)[B]) return A;
	return "NINGUNO";
}

std::string LogicalBoard::updateScore()
{
	std::string res;

	std::pair<int, int> position = std::make_pair((this->_ball).i, (this->_ball).j);
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

void LogicalBoard::startingPositions(const std::vector<std::pair<int, int>> &position_A, 
                       const std::vector<std::pair<int, int>> &position_B,
                       std::string starting)
{
	for (int i = 0; i < 3; ++i)
	{
		// Si algún jugador tenía la pelota, se la saco
		if ((this->_team_A[i]).in_possession)
		{
			(this->_team_A[i]).in_possession = false;
		}

		if ((this->_team_B[i]).in_possession)
		{
			(this->_team_B[i]).in_possession = false;
		}

		// Coloco a los jugadores en las posiciones correctas
		(this->_team_A[i]).i = (position_A[i]).first;
		(this->_team_A[i]).j = (position_A[i]).second;
		(this->_team_B[i]).i = (position_B[i]).first;
		(this->_team_B[i]).j = (position_B[i]).second;
	}

	(this->_ball).is_free = false;

	// Muevo la pelota al centro de la cancha
	(this->_ball).i = floor(this->_rows/2);
	(this->_ball).j = (this->_columns/2) - 1;

	// Le doy la pelota al jugador que saca y lo pongo en el centro
	if (starting == A)
	{
		(this->_team_A[0]).in_possession = true;
		(this->_team_A[0]).i = (this->_ball.i);
		(this->_team_A[0]).j = (this->_ball.j);
	}
	else
	{
		(this->_team_B[0]).in_possession = true;
		(this->_team_B[0]).i = (this->_ball.i);
		(this->_team_B[0]).j = (this->_ball.j);
	}
}

bool LogicalBoard::positionInBoard(int i, int j) const
{
	bool i_ok = i >= 0 && i < this->_rows;
	bool j_ok = j >= 0 && j < this->_rows;
	return i_ok && j_ok;
}

board_status LogicalBoard::getState() const
{
	board_status curr_state;
	curr_state.team = this->_team_A;
	curr_state.oponent_team = this->_team_B;
	curr_state.ball = this->_ball;

	return curr_state;
}

const std::vector<std::pair<int, int>>& LogicalBoard::getGoal(std::string team) const
{
	if (team == A) return this->_goal_A;
	return this->_goal_B;
}

std::vector<player_status>& LogicalBoard::getTeam(std::string team)
{
	if (team == A) return this->_team_A;
	return this->_team_B;
}

ball_status LogicalBoard::getBall()
{
	return this->_ball;
}


bool LogicalBoard::isValidTeamMove(const std::vector<player_status>& team, const std::vector<player_move>& moves){
	bool ret = true;

	for (const player_status &p : team){


	}

	return ret;
}