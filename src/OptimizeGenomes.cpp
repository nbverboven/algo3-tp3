#include <vector>
#include <fstream>
#include <ostream>
#include <bitset>
#include "constants.hpp"
#include "Util.h"
#include "Referee.h"
#include "GreedyPlayer.h"
#include "GeneticAlgorithm.h"

const int COLUMNS = 10;
const int ROWS = 5;
const int STEPS = 100;
//const int CONVERGENCE_CRITERION = 7
const int CRITERIO_TERMINACION_ITERACIONES = 10;
const int INITIAL_POPULATION = 10;   //Población inicial de la población de genomas
const int GAMES_TO_PLAY = 5;        //Partidos a jugar



std::random_device _rd;
std::mt19937 _generator(_rd());
static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice
std::ofstream log_file;

//int argc, char **argv 
int main() {

	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

    //Inicializo el archivo de Log
	log_file.open("log/optimize_genomes.log");

    //Población inicial
    std::vector<genome> genomePopulation(INITIAL_POPULATION);

    //Inicializo los genomas con valores random
    for(int i=0; i<INITIAL_POPULATION; i++) {
        genome newGenome;
        for(int l=0; l<9; l++)
            newGenome.genic_values[l] = urd(_generator);
        genomePopulation[i] = newGenome;
    }

    //Corre el algoritmo genético
    std::vector<genome_fitness> genomePopulationFitness = RunGeneticAlgorithm(genomePopulation);

    int bestFitness = 0;
    for(unsigned int i=1; i<genomePopulation.size(); i++){
        //TODO: En caso de que se parametrize la función de fitness, acá debo cambiarla.
        if(genomePopulationFitness[i].compareFitnessByWonGames(genomePopulationFitness[bestFitness]))
            bestFitness = i;
    }

    //Devuelvo el mejor genoma por consola y log
    std::cout << "----------  Genoma resultante ----------" << std::endl;
    log( std::cout, genomePopulation[bestFitness]);
    std::cout << genomePopulation[bestFitness] << std::endl;
    std::cout << genomePopulationFitness[bestFitness];
    log_file << "----------  Genoma resultante ----------" << std::endl;
    log( log_file, genomePopulation[bestFitness]);
    log_file << genomePopulation[bestFitness];
    log_file << genomePopulationFitness[bestFitness];

    //Cierro el archivo log
	log_file.close();

	return 0;
}

