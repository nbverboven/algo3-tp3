#include <vector>
#include <fstream>
#include <ostream>
#include <bitset>
#include "constants.hpp"
#include "Util.h"
#include "Referee.h"
#include "GreedyPlayer.h"

#define COLUMNS 10
#define ROWS 5
#define STEPS 100

//#define CONVERGENCE_CRITERION 7
#define INITIAL_POPULATION 10   //Población inicial de la población de genomas
#define GAMES_TO_PLAY 5        //Partidos a jugar

std::random_device _rd;
std::mt19937 _generator(_rd());
static std::uniform_real_distribution<double> urd(-1.0,1.0); // random dice
std::ofstream log_file;

/**
 * Loguea el genoma con una mejor visualización que un arreglo plano.
 * Para debug.
 */
void log(std::ostream& o, genome g){
    o << "Genome {" << std::endl;
    o << " ball_possession: " << g.genic_values[0] << std::endl;
    o << " ball_in_oponent_possession: " << g.genic_values[1] << std::endl;
    o << " ball_free: " << g.genic_values[2] << std::endl;
    o << " goal_distance: " << g.genic_values[3] << std::endl;
    o << " ball_distance: " << g.genic_values[4] << std::endl;
    o << " oponent_with_ball_distance: " << g.genic_values[5] << std::endl;
    o << " dispersion: " << g.genic_values[6] << std::endl;
    o << " distance_ball_oponent_goal: " << g.genic_values[7] << std::endl;
    o << " distance_ball_our_goal: " << g.genic_values[8] << std::endl;
    o << "}"<<std::endl;
}

/**
 * Función de fitness, evalúa el genoma
 */
void EvaluarGenoma(const std::vector<genome> &population, unsigned int genomePoss, std::vector<genome_fitness>& populationFitness){
    std::vector<player> players;
    std::vector<player> opponents;
	for (unsigned int i = 0; i < 3; ++i) {
		players.push_back(player(i, 0.5));
		opponents.push_back(player(i, 0.5));
	}

    GreedyPlayer myTeam(COLUMNS, ROWS, STEPS, IZQUIERDA, players, opponents, population[genomePoss]);
    bool runFirstTeamA = true;
    for (unsigned int oppGenomePoss=genomePoss+1; oppGenomePoss<population.size(); oppGenomePoss++){
        log_file << "Partidos contra genoma nro " << oppGenomePoss << std::endl;
        GreedyPlayer opponentTeam(COLUMNS, ROWS, STEPS, DERECHA, opponents, players, population[oppGenomePoss]);

        /* Pongo a los dos equipos a jugar una cantidad de partidos y 
            registro cuántos ganó cada uno */
        for (unsigned int l=0; l < GAMES_TO_PLAY; l++) {
            Referee ref(COLUMNS, ROWS, STEPS, myTeam, opponentTeam);
            std::string the_winner;
            if(runFirstTeamA)
                the_winner = ref.runPlay(A);
            else
                the_winner = ref.runPlay(B);
            runFirstTeamA = !runFirstTeamA;

            populationFitness[genomePoss].games_played++;
            populationFitness[oppGenomePoss].games_played++;
            populationFitness[genomePoss].goals += ref.getTeamScore(A);
            populationFitness[oppGenomePoss].goals += ref.getTeamScore(B);
            populationFitness[genomePoss].opponent_goals += ref.getTeamScore(B);
            populationFitness[oppGenomePoss].opponent_goals += ref.getTeamScore(A);

            if (the_winner == A) {
                populationFitness[genomePoss].games_won++;
                populationFitness[oppGenomePoss].games_lost++;
            } else if(the_winner == B){
                populationFitness[genomePoss].games_lost++;
                populationFitness[oppGenomePoss].games_won++;
            }
        }
    }
}

/**
 * Evalúa todos los genomas de la población
 */
std::vector<genome_fitness> EvaluarTodosGenomas(std::vector<genome>& population){
    log_file << "--- --- Evaluar todos los genomas --- ---" << std::endl;
    std::vector<genome_fitness> populationFitness(population.size(), genome_fitness());

    for(unsigned int i=0; i<population.size(); i++) {
        log_file << "Evaluando genoma nro " << i << std::endl;
        EvaluarGenoma(population, i,  populationFitness);
    }

    log_file << "--- --- Resultado de los genomas --- ---" << std::endl;
    for(auto gf : populationFitness){
        log_file << gf;
    }

    return populationFitness;
}


int criterioTerminacionEnIteraciones=0;
/**
 * Función que determina cuando terminar el algoritmo genético
 * Por ahora estoy usando una cantidad fija de iteraciones pero podría usar
 * Un criterio por convergencia, calidad de la población, entre otros.
 * Cuando todo funcione OK lo cambio.
 */
bool CriterioTerminacion(std::vector<genome> &genomePopulation, std::vector<genome_fitness>& populationFitness){
    bool cumpleCriterio = false;

    criterioTerminacionEnIteraciones++;
    if(criterioTerminacionEnIteraciones > 10 )
        cumpleCriterio = true;

    return cumpleCriterio;
}

/**
 * Selecciona dos individuos de la población de manera aleatoria
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 */
std::pair<genome,genome> SeleccionarIndividuosRandom(std::vector<genome> &population){
    std::uniform_int_distribution<int> uid(0,population.size()-1); // random dice
    
    int index = uid(_generator);
    genome indiv1 = population[index];
    population.erase(population.begin() + index);
    
    uid = std::uniform_int_distribution<int>(0, population.size()-1);

    index = uid(_generator);
    genome indiv2 = population[index];
    population.erase(population.begin() + index);

    return std::make_pair(indiv1, indiv2);
}

/**
 * Selecciona dos individuos de la población de manera aleatoria
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 */
std::pair<genome,genome> SeleccionarIndividuosByFitness(std::vector<genome> &population, std::vector<genome_fitness> &populationFitness){
    int bestFitness = (populationFitness[0]>populationFitness[1])?0:1;
    int sndBestFitness = (populationFitness[0]>populationFitness[1])?1:0;;

    for(unsigned int i=2; i<population.size(); i++){
        if(populationFitness[i] > populationFitness[bestFitness]){
            sndBestFitness = bestFitness;
            bestFitness = i;
        }else if(populationFitness[i] > populationFitness[sndBestFitness]){
            sndBestFitness = i;
        }
    }

    genome indiv1 = population[bestFitness];
    genome indiv2 = population[sndBestFitness];

    //Veo que indice es mayor (a<b)
    int a = (bestFitness>sndBestFitness)?sndBestFitness:bestFitness;
    int b = (bestFitness>sndBestFitness)?bestFitness:sndBestFitness;

    //Borro los genomas seleccionados primero b (mayor), luego a (no cambia el indice)
    population.erase(population.begin() + b);
    populationFitness.erase(populationFitness.begin() + b);
    population.erase(population.begin() + a);
    populationFitness.erase(populationFitness.begin() + a);

    return std::make_pair(indiv1, indiv2);
}

/**
 * Cruza los genomas por los valores, aleatoreamente
 * Al menos cambia 1 y no puede cambiar todos.
 */
genome CruzarGenomesValues(const genome& g1, const genome& g2){
    int cantGenes = g1.genic_values.size();
    genome cruza(g1);
    std::uniform_int_distribution<int> uid(1,cantGenes-1); // random dist

    int iteraciones = uid(_generator);
    for(int i=0; i<iteraciones; i++){
        uid = std::uniform_int_distribution<int>(0, cantGenes-1);
        int index = uid(_generator);
        cruza.genic_values[index] = g2.genic_values[index];
    }
    
    return cruza;
}

/**
 * Cruza los genomas binariamente, aleatoriamente
 */
genome CruzarGenomesBinary(const genome& g1, const genome& g2){
    int cantGenes = g1.genic_values.size();
    int BIT_SIZE = sizeof(double)*8;// 64;
    std::vector<std::bitset<sizeof(double)*8> > values1(cantGenes);
    std::vector<std::bitset<sizeof(double)*8> > values2(cantGenes);
    std::vector<double> genicValues1 = g1.genic_values;
    std::vector<double> genicValues2 = g2.genic_values;

    //Transformo los genomas a binario
    for(int i=0; i<cantGenes; i++){
        unsigned long long ull_bits1 = *reinterpret_cast<unsigned long long*>(&genicValues1[i]);
        unsigned long long ull_bits2 = *reinterpret_cast<unsigned long long*>(&genicValues2[i]);
        std::bitset<sizeof(double)*8> v1(ull_bits1);
        std::bitset<sizeof(double)*8> v2(ull_bits2);
        values1[i] = v1;
        values2[i] = v2;
    }

    //Me genero aleatoriamente indices de cortes
    std::uniform_int_distribution<int> uid(1,(cantGenes)-1); // random dist
    int cantCortes = uid(_generator);
    uid = std::uniform_int_distribution<int>(0,(cantGenes*BIT_SIZE)-1); // random dist
    std::vector<int> cortes;

    for(int i=0; i<cantCortes; i++){
        int indice_corte = uid(_generator);
        cortes.push_back(indice_corte);
    }
    
    //Ordeno el vector de Cortes
    std::sort (cortes.begin(), cortes.end());
    int c = 0;

    int bitlistSize = cantGenes*BIT_SIZE;
    std::vector<std::bitset<sizeof(double)*8> > bitlist(cantGenes);
    bool useValues2 = false;
    for(int i=0; i<bitlistSize; i++){
        int l = i/BIT_SIZE;
        int r = i%BIT_SIZE;

        if(cortes[c]==i){
            c++;
            useValues2 = !useValues2;
        }
        if(useValues2)
            bitlist[l][r] = values2[l][r];
        else
            bitlist[l][r] = values1[l][r];
    }

    //Transformo el binario a genomas
    std::vector<double> newValues(cantGenes);
    for(int i=0; i<bitlist.size(); i++){        
        unsigned long long ull_bits = bitlist[i].to_ullong();
        *reinterpret_cast<unsigned long long*>(&newValues[i]) = ull_bits;
    }

    return genome(newValues);
}

/**
 * Muta los genomas devolviendo la mutación
 */
genome MutarGenomes(const genome& g1, const genome& g2){
    return g2; //TODO Mutar..
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
        log_file << "------------------ Iteración Nro " << logCantIteaciones;
        log_file <<" ------------------" << std::endl;
        log_file << "Población: "<< genomePopulation.size() << std::endl;
        //Producir una nueva generación
        std::vector<genome> newGenomePopulation;
        std::vector<genome_fitness> newGenomePopulationFitness;

        int sizeCicloReproductivo = genomePopulation.size()/2;
        for(int i=0; i < sizeCicloReproductivo; i++){
            log_file << "Ciclo reproductivo nro " << i << std::endl;
            //Ciclo reproductivo
            //Selecciono dos individuos de la anterior generación.
            std::pair<genome,genome> individuos = SeleccionarIndividuosRandom(genomePopulation);

            //Cruzar (crossover) con cierta probabilidad los dos individuos obteniendo dos descendientes.
            genome descendiente = CruzarGenomesBinary(std::get<0>(individuos), std::get<1>(individuos));

            //Mutar los dos descendientes con cierta probabilidad.
            genome mutacion = MutarGenomes(std::get<0>(individuos), std::get<1>(individuos));

            //Evaluar los dos descendientes mediante la función de fitness. 

            //Insertar los dos descendientes mutados en la nueva generación.
            newGenomePopulation.push_back(descendiente);
            newGenomePopulation.push_back(mutacion);
        }

        genomePopulation = newGenomePopulation;
        genomePopulationFitness = EvaluarTodosGenomas(genomePopulation);
        logCantIteaciones++;
    }

    int bestFitness = 0;
    for(unsigned int i=1; i<genomePopulation.size(); i++){
        if(genomePopulationFitness[i] > genomePopulationFitness[bestFitness])
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