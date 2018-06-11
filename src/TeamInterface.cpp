#include "TeamInterface.h"

/**
 * Constructor.
 */
TeamInterface::TeamInterface(
        int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<player>& players,
        const std::vector<player>& oponent_players):players_(players), oponent_players_(oponent_players) {

    this->columns_ = columns;
    this->rows_ = rows;
    this->steps_ = steps;
    this->side_ = side;
}

/**
 * Destructor.
 */
TeamInterface::~TeamInterface() {
}


std::vector<player> TeamInterface::getPlayers() {
    return this->players_;
}

/**
 * Finaliza la ejecucion del equipo (necesaria para poder usar el player_controller).
 */
void TeamInterface::finish(std::string result) {
}
