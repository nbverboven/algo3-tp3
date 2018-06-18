#include "MoronicPlayer.h"
#include "player_controller.hpp"

int main(int argc, char const *argv[]) {
    player_controller<MoronicPlayer> player(argv[1]);
    player.play();
    return 0;
}
