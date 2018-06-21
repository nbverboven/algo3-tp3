#include <vector>
#include "Referee.h"
#include "GreedyPlayer.h"

#define ROWS 5
#define COLUMNS 10
#define STEPS 50

#define GAMES_TO_PLAY 20
#define MAX_NUMBER_OF_SEARCHS 25
#define CONVERGENCE_CRITERION 7

int main()
{
	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

	genome current_genome;
	int neighborhoods_visited = 0;
	int consecutive_equal_maxima_found = 0;
	int previous_maximum = 0;

	std::ofstream log_file;
	log_file.open("log/local_search.log");

	// while (neighborhoods_visited < MAX_NUMBER_OF_SEARCHS && consecutive_equal_maxima_found < CONVERGENCE_CRITERION) {

		/* La idea es guardar los parámetros usados, y cuántos partidos
		   ganó mi equipo para esos valores */
		std::vector<std::pair<genome, int>> game_results;

		/* Obtengo el vecindario del current_genome y lo recorro
		   exhaustivamente buscando algún genoma que mejore el mío */
		std::vector<std::vector<double>> current_neighborhood = getNeighborhood(current_genome);
		for (std::vector<double> &neighbor : current_neighborhood) {
			// Sus valores dependerán del vecino en cuestión
			genome test_genome(neighbor);
			
			// Voy a asumir que mi equipo es el A
			int games_won_by_my_team = 0;
			int games_won_by_opponent = 0;
			
			/* Pongo a los dos equipos a jugar una cantidad de partidos y 
			   registro cuántos ganó cada uno */
			for (int i = 0; i < GAMES_TO_PLAY; ++i) {
			    GreedyPlayer my_team(COLUMNS, ROWS, STEPS, IZQUIERDA, players, opponents, current_genome);
			    GreedyPlayer opponent(COLUMNS, ROWS, STEPS, DERECHA, opponents, players, test_genome);
				Referee ref(COLUMNS, ROWS, STEPS, my_team, opponent);
				std::string the_winner = ref.runPlay(A);
				
				if (the_winner == A) {
					++games_won_by_my_team;
				}
				else if (the_winner == B) {
					++games_won_by_opponent;
				}
			}
	
			// Agrego los genomas más ganadores que el mío a la lista de resultados
			if (games_won_by_my_team <= games_won_by_opponent) {
				game_results.push_back(std::make_pair(test_genome, games_won_by_opponent));
			}
		}

		/* Busco la combinación de parámetros que haya ganado la 
		   mayor cantidad de partidos contra mi equipo */
		std::pair<genome, int> best_local_result;
		if (!game_results.empty()) {
			best_local_result = maximum(game_results);
		}
		else {
			best_local_result = std::make_pair(current_genome, current_neighborhood.size());
		}

		log_file << "================================================" << std::endl;
		log_file << "VECINDARIO: " << neighborhoods_visited << std::endl;
		log_file << "GENOMA MÁS GANADOR A NIVEL LOCAL: " << std::endl;
		log_file << best_local_result.first << std::endl;
		log_file << "PARTIDOS GANADOS: " << best_local_result.second << std::endl;
		log_file << "================================================" << std::endl;

		// Si le gané a todos mis vecinos, soy el más mejor :D
		// if (best_local_result.first.genic_values == current_genome.genic_values) {
		// 	break;
		// }

		// Veo si puedo estar convergiendo en un máximo local
		int max_local_games_won = best_local_result.second;
		if (max_local_games_won == previous_maximum) {
			++consecutive_equal_maxima_found;
		}
		// Veo si encontré un nuevo máximo
		else if (max_local_games_won > previous_maximum) {
			consecutive_equal_maxima_found = 0;
			previous_maximum = max_local_games_won;
		}

		current_genome = best_local_result.first;
		++neighborhoods_visited;
	// }

	log_file.close();

	return 0;
}

