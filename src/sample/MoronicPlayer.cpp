#include "MoronicPlayer.h"
#include <cassert>

MoronicPlayer::MoronicPlayer(
    int columns,
    int rows,
    int steps,
    std::string side,
    const std::vector<player>& players, // Este no lo usa para nada
    const std::vector<player>& oponent_players // Este no lo usa para nada
) : static_player(columns, rows, steps, side, players, oponent_players) {

    this->players_ = players;
    this->oponentPlayers_ = oponent_players;
    if (side == IZQUIERDA) {
        laDireccion = 1;
    } else {
        laDireccion = -1;
    }
}

void MoronicPlayer::starting_positions(std::vector<player_status>& positions) {
    int column = this->columns - 1;
    if (this->side == IZQUIERDA) {
        column = 0;
    }

    positions.emplace_back(0, 1, column, players[0].probability, false);
    positions.emplace_back(1, 0, column, players[0].probability, false);
    positions.emplace_back(2, 4, column, players[0].probability, false);
}

void MoronicPlayer::make_move(const board_status& current_board, std::vector<player_move>& made_moves) {
    made_moves.clear();
    for (const player_status& p : current_board.team) {
        player_move new_move;
        new_move.player_id = p.id;
        new_move.move_type = MOVIMIENTO;
        if (p.j < this->columns-1 || ((p.i == 2) && p.in_possession)) {
            new_move.dir = 4;
        } else {
            new_move.dir = 0;
        }
        made_moves.push_back(new_move);
    }
}

void MoronicPlayer::finish(std::string result) { }

MoronicPlayer::~MoronicPlayer() {
}
