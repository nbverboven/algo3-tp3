#include <vector>
#include <fstream>
#include <ostream>
#include <bitset>
#include "constants.hpp"
#include "Util.h"
#include "Referee.h"
#include "GreedyPlayer.h"
#include "GeneticAlgorithm.h"

#define INITIAL_POPULATION  10  //Población inicial de la población de genomas

std::random_device _rd;
std::mt19937 _generator(_rd());
static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice

/**
 * Uso del ejecutable con los parametros:
 * ./optimize_genomes WON/LOST VALUES/BINARY RANDOM/FITNESS
 */
int main(int argc, char **argv) {

    if (argc != 4) {
        std::cout << "uso: " << argv[0] << "fitnessMethod crossOverMethod selectionMethod" << std::endl;
        return 1;
    }

    // obtiene los argumentos
    CliArguments args;
    args.fitnessMethod = std::string(argv[0]);
    args.crossOverMethod = std::string(argv[1]);
    args.selectionMethod = std::string(argv[2]);

	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

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
    std::vector<genome_fitness> genomePopulationFitness = RunGeneticAlgorithm(genomePopulation, args);

    //Obtengo el mejor fitness
    int bestFitness = 0;
    for(unsigned int i=1; i<genomePopulation.size(); i++){
        if (args.fitnessMethod == FITNESS_WON_GAMES) {
            if(genomePopulationFitness[i].compareFitnessByWonGames(genomePopulationFitness[bestFitness]))
                bestFitness = i;
        } else {
            if(genomePopulationFitness[i].compareFitnessByLostGames(genomePopulationFitness[bestFitness]))
                bestFitness = i;
        }
    }

    //Devuelvo el mejor genoma por consola y log
    std::cout << "----------  Genoma resultante ----------" << std::endl;
    log( std::cout, genomePopulation[bestFitness]);
    std::cout << genomePopulation[bestFitness] << std::endl;
    std::cout << genomePopulationFitness[bestFitness];

	return 0;
}

