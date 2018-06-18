#include "referee.h"

#define PARTIDOS_A_DISPUTAR 10

int main()
{
	std::vector<player> players;
	std::vector<player> oponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		oponents.push_back(player(i, 0.5));
	}

	int columns = 10;
	int rows = 5;
	int steps = 50;

	/* Puede ser así o puede ser un struct */
	// std::vector<int> parametros;
	// parametros <- los pesos de las características a evaluar al puntuar un tablero

	GreedyPlayer GreedyPlayer(columns, rows, steps, IZQUIERDA, players, oponents);
	static_player static_p(columns, rows, steps, DERECHA, oponents, players);

	/* La idea es guardar los parámetros usados, y cuántos partidos
	   ganó mi equipo para esos valores */
	// std::std::vector<std::pair<int, int, int, int, int,..., partidos_ganados>> game_result;
	// std::std::vector<std::pair<struct_que_guarda_parámetros, partidos_ganados>> game_result;

	for (int i = 0; i < PARTIDOS_A_DISPUTAR; ++i) {
		// Voy a asumir que mi equipo es el A
		// int games_won_by_my_team = 0;

		Referee ref(columns, rows, steps, moronic_p, static_p);
		std::string the_winner = ref.runPlay(A);
		
		// Registro cuántos partidos ganó cada uno
		// if (the_winner == A) {
		// 	++games_won_by_my_team;
		// }
		// else {
		//  ++games_won_by_oponent;
		// }
	}

	/* Busco la combinación de parámetros que haya ganedo la 
	   mayor cantidad de partidos. Si hay más de una, puedo elegir todas o
	   quedarme solo con una */
	// res <- i \in game_result | i.partidos_ganados = max{ e.partidos_ganados \forall e \in game_result } ;

	return 0;
}