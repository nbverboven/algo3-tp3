#include "Player.h"
#include "BoardState.h"

#ifndef TEAM_INTERFACE_H
#define TEAM_INTERFACE_H

/**
 * Interfaz generica para un equipo. Esta clase base es un intento
 * de estandarizar de verdad como tiene que ser una clase que
 * represente un equipo, necesaria para que el Referee sepa como
 * manipular los objetos de tipo 'equipo'.
 */
class TeamInterface {

public:
    /**
     * Constructor.
     */
    TeamInterface(int columns,
        int rows,
        int steps,
        std::string side,
        const std::vector<Player>& players,
        const std::vector<Player>& oponent_players
    );

    /**
     * Destructor.
     */
    virtual ~TeamInterface();

    /**
     * Setea las posiciones iniciales de los jugadores.
     */
    virtual void starting_positions(std::vector<player_status>& positions)=0;

    /**
     * Ejecuta el movimiento, depositando el movimiento a realizar
     * en el segundo parametro.
     */
    virtual void make_move(BoardState* current_board, std::vector<player_move>& made_moves)=0;

    /**
     * Finaliza la ejecucion del equipo (necesaria para poder usar el player_controller).
     */
    virtual void finish(std::string result);

    /**
     * Devuelve el array de players del equipo.
     */
    std::vector<Player> getPlayers();

protected:
    /** Cantidad de columnas de la cancha. */
    int columns_;
    /** Cantidad de filas de la cancha. */
    int rows_;
    /** Cantidad de pasos del partido*/
    int steps_;
    /** Lado (izquierda/derecha) de la cancha */
    std::string side_;
    /** Jugadores del equipo */
    std::vector<Player> players_;
    /** Jugadores del equipo rival */
    std::vector<Player> oponent_players_;
};


#endif
