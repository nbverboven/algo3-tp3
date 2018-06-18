#ifndef GRASP_PLAYER
#define GRASP_PLAYER

#include <vector>
#include "GenericPlayer.hpp"
#include "board_status.hpp"
#include "constants.hpp"
#include "LogicalBoard.h"

class GraspPlayer : public GenericPlayer {

public:

    GraspPlayer() = default;

    /**
     * Constructor.
     */
    GraspPlayer(
        int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<player>& players, // Este no lo usa para nada
        const std::vector<player>& oponent_players, // Este no lo usa para nada
        genome weights = genome()
    );

    /**
     * Destructor.
     */
    virtual ~GraspPlayer();

    /**
     * Modifica la lista pasada como parámetro con los movimientos de los juegadores
     */
    virtual void make_move(const board_status& current_board, std::vector<player_move>& made_moves);

protected:

    /**
     * Setea los movimientos del oponente
     */
    void setOponentMoves(const board_status& current_board, std::vector<player_move>& oponent_moves);

    std::vector<player> players;
    std::vector<player> oponents;
    LogicalBoard logicalBoard;
    genome _genome;
};

#endif //GLOTTONUS_PLAYER
