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
    std::vector<genome_fitness> genomePopulationFitness;

    //Inicializo los genomas con valores random
    for(int i=0; i<INITIAL_POPULATION; i++) {
        genome newGenome;
        for(int l=0; l<9; l++)
            newGenome.genic_values[l] = urd(_generator);
        genomePopulation[i] = newGenome;
    }

    //Calculo el fitness de cada individuo
    genomePopulationFitness = EvaluarTodosGenomas(genomePopulation);

    int logCantIteaciones = 0;
    //Definir una función de evaluación (fitness) para cada individuo
    while( !CriterioTerminacion(genomePopulation, genomePopulationFitness) ){
        log_file << "------------------ Iteración Nro " << logCantIteaciones <<" ------------------" << std::endl;
        log_file << "Población: "<< genomePopulation.size() << std::endl;
        //Producir una nueva generación
        std::vector<genome> newGenomePopulation;
        std::vector<genome_fitness> newGenomePopulationFitness;

        int sizeCicloReproductivo = genomePopulation.size()/2;
        for(int i=0; i < sizeCicloReproductivo; i++){ //Ciclo reproductivo
            log_file << "Ciclo reproductivo nro " << i << std::endl;

            //Selecciono dos individuos de la anterior generación.
            std::pair<genome,genome> individuos = SeleccionarIndividuosRandom(genomePopulation);

            //Cruzo (crossover) con cierta probabilidad los dos individuos obteniendo un descendiente
            genome descendiente = CruzarGenomesBinary(std::get<0>(individuos), std::get<1>(individuos));

            //Mutar los dos individuos con cierta probabilidad.
            genome mutacion = MutarGenomes(std::get<0>(individuos), std::get<1>(individuos));

            //Inserto los dos descendientes en la nueva generación.
            newGenomePopulation.push_back(descendiente);
            newGenomePopulation.push_back(mutacion);
        }

        genomePopulation = newGenomePopulation;
        //Evalúlo todos los descendientes en la nueva generación
        genomePopulationFitness = EvaluarTodosGenomas(genomePopulation);
        logCantIteaciones++;
    }

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

