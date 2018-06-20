#include "Referee.h"
#include "GreedyPlayer.h"
#include <vector>
#include <fstream>

#define COLUMNS 10
#define ROWS 5
#define STEPS 50

#define GAMES_TO_PLAY 10
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

    // Genero los vecinos son incluir al genoma que paso como parámetro
    std::vector<std::vector<double>> result = cart_product(elInput);
    for (unsigned int i = 0; i < result.size(); ++i) {
    	if (result[i] == g.genic_values) {
    		result[i] == result[result.size()-1];
    		result.pop_back();
    	}
    }

    return result;
}


std::pair<genome, int> maximum(std::vector<std::pair<genome, int>> &v) {
	unsigned int max_index = 0;
	unsigned int i = 1;
	while (i < v.size()) {
		if (v[i].second > v[max_index].second) {
			max_index = i;
		}
		++i;
	}

	return v[max_index];
}


std::ostream& operator<<(std::ostream &os, const genome &g) {
	os << '[';
	for (unsigned int i = 0; i < g.genic_values.size(); ++i) {
		os << g.genic_values[i];
		if (i < g.genic_values.size()-1) {
			os << ", ";
		}
	}
	os << ']';

	return os;
}


std::ostream& operator<<(std::ostream &os, const std::vector<std::vector<double>> &v) {
	os << '[';
	for (unsigned int i = 0; i < v.size(); ++i) {
		os << '[';
		for (unsigned int j = 0; j < v[i].size(); ++j) {
			os << v[i][j];
			if (j < v[i].size()-1) {
				os << ", ";
			}
		}
		os << ']' << std::endl;
	}
	os << ']';

	return os;
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
			
			/* Como en algún momento voy a jugar contra mi propio genoma (por la forma
			   en la que se generan los vecinos) incluyo un chequeo para no considerarlo */
			if (test_genome.genic_values != current_genome.genic_values) {
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
			}
	
			// Agrego los genomas más ganadores que el mío a la lista de resultados
			if (games_won_by_my_team <= games_won_by_opponent) {
				game_results.push_back(std::make_pair(test_genome, games_won_by_opponent));

				log_file << "GENOMA DEL OPONENTE: " << std::endl;
				log_file << test_genome << std::endl;
				log_file << "PARTIDOS GANADOS: " << std::endl;
				log_file << games_won_by_opponent << std::endl;
			}
		}

		/* Busco la combinación de parámetros que me haya ganado la 
		   mayor cantidad de partidos*/
		std::pair<genome, int> best_local_result;
		if (!game_results.empty()) {
			best_local_result = maximum(game_results);
		}
		else {
			best_local_result = std::make_pair(current_genome, current_neighborhood.size());
		}

		log_file << "GENOMA: " << std::endl;
		log_file << best_local_result.first << std::endl;
		log_file << "PARTIDOS GANADOS: " << std::endl;
		log_file << best_local_result.second << std::endl;

		// Si ningún vecino me ganó, soy el más mejor :D
		if (best_local_result.first.genic_values == current_genome.genic_values) {
			break;
		}

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
	}

	log_file.close();

	return 0;
}