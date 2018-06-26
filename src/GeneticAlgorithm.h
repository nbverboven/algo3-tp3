#ifndef GENETIC_ALGORITHM
#define GENETIC_ALGORITHM

#include <vector>
#include <fstream>
#include <ostream>
#include <bitset>
#include "constants.hpp"
#include "Util.h"

std::vector<genome_fitness> RunGeneticAlgorithm(std::vector<genome> genomePopulation);

/**
 * Evalúa todos los genomas de la población
 */
std::vector<genome_fitness> EvaluarTodosGenomas(std::vector<genome>& population);

/**
 * Elvalúa el genoma y le asigna una función de fitness
 */
void EvaluarGenoma(const std::vector<genome> &population, unsigned int genomePoss, std::vector<genome_fitness>& populationFitness);

/**
 * Función que determina cuando terminar el algoritmo genético
 * Por ahora estoy usando una cantidad fija de iteraciones pero podría usar
 * Un criterio por convergencia, calidad de la población, entre otros.
 * Cuando todo funcione OK lo cambio.
 */
bool CriterioTerminacion(std::vector<genome> &genomePopulation, std::vector<genome_fitness>& populationFitness);

/**
 * Selecciona dos individuos de la población de manera aleatoria
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 */
std::pair<genome,genome> SeleccionarIndividuosRandom(std::vector<genome> &population);

/**
 * Selecciona dos individuos de la población de manera aleatoria
 * y los elimina de la población (Para que no se vuelvan a seleccionar)
 */
std::pair<genome,genome> SeleccionarIndividuosByFitness(std::vector<genome> &population, std::vector<genome_fitness> &populationFitness);

/**
 * Cruza los genomas por los valores, aleatoreamente
 * Al menos cambia 1 y no puede cambiar todos.
 */
genome CruzarGenomesValues(const genome& g1, const genome& g2);

/**
 * Cruza los genomas binariamente, aleatoriamente
 */
genome CruzarGenomesBinary(const genome& g1, const genome& g2);

/**
 * Muta los genomas devolviendo la mutación
 */
genome MutarGenomes(const genome& g1, const genome& g2);

#endif
