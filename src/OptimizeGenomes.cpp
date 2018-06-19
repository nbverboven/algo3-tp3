#include <vector>
#include <fstream>
#include "constants.hpp"
#include "Referee.h"
#include "GreedyPlayer.h"

#define COLUMNS 10
#define ROWS 5
#define STEPS 50

//#define GAMES_TO_PLAY 20
//#define MAX_NUMBER_OF_SEARCHS 100
//#define CONVERGENCE_CRITERION 7
#define INITIAL_POPULATION 10

void log(std::ofstream &f, genome g){
    f << "ball_possession: " << g.ball_possession << std::endl;
    f << "ball_in_oponent_possession: " << g.ball_in_oponent_possession << std::endl;
    f << "ball_free: " << g.ball_free << std::endl;
    f << "goal_distance: " << g.goal_distance << std::endl;
    f << "ball_distance: " << g.ball_distance << std::endl;
    f << "oponent_with_ball_distance: " << g.oponent_with_ball_distance << std::endl;
    f << "dispersion: " << g.dispersion << std::endl;
    f << "distance_ball_oponent_goal: " << g.distance_ball_oponent_goal << std::endl;
    f << "distance_ball_our_goal: " << g.distance_ball_our_goal << std::endl;
}

std::random_device rd;
std::mt19937 generator(rd());
static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice

struct genome_fitness {
    int games_played;
    int games_won;
    int games_lost;
    int goals;
    int oponent_goals;
};

/**
 * Función de fitness, evalúa el genoma
 */
genome_fitness EvaluarGenoma(const genome& g, const std::vector<genome> &population){

}

int criterioTerminacionEnIteraciones=0;
/**
 * Función que determina cuando terminar el algoritmo genético
 */
bool CriterioTerminacion(std::vector<genome> &genomePopulation){
    bool cumpleCriterio = false;

    criterioTerminacionEnIteraciones++;
    if(criterioTerminacionEnIteraciones > 5 )
        cumpleCriterio = true;

    return cumpleCriterio;
}

/**
 * Selecciona dos individuos de la población
 */
std::pair<genome,genome> SeleccionarIndividuos(std::vector<genome> &population){
    std::uniform_int_distribution<int> uid(0,population.size()); // random dice
    
    int index = uid(generator);
    genome indiv1 = population[index];
    population.erase(population.begin() + index);
    
    std::uniform_int_distribution<int> uid2(0,population.size()); // random dice

    index = uid2(generator);
    genome indiv2 = population[index];
    population.erase(population.begin() + index);

    return std::make_pair(indiv1, indiv2);
}


genome CruzarGenomes(const genome& g1, const genome& g2){

}

genome MutarGenomes(const genome& g1, const genome& g2){
    
}


//int argc, char **argv 
int main() { 

	std::vector<player> players;
	std::vector<player> opponents;

	for (int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

    //Inicializo el archivo de Log
	std::ofstream log_file;
	log_file.open("log/optimize_genomes.log");

    //Población inicial
    std::vector<genome> genomePopulation;
    std::vector<genome_fitness> genomePopulationFitness;

    for(int i=0; i<INITIAL_POPULATION; i++) {
        genome newGenome;
        newGenome.ball_possession = urd(generator);
        newGenome.ball_in_oponent_possession = urd(generator);
        newGenome.ball_free = urd(generator);
        newGenome.goal_distance = urd(generator);
        newGenome.ball_distance = urd(generator);
        newGenome.oponent_with_ball_distance = urd(generator);
        newGenome.dispersion = urd(generator);
        newGenome.distance_ball_oponent_goal = urd(generator);
        newGenome.distance_ball_our_goal = urd(generator);

        genomePopulation.push_back(newGenome);
        //genome_fitness gf = EvaluarGenoma(newGenome);
        //genomePotuplationFitness.push_back(gf);
    }

    //Definir una función de evaluación (fitness) para cada individuo
    while( !CriterioTerminacion(genomePopulation) ){
        //Producir una nueva generación
        std::vector<genome> newGenomePopulation;
        std::vector<genome_fitness> newGenomePopulationFitness;

        for(int i=0; i<genomePopulation.size()/2; i++){
            //Ciclo reproductivo
            //Selecciono dos individuos de la anterior generación.
            std::pair<genome,genome> individuos = SeleccionarIndividuos(genomePopulation);

            //Cruzar (crossover) con cierta probabilidad los dos individuos obteniendo dos descendientes.
            genome descendiente = CruzarGenomes(std::get<0>(individuos), std::get<1>(individuos));

            //Mutar los dos descendientes con cierta probabilidad.
            genome mutacion = MutarGenomes(std::get<0>(individuos), std::get<1>(individuos));

            //Evaluar los dos descendientes mediante la función de fitness. 
            //genome_fitness descendiente_fitness = EvaluarGenoma(descendiente);
            //genome_fitness mutacion_fitness = EvaluarGenoma(mutacion);

            //Insertar los dos descendientes mutados en la nueva generación.
            newGenomePopulation.push_back(descendiente);
            newGenomePopulation.push_back(mutacion);
            //newGenomePopulationFitness.push_back(descendiente_fitness);
            //newGenomePopulationFitness.push_back(mutacion_fitness);
        }

        genomePopulation = newGenomePopulation;
        genomePopulationFitness = newGenomePopulationFitness;
    }

    //Cierro el archivo log
	log_file.close();

	return 0;
}