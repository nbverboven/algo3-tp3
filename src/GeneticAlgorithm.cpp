#include "GeneticAlgorithm.h"
#include <vector>
#include <fstream>
#include <ostream>
#include <bitset>
#include <math.h>       /* modf */
#include "constants.hpp"
#include "Util.h"
#include "Referee.h"
#include "GreedyPlayer.h"

#define COLUMNS 10
#define ROWS 5
#define STEPS 100
#define CRITERIO_TERMINACION_ITERACIONES 10
#define PROBABILIDAD_MUTAR_GEN 0.1       //Probabilidad de que el gen mute
#define GAMES_TO_PLAY 5                  //Partidos a jugar

std::ofstream log_file;

std::random_device local_rd;
std::mt19937 local_generator(local_rd());


std::vector<genome_fitness> RunGeneticAlgorithm(std::vector<genome> genomePopulation, CliArguments& args) {
    //Inicializo el archivo de Log
	log_file.open("log/optimize_genomes.log");

    //Calculo el fitness de cada individuo
    std::vector<genome_fitness> genomePopulationFitness = EvaluarTodosGenomas(genomePopulation);

    int bestFitness;
    int logCantIteaciones = 0;
    //Definir una función de evaluación (fitness) para cada individuo
    while( !CriterioTerminacion(genomePopulation, genomePopulationFitness) ){
        log_file << "------------------ Iteración Nro " << logCantIteaciones <<" ------------------" << std::endl;
        log_file << "Población: "<< genomePopulation.size() << std::endl;

        //Producir una nueva generación
        std::vector<genome> newGenomePopulation;

        int sizeCicloReproductivo = genomePopulation.size()/2;
        for(int i=0; i < sizeCicloReproductivo; i++){ //Ciclo reproductivo
            // log_file << "Ciclo reproductivo nro " << i << std::endl;

            //Selecciono dos individuos de la anterior generación.
            std::pair<genome,genome> individuos;
            if (args.selectionMethod == SELECT_RANDOM) {
                individuos = SeleccionarIndividuosRandom(genomePopulation);
            } else {
                individuos = SeleccionarIndividuosByFitness(genomePopulation, genomePopulationFitness, args);
            }

            //Cruzo (crossover) con cierta probabilidad los dos individuos obteniendo un descendiente
            genome descendiente;
            if (args.crossOverMethod == CROSS_VALUES) {
                descendiente = CruzarGenomesValues(std::get<0>(individuos), std::get<1>(individuos));
            } else {
                descendiente = CruzarGenomesBinary(std::get<0>(individuos), std::get<1>(individuos));
            }

            //Mutar los dos individuos con cierta probabilidad.
            genome mutacion = MutarGenomes(std::get<0>(individuos), std::get<1>(individuos));

            //Inserto los dos descendientes en la nueva generación.
            newGenomePopulation.push_back(descendiente);
            newGenomePopulation.push_back(mutacion);
        }

        genomePopulation = newGenomePopulation;
        //Evalúlo todos los descendientes en la nueva generación
        genomePopulationFitness = EvaluarTodosGenomas(genomePopulation);
        logCantIteaciones++;

        //Logueo el mejor genoma de la población
        bestFitness = getIndexOfBestFitness(genomePopulationFitness, args);
        log_file << "---------- Mejor Genoma de la población ----------" << std::endl;
        log( log_file, genomePopulation[bestFitness]);
        log_file << genomePopulation[bestFitness] << std::endl;
        log_file << genomePopulationFitness[bestFitness];
    }

    //Cierro el archivo log
	log_file.close();

    return genomePopulationFitness;
}


int getIndexOfBestFitness(const std::vector<genome_fitness>& genomePopulationFitness, CliArguments& args){
    int bestFitness = 0;
    for(unsigned int i=1; i<genomePopulationFitness.size(); i++){
        if (args.fitnessMethod == FITNESS_WON_GAMES) {
            if(genomePopulationFitness[i].compareFitnessByWonGames(genomePopulationFitness[bestFitness]))
                bestFitness = i;
        } else {
            if(genomePopulationFitness[i].compareFitnessByLostGames(genomePopulationFitness[bestFitness]))
                bestFitness = i;
        }
    }
    return bestFitness;
}


/**
 * Evalúa todos los genomas de la población
 */
std::vector<genome_fitness> EvaluarTodosGenomas(std::vector<genome>& population){
    log_file << "------- Evaluando todos los genomas -------" << std::endl;
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


/**
 * Elvalúa el genoma y le asigna una función de fitness
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
    if(criterioTerminacionEnIteraciones > CRITERIO_TERMINACION_ITERACIONES )
        cumpleCriterio = true;

    return cumpleCriterio;
}

/**
 * Selecciona dos individuos de la población de manera aleatoria
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 */
std::pair<genome,genome> SeleccionarIndividuosRandom(std::vector<genome> &population){
    std::uniform_int_distribution<int> uid(0,population.size()-1); // random dice

    int index = uid(local_generator);
    genome indiv1 = population[index];
    population.erase(population.begin() + index);

    uid = std::uniform_int_distribution<int>(0, population.size()-1);

    index = uid(local_generator);
    genome indiv2 = population[index];
    population.erase(population.begin() + index);

    return std::make_pair(indiv1, indiv2);
}

/**
 * Selecciona dos individuos de la población segun su fitness
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 *
 * TODO: aca falta parametrizar que funcion de fitness (compareFitnessByWonGames/compareFitnessByLostGames)
 * se debe usar.
 */
std::pair<genome,genome> SeleccionarIndividuosByFitness(
    std::vector<genome> &population,
    std::vector<genome_fitness> &populationFitness,
    CliArguments args) {

    int bestFitness = (populationFitness[0]>populationFitness[1])?0:1;
    int sndBestFitness = (populationFitness[0]>populationFitness[1])?1:0;;

    for(unsigned int i=2; i<population.size(); i++){
        if (args.fitnessMethod == FITNESS_WON_GAMES) {
            if(populationFitness[i].compareFitnessByWonGames(populationFitness[bestFitness])){
                sndBestFitness = bestFitness;
                bestFitness = i;
            } else if(populationFitness[i].compareFitnessByWonGames(populationFitness[sndBestFitness])) {
                sndBestFitness = i;
            }
        } else {
            if(populationFitness[i].compareFitnessByLostGames(populationFitness[bestFitness])){
                sndBestFitness = bestFitness;
                bestFitness = i;
            } else if(populationFitness[i].compareFitnessByLostGames(populationFitness[sndBestFitness])){
                sndBestFitness = i;
            }
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

    int iteraciones = uid(local_generator);
    for(int i=0; i<iteraciones; i++){
        uid = std::uniform_int_distribution<int>(0, cantGenes-1);
        int index = uid(local_generator);
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
    int cantCortes = uid(local_generator);
    uid = std::uniform_int_distribution<int>(0,(cantGenes*BIT_SIZE)-1); // random dist
    std::vector<int> cortes;

    for(int i=0; i<cantCortes; i++){
        int indice_corte = uid(local_generator);
        cortes.push_back(indice_corte);
    }

    //Ordeno el vector de Cortes
    std::sort (cortes.begin(), cortes.end());
    int c = 0;

    //Recorro los cortes
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
    std::uniform_int_distribution<int> uid(0,999); // random dist
    int randomNumber = uid(local_generator);
    genome resultante;

    //Elijo que genoma voy a mutar
    if(randomNumber%2 == 0){
        resultante = g2;
    }else{
        resultante = g1;
    }

    // log_file << "-- -- -- Mutando los Genomas -- -- -- " << std::endl;
    // log(log_file,resultante);

    std::uniform_real_distribution<double> urd(0.0,1.0); // random distribution
    std::normal_distribution<double> normalDist(-1.0,1.0); //normal distribution
    //Recorro todos los genes
    for(int i=0; i<resultante.genic_values.size(); i++){
        //Si el random es menor a la probabilidad, lo muto
        float proba = urd(local_generator);
        if(proba < PROBABILIDAD_MUTAR_GEN){
            // log_file << "Altero el gen nro " << i << std::endl;
            //al gen le sumo un numero de la distribución normal estándar
            double intpart;
            double valor = resultante.genic_values[i] + normalDist(local_generator);
            //Normalizo el valor
            resultante.genic_values[i] = std::modf(valor, &intpart);
        }
    }

    // log(log_file,resultante);

    return g2;
}
