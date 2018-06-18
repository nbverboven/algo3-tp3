#include "../static_player.hpp"

class MoronicPlayer : public static_player {

public:
    MoronicPlayer(int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<player>& players, // Este no lo usa para nada
        const std::vector<player>& oponent_players // Este no lo usa para nada
    );

    MoronicPlayer() = default;

    virtual ~MoronicPlayer();

    virtual void make_move(const board_status& current_board, std::vector<player_move>& made_moves);

    void starting_positions(std::vector<player_status>& positions);

    void finish(std::string result);

private:
    int laDireccion;

    std::vector<player> players_;
    std::vector<player> oponentPlayers_;

    std::vector<player_status> myStatus_;
};
