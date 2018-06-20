#include "Referee.h"
#include "GreedyPlayer.h"
#include <vector>
#include <fstream>

#define COLUMNS 10
#define ROWS 5
#define STEPS 50

#define GAMES_TO_PLAY 20
#define MAX_NUMBER_OF_SEARCHS 100
#define CONVERGENCE_CRITERION 7
 
/**
 * Genera el producto cartesiano entre todos los items de todos
 * los vectores de vectores.
 */
std::vector<std::vector<double>> cart_product(std::vector<std::vector<double>>& v)
{
    std::vector<std::vector<double>> s = {{}};
    for (auto& u : v) {
        std::vector<std::vector<double>> r;
        for (auto& x : s) {
            for (auto y : u) {
                r.push_back(x);
                r.back().push_back(y);
            }
        }
        s.swap(r);
    }
    return s;
}


/**
 * Dado un genoma, devuelve una matriz cuyas filas corresponden 
 * a sus vecinos
 */
std::vector<std::vector<double>> getNeighborhood(genome &g) 
{
    std::vector<std::vector<double>> elInput;

    for (double gene_value : g.genic_values) {

    	/* Chequeo los casos borde para que los valores resultantes
    	   no salgan del rango [0, 1] */
    	if (gene_value == -1) {
    		elInput.push_back({gene_value, gene_value+0.1});
    	}
    	else if (gene_value == 1) {
    		elInput.push_back({gene_value-0.1, gene_value});	
    	}
    	else {
		    elInput.push_back({gene_value-0.1, gene_value, gene_value+0.1});
    	}
    }
  
    return cart_product(elInput);
}


std::pair<genome, int> maximum(std::vector<std::pair<genome, int>> &v) {
	unsigned int max_index = 0;
	int i = 1;
	while (i < v.size()) {
		if (v[i].second > v[max_index].second) {
			max_index = i;
		}
		++i;
	}

	return v[max_index];
}


int main()
{
	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

	int columns = COLUMNS;
	int rows = ROWS;
	int steps = STEPS;

	// Creo un genoma con valores por defecto
	genome current_genome;

	// Contadores varios
	int neighborhoods_visited = 0;
	int consecutive_equal_maxima_found = 0;
	int previous_maximum = 0;

	std::ofstream log_file;
	log_file.open("log/local_search.log");

	while (neighborhoods_visited < MAX_NUMBER_OF_SEARCHS && 
		   consecutive_equal_maxima_found < CONVERGENCE_CRITERION) {

		GreedyPlayer my_team(columns, rows, steps, IZQUIERDA, players, opponents, current_genome);

		/* La idea es guardar los parámetros usados, y cuántos partidos
		   ganó mi equipo para esos valores */
		std::vector<std::pair<genome, int>> game_results;

		/* Obtengo el vecindario del current_genome y lo recorro
		   exhaustivamente buscando algún genoma que mejore el mío */
		std::vector<std::vector<double>> current_neighborhood = getNeighborhood(current_genome);
		for (std::vector<double> neighbor : current_neighborhood)
		{
			genome test_genome(neighbor); // Sus valores dependerán del vecino en cuestión
			GreedyPlayer opponent(columns, rows, steps, DERECHA, opponents, players, test_genome);
	
			// Voy a asumir que mi equipo es el A
			int games_won_by_my_team = 0;
			int games_won_by_opponent = 0;
	
			/* Pongo a los dos equipos a jugar una cantidad de partidos y 
			   registro cuántos ganó cada uno */
			for (int i = 0; i < GAMES_TO_PLAY; ++i) {
				Referee ref(columns, rows, steps, my_team, opponent);
				std::string the_winner = ref.runPlay(A);
				
				if (the_winner == A) {
					++games_won_by_my_team;
				}
				else {
					++games_won_by_opponent;
				}
			}
	
			// Agrego los genomas más ganadores que el mío a la lista de resultados
			if (games_won_by_my_team <= games_won_by_opponent) {
				game_results.push_back(std::make_pair(test_genome, games_won_by_opponent));
			}
		}

		/* Busco la combinación de parámetros que me haya ganado la 
		   mayor cantidad de partidos*/
		std::pair<genome, int> best_local_result = maximum(game_results);
		current_genome = best_local_result.first;
		int max_local_games_won = best_local_result.second;

		// Veo si puedo estar convergiendo en un máximo local
		if (max_local_games_won == previous_maximum) {
			++consecutive_equal_maxima_found;
		}
		// Veo si encontré un nuevo máximo
		else if (max_local_games_won > previous_maximum) {
			consecutive_equal_maxima_found = 0;
			previous_maximum = max_local_games_won;
		}

		++neighborhoods_visited;

		// log_file << 
	}

	log_file.close();

	return 0;
}