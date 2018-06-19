#include "Referee.h"
#include "GreedyPlayer.hpp"

#define GAMES_TO_PLAY 20
#define MAX_NUMBER_OF_SEARCHS 100

int main()
{
	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

	int columns = 10;
	int rows = 5;
	int steps = 50;

	/* La idea es guardar los parámetros usados, y cuántos partidos
	   ganó mi equipo para esos valores */
	std::vector<std::pair<genome, int>> game_results;
	genome current_genome(); //Con algunos valores iniciales

	// Contadores de partidos ganados
	int number_of_local_maxima_found = 0;
	int consecutive_equal_maxima_found = 0;
	int last_maximum = 0;

	/* Si k1, k2,..., kn son los genes, dado un genoma en particular definimos
	   su vecindad como un entorno de 0,1 alrededor de cada ki, 1<=i<=n */
	// Mientras no haya llegado a las 200 (!!) búsquedas
	// o no converja la cantidad de partidos ganados
	{
		GreedyPlayer GreedyPlayer(columns, rows, steps, IZQUIERDA, players, opponents, current_genome);
		// vecindad_del_genoma_actual <- obtenerVecindad(current_genome)

		// Busco en la vecindad
		// for (elem in vecindad) do
		{
			genome test_genome; // Sus valores dependerán del vecino en cuestión
			GreedyPlayer GreedyPlayer(columns, rows, steps, DERECHA, opponents, players, test_genome);
	
			// Voy a asumir que mi equipo es el A
			int games_won_by_my_team = 0;
			int games_won_by_opponent = 0;
	
			/* Pongo a los dos equipos a jugar una cantidad de partidos y 
			   registro cuántos ganó cada uno */
			for (int i = 0; i < GAMES_TO_PLAY; ++i) {
				Referee ref(columns, rows, steps, moronic_p, static_p);
				std::string the_winner = ref.runPlay(A);
				
				// Registro quién ganó
				if (the_winner == A) {
					++games_won_by_my_team;
				}
				else {
					++games_won_by_opponent;
				}
			}
	
			// Agrego el genoma más ganador a la lista de resultados
			if (games_won_by_my_team >= games_won_by_opponent) {
				game_results.push_back(std::make_pair(current_genome, games_won_by_my_team));
			}
			else {
				game_results.push_back(std::make_pair(test_genome, games_won_by_opponent));
			}
		}
		// end for
		// fin busco en la vecindad

		/* Busco la primera combinación de parámetros que haya ganado la 
		   mayor cantidad de partidos*/
		int i = 0;
		while (game_results[i].second != last_maximum) {
			++i;
		}
		std::pair<genome, int> best_local_result = game_results[i];

		current_genome = best_local_result.first;
		int max_local_games_won = best_local_result.second;

		// Veo si puedo estar convergiendo en un máximo local
		if (max_local_games_won == last_maximum) {
			++consecutive_equal_maxima_found;
		}
		// Veo si encontré un nuevo máximo
		else if (max_local_games_won > last_maximum) {
			consecutive_equal_maxima_found = 0;
			last_maximum = max_local_games_won;
		}
	}
	// fin mientras

	return 0;
}