#include <vector>
#include "Referee.h"
#include "GreedyPlayer.h"

#define ROWS 5
#define COLUMNS 10
#define STEPS 100

#define GAMES_TO_PLAY 60
#define MAX_NUMBER_OF_SEARCHS 10

int main(int argc, char const *argv[])
{
	// Generador de números (pseudo) aleatorios
	std::random_device _rd;
	std::mt19937 _generator(_rd());
	static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice

	// Crea los jugadores
    std::vector<player> players;
    std::vector<player> opponents;
    for (unsigned int i = 0; i < 3; ++i) {
        players.push_back(player(i, 0.5));
        opponents.push_back(player(i, 0.5));
    }

    // Creo el genoma inicial con valores al azar
	genome current_genome;
	for (int l = 0; l < 9; l++) {
        current_genome.genic_values[l] = urd(_generator);
    }

	// Abro el archivo de log
    std::string log_file_name = std::string("./log/") + std::string(argv[1]);
    std::ofstream log_file;
    log_file.open(log_file_name);

    for (int vecinity = 0; vecinity < MAX_NUMBER_OF_SEARCHS; ++vecinity) {
        std::vector<std::vector<double>> neighborhood = getNeighborhood(current_genome);
        GreedyPlayer myTeam(COLUMNS, ROWS, STEPS, IZQUIERDA, players, opponents, current_genome);

        /* Acumulo los resultados de los partidos ganados por el 
           equipo vecino */
        std::vector<std::pair<genome, game_series_info>> game_results;

        /* Booleano que indica si mi equipo arranca con la pelota.
           entre cada partido, se complementa su valor para que el
           otro equipo arranque con la posesion de la pelota */
        bool runFirstTeamA = true;

        for (const std::vector<double> &neighbor : neighborhood) {
    		game_series_info results;
        	genome test_genome(neighbor);
            GreedyPlayer opponentTeam(COLUMNS, ROWS, STEPS, DERECHA, opponents, players, test_genome);

            /* Pongo a los dos equipos a jugar una cantidad de partidos y 
               registro cuántos ganó cada uno. Asumo que soy A */
            for (unsigned int game = 0; game < GAMES_TO_PLAY; ++game) {
                Referee ref(COLUMNS, ROWS, STEPS, myTeam, opponentTeam);

                std::string initialTeam = runFirstTeamA ? A : B;
                std::string the_winner = ref.runPlay(initialTeam);
                runFirstTeamA = !runFirstTeamA;

                // Registro los resultados del partido
                results.games_played += 1;
                results.points_scored_A += ref.getTeamScore(A);
                results.points_scored_B += ref.getTeamScore(B);
                results.games_won_by_A += (the_winner == A) ? 1 : 0;
                results.games_won_by_B += (the_winner == B) ? 1 : 0;
                results.tied_games += (the_winner == "NINGUNO") ? 1 : 0;
            }

            // Si el genoma vecino le gano al actual, lo guardo como candidato
            if (results.games_won_by_B >= results.games_won_by_A) {
                game_results.push_back(std::make_pair(test_genome, results));
            }
        }

        if (game_results.empty()) {
            /* Si nadie supero al genoma inicial, loggeo el genoma con el
               que empece y no hago nada mas para este genoma inicial */
            std::cout << "Ahora sé que existe un Dios... Soy yo." << std::endl;
            std::cout << current_genome << std::endl;
            vecinity = MAX_NUMBER_OF_SEARCHS;
        } else {
            /* Si tengo al menos un genoma que superó al inicial, me quedo
               con el mejor de ellos y lo imprimo al archivo, ademas
               de actualizar current_genome con el mejor vecino */
            std::pair<genome, game_series_info> local_best = maximum(game_results);
            current_genome = local_best.first;

            std::cout << "Nuevo máximo local, bien piola" << std::endl;
        	std::cout << current_genome << std::endl;
            std::cout << "genome_fitness {"     << std::endl;
    	    std::cout << "    games_played: "   << GAMES_TO_PLAY << std::endl;
    	    std::cout << "    games_won A: "    << local_best.second.games_won_by_A  << std::endl;
    	    std::cout << "    games_won B: "    << local_best.second.games_won_by_B  << std::endl;
    	    std::cout << "    games tied: "     << local_best.second.tied_games      << std::endl;
    	    std::cout << "    goals A: "        << local_best.second.points_scored_A << std::endl;
    	    std::cout << "    goals B: "        << local_best.second.points_scored_B << std::endl;
    	    std::cout << "}"                    << std::endl;
    	    std::cout << "------------------------------------------" << std::endl;
        }
    }

    std::cout << "TERMINADO" << std::endl;
    log_file.close();
    return 0;
}