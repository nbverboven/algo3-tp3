#include <iostream>
#include <fstream>
#include "Referee.h"
#include "GreedyPlayer.h"

// defines de cada partido
#define ROWS 5
#define COLUMNS 10
#define STEPS 100

// defines de GRASP
#define INITIAL_GENOMES_COUNT 5
#define GAMES_TO_PLAY 9
#define MAX_NUMBER_OF_SEARCHS 8


/**
 * Ejecuta INITIAL_GENOMES_COUNT veces una busqueda local,
 * loggeando al archivo indicado por argv[1], para cada
 * genoma inicial, la sucesion de genomas hasta cortar
 * la busqueda local.
 */
int main(int argc, char const *argv[]) {

    // generador de numeros aleatorios
    std::random_device device;
    std::mt19937 generator(device());
    std::uniform_real_distribution<double> distribution(-1.0,1.0); // random dice

    if (argc != 2) {
        std::cout << "uso: " << argv[0] << " log_file" << std::endl;
        return 1;
    }

    // abre el archivo de log
    std::string logFile = std::string("./log/") + std::string(argv[1]);
    std::ofstream log_file;
    log_file.open(logFile);

    for (int g = 0; g < INITIAL_GENOMES_COUNT; ++g) {
        // crea los jugadores
        std::vector<player> players;
        std::vector<player> opponents;
        for (unsigned int i = 0; i < 3; ++i) {
            players.push_back(player(i, 0.5));
            opponents.push_back(player(i, 0.5));
        }

        // crea el genoma random inicial
        genome current_genome;
        for (int l = 0; l < 9; l++) {
           current_genome.genic_values[l] = distribution(generator);
        }

        log_file << "genoma inicial: " << current_genome << std::endl;

        // busca consecutivamente esta cantidad de vecindarios
        for (int vecinity = 0; vecinity < MAX_NUMBER_OF_SEARCHS; ++vecinity) {
            std::vector<std::vector<double>> neighborhood = getNeighborhood(current_genome);
            GreedyPlayer myTeam(COLUMNS, ROWS, STEPS, IZQUIERDA, players, opponents, current_genome);

            // acumulo los resultados de los partidos ganados
            // por el equipo vecino
            std::vector<std::pair<genome, game_series_info>> game_results;

            // booleano que indica si mi equipo arranca con la pelota.
            // entre cada partido, se complementa su valor para que el
            // otro equipo arranque con la posesion de la pelota
            bool runFirstTeamA = true;

            for (const std::vector<double> &neighbor : neighborhood) {
                game_series_info results;
                // para cada genoma vecino, lo hace competir con el actual
                genome neighbor_genome(neighbor);
                GreedyPlayer opponentTeam(COLUMNS, ROWS, STEPS, DERECHA, opponents, players, neighbor_genome);
                // hago competir a los dos equipos, registrando cuantos gano cada uno
                for (int game = 0; game < GAMES_TO_PLAY; ++game) {
                    // corre el partido con el equipo correcto con la posesion
                    Referee ref(COLUMNS, ROWS, STEPS, myTeam, opponentTeam);
                    std::string initialTeam = runFirstTeamA ? A : B;
                    runFirstTeamA = !runFirstTeamA;
                    std::string winner = ref.runPlay(initialTeam);

                    // registra los resultados del partido actual
                    results.games_played += 1;
                    results.points_scored_A += ref.getTeamScore(A);
                    results.points_scored_B += ref.getTeamScore(B);
                    results.games_won_by_A += (winner == A) ? 1 : 0;
                    results.games_won_by_B += (winner == B) ? 1 : 0;
                    results.tied_games += (winner == "NINGUNO") ? 1 : 0;
                }

                if (results.games_won_by_B >= results.games_won_by_A) {
                    // si el genoma vecino le gano al actual, lo guardo como candidato
                    game_results.push_back(std::make_pair(neighbor_genome, results));
                }
            }

            if (game_results.empty()) {
                // si nadie supero al genoma inicial, loggeo el genoma con el
                // que empece y no hago nada mas para este genoma inicial
                log_file << current_genome << std::endl;
                // adelanto el contador para cortar con la iteracion
                // correspondiente a este genoma inicial
                vecinity = MAX_NUMBER_OF_SEARCHS;
            } else {
                // si tengo genomas que superaron al inicial, me quedo
                // con el mejor de ellos y lo imprimo al archivo, ademas
                // de actualizar current_genome con el mejor vecino
                std::pair<genome, game_series_info> local_best = maximum(game_results);
                current_genome = local_best.first;
                log_file << current_genome << std::endl;
            }
        }

        // al terminar con un genoma random inicial, imprimo un separador
        // para que se note en el archivo de log que cambio de genoma inicial
        log_file << "------------------------------------------------------------------------" << std::endl;
    }

    // cierra el archivo de log
    log_file.close();
    return 0;
}
