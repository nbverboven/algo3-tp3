#ifndef GLOTTONUS_PLAYER
#define GLOTTONUS_PLAYER

#include <vector>
#include "static_player.hpp"
#include "board_status.hpp"
#include "constants.hpp"
#include "LogicalBoard.h"

class GreedyPlayer : public static_player {

public:

    GreedyPlayer() = default;

    /**
     * Constructor.
     */
    GreedyPlayer(
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
    virtual ~GreedyPlayer();

    /**
     * Modifica la lista pasada como parámetro con los movimientos de los juegadores
     */
    virtual void make_move(const board_status& current_board, std::vector<player_move>& made_moves);

    /**
     * Evalua el tablero asignandole un puntaje.
     * Mientras mas bajo el puntaje del tablero, mejor.
     */
    int EvaluateBoard(const board_status& current_board);

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
