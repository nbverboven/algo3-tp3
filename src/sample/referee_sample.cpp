#include "MoronicPlayer.h"
#include "../static_player.hpp"
#include "../Referee.h"

int main(int argc, char const *argv[]) {
    std::vector<player> players;
    std::vector<player> oponents;
    for (int i = 0; i < 3; ++i) {
        players.push_back(player(i, 0.5));
        oponents.push_back(player(i, 1));
    }

    int columns = 10;
    int rows = 5;
    int steps = 100;

    MoronicPlayer moronic_p(columns, rows, steps, IZQUIERDA, players, oponents);
    static_player static_p(columns, rows, steps, "DERECHA", oponents, players);
    Referee ref(columns, rows, steps, moronic_p, static_p);
    std::string the_winner = ref.runPlay("A");
    std::cout << the_winner << std::endl;

    return 0;
}
