#ifndef TEAM_MOVEMENTS_H
#define TEAM_MOVEMENTS_H

#include <string>
#include <vector>
#include <map>
#include <utility>

class TeamMovements
{
public:
	/**
	  * Constructor. 
	  */
	TeamMovements();

	/**
	 * Asigna el movimiento (type, value) al jugador indicado por parámetro
	 */
	void assignMovement(unsigned int player_id, std::string type, std::pair<int, int> value);

	/**
	 * Devuelve true si el jugador indicado tiene asignado algún movimiento
	 * y false en caso contrario
	 */
	bool hasMovementAssigned(unsigned int player_id);

	/**
	 * Devuelve el tipo de movimiento a realizar por el jugador de id 
	 * player_id
	 */
	std::string getMovementType(unsigned int player_id);

	/**
	 * Devuelve el par (dirección, #pasos) del movimiento a realizar por 
	 * el jugador de id  player_id
	 */
	std::pair<int, int> getMovementValue(unsigned int player_id);

private:
	struct move_str
	{
		std::string type;
		int direction;
		int steps;
	};

	/* Vector de pares (tipo de movimiento, #pasos).
	   La i-ésima posición corresponde al jugador con player_id = i */
	std::map<unsigned int, move_str> _moves;
};

#endif // TEAM_MOVEMENTS_H