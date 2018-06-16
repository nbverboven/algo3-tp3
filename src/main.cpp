#include <string>
#include <random>
#include <vector>
#include <map>

//#include "static_player.hpp"
#include "player_controller.hpp"
#include "GlottonusPlayer.cpp"

int main(int argc, char **argv ) {
    player_controller<GlottonusPlayer> player(argv[1]);
    player.play();
    return 0;
}