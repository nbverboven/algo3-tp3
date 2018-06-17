#include <vector>
#include "static_player.hpp"
#include "constants.hpp"
#include "board_status.hpp"

class GenericPlayer : public static_player {

public:

	GenericPlayer() = default;

	/**
	 * Constructor.
	 */
	GenericPlayer(
		int columns,
		int rows,
		int steps,
		std::string side,
		const std::vector<player>& players, // Este no lo usa para nada
		const std::vector<player>& oponent_players // Este no lo usa para nada
	){
		this->columns = columns;
		this->rows = rows;
		this->steps = steps;
		this->side = side;

		if(this->side == IZQUIERDA)
			this->team = 'B';
		else
			this->team = 'A';
	}

	/**
	 * Destructor.
	 */
	virtual ~GenericPlayer(){

	}

	/**
	 * Modifica la lista pasada como parámetro con los movimientos de los juegadores
	 */
	//void make_move(const board_status& current_board, std::vector<player_move>& made_moves);

protected:

	/**
	 *
	 */
	int EvaluateBoard(const board_status& current_board){

	};

	
	std::vector<player> players;
	std::vector<player> oponents;
	std::vector<int> genomes;
	

};