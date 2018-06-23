#include <vector>
#include "Referee.h"
#include "GreedyPlayer.h"

#define ROWS 5
#define COLUMNS 10
#define STEPS 100

#define GAMES_TO_PLAY 60
#define MAX_NUMBER_OF_SEARCHS 8
#define CONVERGENCE_CRITERION 7

std::random_device _rd;
std::mt19937 _generator(_rd());
static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice
std::ofstream log_file;

int main(int argc, char const *argv[])
{
	genome current_genome;
    std::vector<player> players;
    std::vector<player> opponents;
    for (unsigned int i = 0; i < 3; ++i) {
        players.push_back(player(i, 0.5));
        opponents.push_back(player(i, 0.5));
    }
	for (int l = 0; l < 9; l++) {
        current_genome.genic_values[l] = urd(_generator);
    }
	std::ofstream log_file;
    std::string dir = "log/";
    std::string file_name = dir.append(argv[1]);
	log_file.open(file_name);

    int consecutive_maxima_found = 0;
    int last_maximum = 0;
    for (int i = 0; i < MAX_NUMBER_OF_SEARCHS && consecutive_maxima_found < CONVERGENCE_CRITERION; ++i) {
        std::vector<std::vector<double>> neighborhood = getNeighborhood(current_genome);
        std::vector<std::pair<genome, game_series_info>> game_results;
        GreedyPlayer myTeam(COLUMNS, ROWS, STEPS, IZQUIERDA, players, opponents, current_genome);
        bool runFirstTeamA = true;

        for (const std::vector<double> &neighbor : neighborhood) {
    		game_series_info results;
        	genome test_genome(neighbor);
            GreedyPlayer opponentTeam(COLUMNS, ROWS, STEPS, DERECHA, opponents, players, test_genome);

            /* Pongo a los dos equipos a jugar una cantidad de partidos y 
               registro cuántos ganó cada uno. Asumo que soy A */
            for (unsigned int l=0; l < GAMES_TO_PLAY; l++) {
                Referee ref(COLUMNS, ROWS, STEPS, myTeam, opponentTeam);
                std::string the_winner;

                if (runFirstTeamA) {
                    the_winner = ref.runPlay(A);
                } else {
                    the_winner = ref.runPlay(B);
                }
                runFirstTeamA = !runFirstTeamA;

                results.games_played += 1;
                results.points_scored_A += ref.getTeamScore(A);
                results.points_scored_B += ref.getTeamScore(B);
                results.games_won_by_A += (the_winner == A) ? 1 : 0;
                results.games_won_by_B += (the_winner == B) ? 1 : 0;
                results.tied_games += (the_winner == "NINGUNO") ? 1 : 0;
            }

            // Me quedo con un genoma mejor o igual que el mío
            if (results.games_won_by_B >= results.games_won_by_A) {
                game_results.push_back(std::make_pair(test_genome, results));
            }
        } // End for vecino

        // Imprimo al standard output porque lo estaba corriendo con nohup
        if (game_results.empty()) {
            // Nadie es mejor que yo
            std::cout << "Ahora sé que existe un Dios... Soy yo." << std::endl;
            std::cout << current_genome << std::endl;
            break;
        } else {
            // Alguien es igual de bueno o mejor que yo
            std::pair<genome, game_series_info> local_best = maximum(game_results);
            current_genome = local_best.first;

            std::cout << "Nuevo máximo local, bien piola" << std::endl;
        	std::cout << current_genome << std::endl;
            std::cout << "genome_fitness {"           << std::endl;
    	    std::cout << "    games_played: "         << GAMES_TO_PLAY << std::endl;
    	    std::cout << "    games_won A: "          << local_best.second.games_won_by_A  << std::endl;
    	    std::cout << "    games_won B: "          << local_best.second.games_won_by_B  << std::endl;
    	    std::cout << "    games tied: "           << local_best.second.tied_games      << std::endl;
    	    std::cout << "    goals A: "              << local_best.second.points_scored_A << std::endl;
    	    std::cout << "    goals B: "              << local_best.second.points_scored_B << std::endl;
    	    std::cout << "}"                          << std::endl;
    	    std::cout << "--------------------------" << std::endl;

            // Veo si puedo estar convergiendo
            if (local_best.second.games_won_by_A == last_maximum) {
                ++consecutive_maxima_found;
            } else {
                consecutive_maxima_found = 0;
                last_maximum = local_best.second.games_won_by_A;
            }
        }
    } // End for vecindario

    std::cout << "TERMINADO" << std::endl;
    log_file.close();
    return 0;
}